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

#include <filesystem>
#include <regex>
#include <string>
#include <unordered_map>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace Herd::UnitTestUtils
{
/**
 * @brief Fixture for loading external test data
 * @remarks Helpers for working with command line arguments
 */
class DataLoaderFixture
{
public:

  DataLoaderFixture();  ///< Constructor

  // Directory operations
  boost::property_tree::ptree ReadAsXML( const std::string& i_rRegex, std::size_t i_Index ); ///< Among the files in DataLoaderFixture::m_DataDir that satisfy the regex, reads the one indicated by the index into a property tree
  std::unordered_map< std::string, boost::property_tree::ptree > ReadAsXML( const std::string& i_rRegex ) const; ///< Reads each file in DataLoaderFixture::m_DataDir matching the regex into a property tree
  unsigned int GetFileCount( const std::string& i_rRegex ) const; ///< Number of files in DataLoaderFixture::m_DataDir matching the regex

private:

  static boost::property_tree::ptree ReadAsXML( const std::filesystem::path& i_Path );  ///< Reads an XML file to a property tree
  static bool IsMatchingFile( const std::filesystem::directory_entry& i_rEntry, const std::regex& i_rRegex ); ///< Checks whether a path is a regular file with a name having a substring that matches the regex

  std::filesystem::path m_DataDir; ///< Value of the command line argument DataLoaderFixture::s_DataDirArgumentName

  inline static const std::string s_DataDirArgumentName = "--data-dir";  ///< Command line argument for the test data dir
};
}



#endif /* HCCFFE697_C182_4118_946F_17FE9544A7BF */
