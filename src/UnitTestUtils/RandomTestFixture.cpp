/**
 * @file RandomTestFixture.cpp
 * @author Evren Imre
 * @date 18 Apr 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "RandomTestFixture.h"

#include "UnitTestUtilityFunctions.h"

#include <ctime>
#include <cstddef>
#include <iostream>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/test/unit_test.hpp>

namespace Herd::UnitTestUtils
{

RandomTestFixture::RandomTestFixture() :
    m_Seed( std::time( NULL ) )
{
  if( boost::unit_test::framework::master_test_suite().argc > 1 )
  {
    TrySetSeedFromCommandLine();
  }

  // This adds a top-level context to report the rng seed in the event of a test failure
  boost::unit_test::framework::add_context( BOOST_TEST_LAZY_MSG( "Re-run the test with parameter --seed=" << m_Seed << " to reproduce" ), true );
}

void RandomTestFixture::SetSeed( unsigned int i_Seed )
{
  m_Seed = i_Seed;
  m_Rng.reset();
}

/**
 * @remarks No state change if fails
 */
void RandomTestFixture::TrySetSeedFromCommandLine()
{
  // Scan the command line parameters for the random seed
  std::size_t argCount = boost::unit_test::framework::master_test_suite().argc;
  for( std::size_t c = 1; c < argCount; ++c )
  {
    std::vector< std::string > parsed;
    boost::split( parsed, boost::unit_test::framework::master_test_suite().argv[ c ], boost::is_any_of( "=" ), boost::token_compress_off );
    if( parsed.size() == 2 && parsed[ 0 ] == s_SeedParameterName )
    {
      if( unsigned int Seed = 0; boost::conversion::try_lexical_convert( parsed[ 1 ], Seed ) )
      {
        SetSeed( Seed );
      } else
      {
        BOOST_TEST( false, ( "Ignoring invalid seed value " + parsed[ 1 ] ).data() ); // Passing a string confuses the indexer
      }
    }
  }
}

/**
 * @param i_Probability Probability of returning \c true
 * @return A random boolean value
 */
bool RandomTestFixture::GenerateBool( double i_Probability )
{
  return GenerateNumber( 0., 1. ) < i_Probability;
}

} // namespace Herd::UnitTestUtils
