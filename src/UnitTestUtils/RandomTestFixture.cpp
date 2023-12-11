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

#define BOOST_TEST_DYN_LINK

#include "RandomTestFixture.h"

#include "UnitTestUtilityFunctions.h"

#include <ctime>
#include <cstddef>
#include <vector>

#include <boost/lexical_cast.hpp>

namespace Herd::UnitTestUtils
{

// Explicit instatiations
///@cond
// Suppress incorrect doxygen warnings
template double RandomTestFixture::GenerateNumber( double, double ); // @suppress("Member declaration not found")
template double RandomTestFixture::GenerateNumber(); // @suppress("Member declaration not found")
///@endcond

RandomTestFixture::RandomTestFixture() :
    m_Seed( std::time( nullptr ) )
{
  TrySetSeedFromCommandLine();

  // This adds a top-level context to report the rng seed in the event of a test failure
  boost::unit_test::framework::add_context( BOOST_TEST_LAZY_MSG( "Re-run the test with parameter --seed=" << m_Seed << " to reproduce" ), true );
}

/**
 * @param i_Seed New seed
 * @post RandomTestFixture::m_Rng is unset
 */
void RandomTestFixture::SetSeed( unsigned int i_Seed )
{
  m_Seed = i_Seed;
  m_Rng.reset();
}

/**
 * @return A reference to RandomTestFixture::m_Rng
 */
std::mt19937& RandomTestFixture::Rng()
{

  InitialiseLazy();
  return *m_Rng;
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

/**
 * @post RandomTestFixture::m_Rng is initialised
 */
void RandomTestFixture::InitialiseLazy()
{
  // Lazy initialisation. Avoids redundant construction if user sets a different seed before first call to the generator
  // mt19937 constructor is expensive.
  if( !m_Rng )
  {
    m_Rng = std::mt19937( m_Seed );
  }

}

/**
 * @return A random metallicity value
 * @remarks No unit test- \c GenerateNumber is tested
 * @cite Hurley00
 */
double RandomTestFixture::GenerateMetallicity()
{
  return GenerateNumber( 1e-4, 0.03 );  // Metallicity range from Hurley00 // @suppress("Invalid arguments")
}

/**
 * @return A random mass value in \fS M_{odot}\fS
 * @remarks No unit test- \c GenerateNumber is tested
 * @cite AMUSE
 */
double RandomTestFixture::GenerateMass()
{
  return GenerateNumber( 0.1, 100. ); // Mass range from AMUSE // @suppress("Invalid arguments")
}

/**
 * @return Returns a random luminosity value
 * @remarks No unit test- \c GenerateNumber is tested
 * @remarks https://en.wikipedia.org/wiki/List_of_star_extremes
 */
double RandomTestFixture::GenerateLuminosity()
{
  return GenerateNumber( 0.000126, 6166000. );  // @suppress("Invalid arguments")
}

/**
 * @return Returns a random radius value
 * @remarks No unit test- \c GenerateNumber is tested
 * @remarks https://en.wikipedia.org/wiki/List_of_star_extremes
 */
double RandomTestFixture::GenerateRadius()
{
  return GenerateNumber( 0.084, 2150. );  // @suppress("Invalid arguments")
}

} // namespace Herd::UnitTestUtils
