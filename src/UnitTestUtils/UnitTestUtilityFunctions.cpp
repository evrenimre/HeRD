/**
 * @file UnitTestUtilityFunctions.cpp
 * @author Evren Imre
 * @date 18 Apr 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "UnitTestUtilityFunctions.h"

#include <cstddef>
#include <unordered_set>
#include <vector>

#include <boost/algorithm/string.hpp>

namespace
{

std::unordered_set< std::string > s_RegisteredArguments { "--seed" };

}

namespace Herd::UnitTestUtils
{

bool CheckCurrentTestCaseStatus()
{
  auto testId = boost::unit_test::framework::current_test_case().p_id;
  return boost::unit_test::results_collector.results( testId ).passed();
}

/**
 * @return Key-value pairs for arguments
 * @remarks If an argument does not have a value, it is assigned an empty string
 */
std::unordered_map< std::string, std::string > ParseCommandLineArguments()
{

  std::unordered_map< std::string, std::string > output;

  std::size_t argCount = boost::unit_test::framework::master_test_suite().argc;
  for( std::size_t c = 1; c < argCount; ++c )
  {
    std::vector< std::string > parsed;
    boost::split( parsed, boost::unit_test::framework::master_test_suite().argv[ c ], boost::is_any_of( "=" ), boost::token_compress_off );

    if( !( parsed.size() == 1 || parsed.size() == 2 ) )
    {
      BOOST_TEST( false, "Unable to parse argument " << boost::unit_test::framework::master_test_suite().argv[ c ] << ". Ignoring command line arguments" );
      // @formatter:off
      return {};
      // @formatter:oon
    }

    if( s_RegisteredArguments.count( parsed[ 0 ] ) == 0 )
    {
      BOOST_TEST( false, "Unregistered argument " + parsed[ 0 ] + ". Ignoring command line arguments" );
      // @formatter:off
      return {};
            // @formatter:on
    }

    output.emplace( parsed[ 0 ], parsed.size() == 1 ? std::string() : parsed[ 1 ] );
  }

  return output;
}

/**
 * @param i_rName Argument name
 * @return Argument value. Unset if not found
 */
std::optional< std::string > GetCommandLineArgument( const std::string& i_rName )
{
  if( s_RegisteredArguments.count( i_rName ) == 0 )
  {
    BOOST_TEST( false, "Requested unregistered argument " + i_rName + " . Ignoring command line arguments" );
    return std::nullopt;
  }


  std::unordered_map< std::string, std::string > arguments = ParseCommandLineArguments();
  if( auto iQuery = arguments.find( i_rName ); iQuery == arguments.end() )
  {
    return std::nullopt;
  } else
  {
    return iQuery->second;
  }
}

}


