/**
 * @file QuantityRangeUnitTests.cpp
 * @author Evren Imre
 * @date 19 Jun 2021	
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

#include <Generic/Quantity.h>
#include <Generic/QuantityRange.h>

#include <boost/mpl/list.hpp>
#include <boost/icl/interval.hpp>

BOOST_FIXTURE_TEST_SUITE( Generic, Herd::UnitTestUtils::RandomTestFixture, *Herd::UnitTestUtils::Labels::s_Compile )

/// Quantity range construction and usage
BOOST_AUTO_TEST_CASE( TestQuantityRange )
{
  double lower = GenerateNumber( 0., 10. ); // @suppress("Invalid arguments")
  double upper = GenerateNumber( lower, 10. ) + 1; // @suppress("Invalid arguments")

  Herd::Generic::Mass minMass( lower );
  Herd::Generic::Mass maxMass( upper );
  Herd::Generic::Mass midMass( ( lower + upper ) / 2 );

  Herd::Generic::OpenRange openRange( lower, upper );
  BOOST_TEST( !openRange.Contains( minMass ) );
  BOOST_TEST( !openRange.Contains( maxMass ) );
  BOOST_TEST( openRange.Contains( midMass ) );


  Herd::Generic::LeftOpenRange leftOpenRange( lower, upper );
  BOOST_TEST( !leftOpenRange.Contains( minMass ) );
  BOOST_TEST( leftOpenRange.Contains( maxMass ) );
  BOOST_TEST( leftOpenRange.Contains( midMass ) );

  Herd::Generic::RightOpenRange rightOpenRange( lower, upper );
  BOOST_TEST( rightOpenRange.Contains( minMass ) );
  BOOST_TEST( !rightOpenRange.Contains( maxMass ) );
  BOOST_TEST( rightOpenRange.Contains( midMass ) );

  Herd::Generic::ClosedRange closedRange( lower, upper );
  BOOST_TEST( closedRange.Contains( minMass ) );
  BOOST_TEST( closedRange.Contains( maxMass ) );
  BOOST_TEST( closedRange.Contains( midMass ) );
}

/// Range types under test
using QuantityRangeTypes = boost::mpl::list< Herd::Generic::OpenRange, Herd::Generic::LeftOpenRange, Herd::Generic::RightOpenRange, Herd::Generic::ClosedRange >;

/// Quantity range construction and usage
BOOST_AUTO_TEST_CASE_TEMPLATE( TestQuantityRangeTemplated, T, QuantityRangeTypes )
{
  double lower = GenerateNumber( 0., 10. ); // @suppress("Invalid arguments")
  double upper = GenerateNumber( lower, 10. ) + 1; // @suppress("Invalid arguments")
  T interval( lower, upper );

  BOOST_TEST( interval.Lower() == lower );
  BOOST_TEST( interval.Upper() = upper );

  BOOST_TEST( boost::icl::lower( interval.Range() ) == lower );
  BOOST_TEST( boost::icl::upper( interval.Range() ) == upper );

  {
    Herd::Generic::Mass minMass( lower );
    if( interval.Contains( minMass ) )
    {
      interval.ThrowIfNotInRange( minMass, "InRange" );
    } else
    {
      BOOST_CHECK_THROW( interval.ThrowIfNotInRange( minMass, "OutOfRange" ), Herd::Exceptions::PreconditionError );
    }
  }

}

BOOST_AUTO_TEST_SUITE_END( )
