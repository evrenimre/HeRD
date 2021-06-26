#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# @file ZAMSDataGeneratorSSE.py
# @author Evren Imre
# @date 23 May 2021

# This file is a part of HeRD, a stellar evolution library
# Copyright © 2021 Evren Imre
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/

"""A script to generate and extract some zero-age main sequence properties from the output of the original implementation of SSE, available as a part of https://github.com/amusecode/amuse
"""

import argparse
from lxml import etree
import os
import re

import RunSSE


def generate_data(sse_dir, output_dir, mass_range, z_range):
    """Runs SSE over all combinations of mass and z, and extracts the ZAMS properties to an xml file
    """

    os.chdir(output_dir)

    stars = []  # Tree nodes

    for mass in mass_range:
        for z in z_range:
            RunSSE.run_sse(mass, z, 0, sse_dir, output_dir)

            # Get the second line of evolve.dat
            with open(os.path.join(output_dir, 'evolve.dat'), 'r') as file:
                lines = file.readlines()
                # Starts with an empty element
                zams = re.split(' +', lines[1])[1:]

                stars.append(
                    etree.Element(
                        'Star',
                        M=zams[2],
                        Z=z,
                        Age=zams[0],
                        L=str(10**float(zams[4])),
                        R=str(10**float(zams[5])),
                        T=str(10**float(zams[6]))
                    ))

    tree = etree.Element('Catalogue')
    tree.extend(stars)  # Add the nodes to the tree

    lxml.ElementTree(tree).write(
        "ZAMSUnitTestDataSSE.xml", pretty_print=True)


if __name__ == "__main__":
    # @cond
    parser = argparse.ArgumentParser(
        description='Arguments for ZAMS data generation')
    parser.add_argument('-s', '--sse-binary-dir',
                        dest='sse_dir', help='Directory for the SSE binary')
    parser.add_argument('-o', '--output-dir', dest='output_dir',
                        help='Directory for the output files')
    parser.add_argument('-m', '--mass', dest='mass_values',
                        nargs='+', default='1', help='Mass values')
    parser.add_argument('-z', '--metallicity', dest='z_values',
                        nargs='+', default='0.02', help='Metallicity values')

    args = parser.parse_args()

    generate_data(args.sse_dir, args.output_dir,
                  args.mass_values, args.z_values)
    # @endcond
