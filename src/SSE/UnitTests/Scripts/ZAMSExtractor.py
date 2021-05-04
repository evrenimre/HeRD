## @file ZAMSExtractor.py
#  @author Evren Imre
#  @date 02 May 2021    
 
# This file is a part of HeRD, a stellar evolution library
# Copyright © 2021 Evren Imre 
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/
 
"""A script to extract some zero-age main sequence properties from MIST EEP files available at http://waps.cfa.harvard.edu/MIST/model_grids.html
"""

import os
import lxml.etree

import read_mist_models # https://github.com/jieunchoi/MIST_codes/blob/master/scripts/read_mist_models.py

rootdir = os.path.normpath(r"/home/evren/Code/Stellar Evolution/Scratchpad/MIST EEP/")  # Root for the EEP directories
zams_eep_index = 201    # EEP index for ZAMS

stars = []  # Tree nodes

# Directory scraping for eep files
dir_list = [dir for dir in os.listdir(rootdir) if os.path.isdir(dir)]
for dir in dir_list:
    file_list = [ file for file in os.listdir(dir) if os.path.splitext(file)[1]==".eep"]
    for file in file_list:
        eep = read_mist_models.EEP( os.path.join(dir, file))
        
        # Copy the values at ZAMS
        stars.append(
            lxml.etree.Element(
                'Star',  
                M = str(eep.minit), 
                Z = str(eep.abun['Zinit']), 
                Age = str(eep.eeps['star_age'][zams_eep_index]),
                L = str(10**eep.eeps['log_L'][zams_eep_index]),
                R = str(10**eep.eeps['log_R'][zams_eep_index]),
                T = str(10**eep.eeps['log_Teff'][zams_eep_index])           
            ))

tree = lxml.etree.Element('Catalogue')            
tree.extend(stars)  # Add the nodes to the tree

lxml.etree.ElementTree(tree).write(os.path.join(rootdir, "ZAMSUnitTestData.xml"), pretty_print = True)
