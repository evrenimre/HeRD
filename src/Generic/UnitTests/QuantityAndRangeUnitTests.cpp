/**
 * @file QuantityAndRangeUnitTests.cpp
 * @author Evren Imre
 * @date 14 Apr 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <boost/test/unit_test.hpp>

#include <UnitTestUtils/RandomTestFixture.h>

#include <Generic/Quantities.h>
#include <Generic/QuantityRange.h>

#include <boost/mpl/list.hpp>

BOOST_FIXTURE_TEST_SUITE( Generic, Herd::UnitTestUtils::RandomTestFixture )

/// Quantity types under test
using QuantityTypes = boost::mpl::list< Herd::Generic::Luminosity, Herd::Generic::Mass, Herd::Generic::Metallicity, Herd::Generic::Radius, Herd::Generic::Age >;

/// Quantity construction and usage
BOOST_AUTO_TEST_CASE_TEMPLATE( TestQuantity, T, QuantityTypes )
{
  double value = GenerateReal( 0., 10. );
  T quantity( value );
  BOOST_TEST( value == quantity.Value() );

  double newValue = GenerateReal( 0., 10. );
  quantity.Set( newValue );
  BOOST_TEST( newValue == quantity.Value() );

  // Conversion operator
  double converted = quantity;
  BOOST_TEST( converted == quantity.Value() );

  // Equality operator
  BOOST_TEST( quantity == quantity );
  BOOST_TEST( !( quantity == T( quantity.Value() + 1 ) ) );
}

/// Quantity range construction and usage
BOOST_AUTO_TEST_CASE( TestQuantityRange )
{
  double lower = GenerateReal( 0., 10. );
  double upper = GenerateReal( lower, 10.0 ) + 1;

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

BOOST_AUTO_TEST_SUITE_END( )
