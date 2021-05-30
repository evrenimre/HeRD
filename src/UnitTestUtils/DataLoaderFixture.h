/**
 * @file DataLoaderFixture.h
 * @author Evren Imre
 * @date 30 May 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HCCFFE697_C182_4118_946F_17FE9544A7BF
#define HCCFFE697_C182_4118_946F_17FE9544A7BF

#include <string>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace Herd::UnitTestUtils
{
/**
 * @brief Fixture for loading external test data
 */
class DataLoaderFixture
{
public:

  static boost::property_tree::ptree ReadXML();  ///< Reads an XML file into a property tree

private:

  inline static const std::string s_DataArgumentName = "--data";  ///< Command line argument for the test data
};
}



#endif /* HCCFFE697_C182_4118_946F_17FE9544A7BF */
