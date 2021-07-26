#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# @file TrackGenereratorSSE.py
# @author Evren Imre
# @date 26 Jun 2021

# This file is a part of HeRD, a stellar evolution library
# Copyright © 2021 Evren Imre
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

"""Script for generating unit test data from SSE tracks. It runs SSE for over all mass-metallicity pairs in the user-specified mass and metallicity ranges and converts the output to xml. It generates
- Evolutionary tracks for each pair
- ZAMS unit test data, as the first entry in each track
"""

import argparse
import datetime
from lxml import etree
import os
import re
import uuid

import RunSSE
import SSEHelpers


_version = '1.0.1'
_version_attr = 'Version'
_uuid_attr = 'UUID'
_timestamp_attr = 'Timestamp'
_source_attr = 'Source'
_source_string = 'SSE'

_zams_tag = 'ZAMS'

_track_filename_pattern = '{}_{}.track.xml'
_zams_filename = 'ZAMS.trackpoints.xml'


def generate_tracks_and_zams(sse_dir, output_dir, mass_range, z_range, age):
    """Runs SSE over all combinations of mass and z, and saves each track to an xml file. Also compiles the ZAMS test data from the tracks
    """
    os.chdir(output_dir)

    uuid_string = str(uuid.uuid4())
    timestamp_string = datetime.datetime.now(
        datetime.timezone.utc).isoformat()

    zams_points = []   # Zero-age main sequence entries

    def _add_root_attr(root):
        root.attrib[_version_attr] = _version
        root.attrib[_uuid_attr] = uuid_string
        root.attrib[_timestamp_attr] = timestamp_string
        root.attrib[_source_attr] = _source_string

    for mass in mass_range:
        for z in z_range:
            RunSSE.run_sse(mass, z, age, sse_dir, output_dir)

            track = SSEHelpers.load_sse_track(
                os.path.join(output_dir, 'evolve.dat'))

            SSEHelpers.remove_attributes(track, ['Mo', 'Epoch'])

            # SSE does not compute the evolution of the composition. So we only
            # know the initial value
            SSEHelpers.add_constant_attribute(track, 'Z', '?')
            SSEHelpers.add_attribute_at(track, "Z", z, 0)

            _add_root_attr(track)

            etree.ElementTree(track).write(
                _track_filename_pattern.format(mass, z), encoding='utf-8', xml_declaration=True, pretty_print=True)

            # First entry corresponds to the zero-age main sequence state
            zams_points.append(SSEHelpers.get_node(track, 0))

    # ZAMS
    zams = etree.Element(_zams_tag)
    zams.extend(zams_points)  # Add the nodes to the tree
    _add_root_attr(zams)
    etree.ElementTree(zams).write(
        _zams_filename, encoding='utf-8', xml_declaration=True, pretty_print=True)

    print("Version: {} uuid: {}".format(_version, uuid_string))


if __name__ == "__main__":
    # @cond
    parser = argparse.ArgumentParser(
        description='Arguments for track generation')
    parser.add_argument('-s', '--sse-binary-dir',
                        dest='sse_dir', help='Directory for the SSE binary')
    parser.add_argument('-o', '--output-dir', dest='output_dir',
                        help='Directory for the output files')
    parser.add_argument('-m', '--mass', dest='mass_values',
                        nargs='+', default='1', help='Mass values')
    parser.add_argument('-z', '--metallicity', dest='z_values',
                        nargs='+', default='0.02', help='Metallicity values')
    parser.add_argument('-a', '--age', dest='age',
                        default='13800', help='Cut-off point for evolution')

    args = parser.parse_args()

    generate_tracks_and_zams(args.sse_dir, args.output_dir,
                             args.mass_values, args.z_values, args.age)
    # @endcond
