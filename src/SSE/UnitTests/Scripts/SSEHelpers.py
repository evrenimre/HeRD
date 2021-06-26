#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# @file SSEHelpers.py
# @author Evren Imre
# @date 26 Jun 2021

# This file is a part of HeRD, a stellar evolution library
# Copyright © 2021 Evren Imre
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

from _operator import index
from lxml import etree
import os
import re


_evolution_stages = [
    'MSLM',
    'MS',
    'HG',
    'FGB',
    'CHeB',
    'FAGB',
    'SAGB',
    'HeMS',
    'HeHG',
    'HeWD',
    'COWD',
    'ONWD',
    'NS',
    'BH',
    'MSn'
]

_track_point_tag = 'TrackPoint'
_track_tag = "Track"


def load_sse_track(dat_path):
    """Loads an sse track to an xml tree
    """

    track_points = []

    with open(os.path.join(dat_path), 'r') as file:

        # Skip the column headers, start from one
        # Ignore the last line. It just repeats the state at the end of the
        # evolution. Effectively a sentinel
        lines = file.readlines()
        for line in lines[1:len(lines) - 1]:
            tokens = re.split(' +', line.lstrip().rstrip())

            track_points.append(
                etree.Element(
                    _track_point_tag,
                    Age=tokens[0],
                    Stage=_evolution_stages[int(float(tokens[1]))],
                    Mo=tokens[2],
                    M=tokens[3],
                    logL=tokens[4],
                    logR=tokens[5],
                    logT=tokens[6],
                    Mc=tokens[7],
                    Menv=tokens[8],
                    Epoch=tokens[9],
                    Spin=tokens[10]
                ))

    track = etree.Element(_track_tag)
    track.extend(track_points)  # Add the nodes to the tree

    return track


def remove_attributes(track, to_remove):
    """Removes specified attributes from a track
    """

    for track_point in track.xpath('//{}//{}'.format(_track_tag, _track_point_tag)):
        for name in to_remove:
            track_point.attrib.pop(name)


def add_constant_attribute(track, name, value):
    """Adds/replaces an constant-valued attribute to all track points
    """
    for track_point in track.xpath('//{}//{}'.format(_track_tag, _track_point_tag)):
        track_point.attrib[name] = value


def add_attribute_at(track, name, value, index):
    """Adds/replaces an attribute for the node with the index
    """

    track.xpath('//{}//{}'.format(_track_tag, _track_point_tag)
                )[index].attrib[name] = value
