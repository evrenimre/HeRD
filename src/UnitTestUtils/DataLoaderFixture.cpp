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

#define BOOST_TEST_DYN_LINK

#include "DataLoaderFixture.h"

#include "UnitTestUtilityFunctions.h"

#include <algorithm>

#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/test/unit_test.hpp>

#include <range/v3/algorithm.hpp>
#include <range/v3/view.hpp>

namespace Herd::UnitTestUtils
{

DataLoaderFixture::DataLoaderFixture()
{
  std::optional< std::string > dataPath = Herd::UnitTestUtils::GetCommandLineArgument( s_DataPathArgumentName );
  if( dataPath )
  {
    m_DataPath = *dataPath;
    BOOST_TEST_REQUIRE( std::filesystem::exists( m_DataPath ), "Does not exist: " + *dataPath );
    BOOST_TEST_REQUIRE( std::filesystem::is_regular_file( m_DataPath ), "Not a file: " + *dataPath );
  }

  std::optional< std::string > dataDir = Herd::UnitTestUtils::GetCommandLineArgument( s_DataDirArgumentName );
  if( dataDir )
  {
    m_DataDir = *dataDir;
    BOOST_TEST_REQUIRE( std::filesystem::exists( m_DataDir ), "Does not exist: " + *dataPath );
    BOOST_TEST_REQUIRE( std::filesystem::is_directory( m_DataDir ), "Not a file: " + *dataPath );
  }
}

/**
 * @return Data as a property tree
 * @remarks The test framework will bail if the operation cannot be completed
 */
boost::property_tree::ptree DataLoaderFixture::ReadAsXML() const
{
  if( m_DataPath.empty() )
  {
    BOOST_TEST_REQUIRE( false, "No filename was specified. Missing command line argument " + s_DataPathArgumentName + "?" );
  }
  
  return ReadAsXML( m_DataPath );
}

/**
 * @param i_rRegex Regular expression
 * @param i_Index Index among the files matching the regular expression
 * @return Property tree
 * @remarks Substring match on filename
 */
boost::property_tree::ptree DataLoaderFixture::ReadAsXML( const std::string& i_rRegex, std::size_t i_Index )
{
  std::regex filename_regex( i_rRegex );
  auto iBegin = begin( std::filesystem::directory_iterator( m_DataDir ) );
  auto iEnd = end( std::filesystem::directory_iterator( m_DataDir ) );
  auto dir = boost::make_iterator_range( iBegin, iEnd );  // std::span does not work with directory iterators

  boost::optional< std::filesystem::path > filepath;

  //@formatter:off
  for( const auto& current : dir
      | boost::adaptors::filtered( [ & ]( const auto& i_rEntry ) { return IsMatchingFile(i_rEntry, filename_regex);} )
      | boost::adaptors::indexed())
      //@formatter:on
  {
    if( static_cast< std::size_t >( current.index() ) == i_Index ) // @suppress("Method cannot be resolved")
    {
    filepath = current.value(); // @suppress("Method cannot be resolved")
      break;
    }

  }
  
  if( !filepath )
  {
    BOOST_TEST_REQUIRE( false, "Index exceeds the number of eligible files. Value: " + std::to_string( i_Index ) );
  }

  return ReadAsXML( *filepath );
}

/**
 * @param i_rRegex Regex
 * @return Property trees for the files
 * @remarks Substring match on filename
 */
std::unordered_map< std::string, boost::property_tree::ptree > DataLoaderFixture::ReadAsXML( const std::string& i_rRegex ) const
{
  std::regex filename_regex( i_rRegex );
  auto iBegin = begin( std::filesystem::directory_iterator( m_DataDir ) );
  auto iEnd = end( std::filesystem::directory_iterator( m_DataDir ) );
  auto dir = ranges::subrange( iBegin, iEnd ); // @suppress("Symbol is not resolved")

  std::unordered_map< std::string, boost::property_tree::ptree > forest;
  //@formatter:off
  ranges::cpp20::for_each( dir
      | ranges::views::remove_if( [ & ]( const auto& i_rEntry ){ return !IsMatchingFile(i_rEntry, filename_regex);} ),
      [ & ]( const auto& i_rEntry ){ forest.emplace( i_rEntry.path().filename().string(), ReadAsXML( i_rEntry.path() ) );} );
//@formatter:on

  return forest;
}

/**
 * @param i_rRegex Regular expression
 * @return Number of files in the directory matching the regex
 * @remarks Substring match on filename
 */
unsigned int DataLoaderFixture::GetFileCount( const std::string& i_rRegex ) const
{
  std::regex filename_regex( i_rRegex );

  auto iBegin = begin( std::filesystem::directory_iterator( m_DataDir ) );
  auto iEnd = end( std::filesystem::directory_iterator( m_DataDir ) );
  return std::count_if( iBegin, iEnd, [ & ]( const auto& i_rEntry ) // @suppress("Invalid arguments")
  { return IsMatchingFile(i_rEntry, filename_regex);} );
}

/**
 * @param i_Path Path to the xml file
 * @return Property tree
 */
boost::property_tree::ptree DataLoaderFixture::ReadAsXML( const std::filesystem::path& i_Path )
{
  boost::property_tree::ptree data;
  try
  {
    boost::property_tree::read_xml( i_Path.string(), data );
  } catch( ... )
  {
    BOOST_TEST_REQUIRE( false, "Unable to load file " + i_Path.string() );
  }

  return data;
}

/**
 * @param i_rEntry Directory entry
 * @param i_rRegex Regex
 * @return \c true if the path points to a regular file and the filename has a substring that matches the regex
 */
bool DataLoaderFixture::IsMatchingFile( const std::filesystem::directory_entry& i_rEntry, const std::regex& i_rRegex )
{
  return i_rEntry.is_regular_file() && std::regex_search( i_rEntry.path().filename().string(), i_rRegex );
}

}


