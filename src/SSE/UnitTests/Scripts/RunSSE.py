#!/usr/bin/env python3
# -*- coding: utf-8 -*-

## @file RunSSE.py
#  @author Evren Imre
#  @date 23 May 2021    
 
# This file is a part of HeRD, a stellar evolution library
# Copyright © 2021 Evren Imre 
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/

"""A script to run the original implementation of SSE, available as a part of https://github.com/amusecode/amuse
"""

import argparse
import os

def run_sse( mass, z, age, sse_dir, output_dir ):
    
    # Attempt creating the output directory
    if os.path.isdir(output_dir) is False: 
        os.mkdir(output_dir)
    else:
        # Check write permission
        if os.access(output_dir, os.W_OK) is False:
            raise PermissionError("No write access for {}".format(output_dir))

    # SSE binary needs evolve.in in the current working directory, and also writes evolve.dat there
    os.chdir(output_dir)    

        # Create new evolve.in
    with open('evolve.in', 'w') as file:
        file.write( "{} {} {}\n".format( mass, z, age))
        
        #Defaults
        file.write("0.5 0.0 0.5 190.0 \n")
        file.write("0 1 0 1 3.0 999 \n")
        file.write("0.05 0.01 0.02 \n")
    
    # Run with the new evolve.in
    os.system(os.path.join(sse_dir, 'sse'))
    
if __name__ == "__main__":

    parser = argparse.ArgumentParser(description='Arguments for running SSE')
    parser.add_argument('-s', '--sse-binary-dir', dest = 'sse_dir', help = 'Directory for the SSE binary')
    parser.add_argument('-o', '--output-dir', dest = 'output_dir', help = 'Directory for the output files')
    parser.add_argument('-m', '--mass', dest = 'mass', default = '1', help = 'Initial mass')
    parser.add_argument('-z', '--metallicity', dest = 'z', default = '0.02', help = 'Metallicity')
    parser.add_argument('-a', '--age', dest = 'age', default = '7000', help = 'Age in Myears' )
    
    args = parser.parse_args()

    run_sse(args.mass, args.z, args.age, args.sse_dir, args.output_dir) 
