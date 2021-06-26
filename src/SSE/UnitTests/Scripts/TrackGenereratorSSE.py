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

import argparse
from lxml import etree
import os
import re

import RunSSE
import SSEHelpers


def generate_tracks(sse_dir, output_dir, mass_range, z_range, age):
    """Runs SSE over all combinations of mass and z, and saves each track to an xml file
    """
    os.chdir(output_dir)

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
            etree.ElementTree(track).write(
                "{}_{}.herd.track.xml".format(mass, z), pretty_print=True)


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

    generate_tracks(args.sse_dir, args.output_dir,
                    args.mass_values, args.z_values, args.age)
    # @endcond
