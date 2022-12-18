/**
 * @file RandomTestFixtureUnitTests.cpp
 * @author Evren Imre
 * @date 8 May 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <UnitTestUtils/RandomTestFixture.h>
#include <UnitTestUtils/UnitTestUtilityFunctions.h>

#include <cstddef>
#include <limits>
#include <utility>

#include <boost/mpl/list.hpp>

#include <range/v3/algorithm.hpp>

///< Numeric types under test
using TestTypes = boost::mpl::list< double, int, unsigned int >;

BOOST_AUTO_TEST_SUITE( UnitTestUtils, *Herd::UnitTestUtils::Labels::s_Compile )


/// Random number generation
BOOST_AUTO_TEST_CASE_TEMPLATE( Generators, T, TestTypes )
{
  Herd::UnitTestUtils::RandomTestFixture Fixture;

  {
    long double rolled = Fixture.GenerateNumber< T >(); // Convert to largest numerical type. Otherwise overflow/underflow would mask incorrect operation
    BOOST_TEST( rolled >= static_cast< long double >( std::numeric_limits< T >::lowest() ) );
    BOOST_TEST( rolled <= static_cast< long double >( std::numeric_limits< T >::max() ) );
  }

  {
    T lowerBound = Fixture.GenerateNumber< T >();
    T upperBound = Fixture.GenerateNumber< T >();
    if( lowerBound > upperBound )
    {
      std::swap( lowerBound, upperBound );
    }

    T rolled = Fixture.GenerateNumber( lowerBound, upperBound );
    BOOST_TEST( rolled >= lowerBound );
    BOOST_TEST( rolled <= upperBound );
  }

  // Randomness tests
  {
    // Generate some numbers. At least one of them must be different
    std::size_t setSize = 100;
    std::vector< T > randomNumbers( setSize );
    ranges::generate( randomNumbers, [ & ]()
    { return Fixture.GenerateNumber< T >();} );

    BOOST_TEST( ranges::any_of( randomNumbers, [ & ]( T i_Value )
    { return randomNumbers[0] != i_Value;} ) );
  }
}

///< Rest of the interface
BOOST_AUTO_TEST_CASE( RandomTestFixtureInterface )
{
  Herd::UnitTestUtils::RandomTestFixture Fixture;

  {
    auto newSeed = Fixture.Seed() + 1;
    Fixture.SetSeed( newSeed );
    BOOST_TEST( newSeed == Fixture.Seed() );
  }

  {
    char flag = Fixture.GenerateBool();
    BOOST_TEST( ( flag == 0 || flag == 1 ) );

    // Loaded coin
    BOOST_TEST( Fixture.GenerateBool( 1.0 ) );
    BOOST_TEST( !Fixture.GenerateBool( 0.0 ) );
  }
}

BOOST_AUTO_TEST_SUITE_END( )



