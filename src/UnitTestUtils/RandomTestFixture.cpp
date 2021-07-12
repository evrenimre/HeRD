/**
 * @file RandomTestFixture.cpp
 * @author Evren Imre
 * @date 18 Apr 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "RandomTestFixture.h"

#include "UnitTestUtilityFunctions.h"

#include <ctime>
#include <cstddef>
#include <vector>

#include <boost/lexical_cast.hpp>

namespace Herd::UnitTestUtils
{

// Explicit instatiations
template double RandomTestFixture::GenerateNumber< double >( double, double );
template double RandomTestFixture::GenerateNumber< double >();


RandomTestFixture::RandomTestFixture() :
    m_Seed( std::time( nullptr ) )
{
  TrySetSeedFromCommandLine();

  // This adds a top-level context to report the rng seed in the event of a test failure
  boost::unit_test::framework::add_context( BOOST_TEST_LAZY_MSG( "Re-run the test with parameter --seed=" << m_Seed << " to reproduce" ), true );
}

void RandomTestFixture::SetSeed( unsigned int i_Seed )
{
  m_Seed = i_Seed;
  m_Rng.reset();
}

/**
 * @return Returns \c m_Seed
 */
uint_fast32_t RandomTestFixture::Seed()
{
  return m_Seed;
}

/**
 * @remarks No state change if fails
 */
void RandomTestFixture::TrySetSeedFromCommandLine()
{
  if( auto seedString = Herd::UnitTestUtils::GetCommandLineArgument( s_SeedParameterName ); seedString )
  {
    if( unsigned int seed = 0; boost::conversion::try_lexical_convert( *seedString, seed ) )
    {
      [[likely]] SetSeed( seed );
    } else
    {
      BOOST_TEST_REQUIRE( false, "Invalid seed value " + *seedString ); // Passing a string confuses the indexer
    }
  }
}

/**
 * @param i_Probability Probability of returning \c true
 * @return A random boolean value
 */
bool RandomTestFixture::GenerateBool( double i_Probability )
{
  return GenerateNumber( 0., 1. ) < i_Probability; // @suppress("Invalid arguments")
}

} // namespace Herd::UnitTestUtils
