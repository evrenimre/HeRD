/**
 * @file QuantityUnitTests.cpp
 * @author Evren Imre
 * @date 14 Apr 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <boost/test/unit_test.hpp>

#include <UnitTestUtils/RandomTestFixture.h>
#include <UnitTestUtils/UnitTestUtilityFunctions.h>

#include <Generic/Quantities.h>

#include <boost/mpl/list.hpp>

BOOST_FIXTURE_TEST_SUITE( Generic, Herd::UnitTestUtils::RandomTestFixture, *Herd::UnitTestUtils::Labels::s_Compile )

/// Quantity types under test
using QuantityTypes = boost::mpl::list< Herd::Generic::Luminosity, Herd::Generic::Mass, Herd::Generic::Metallicity, Herd::Generic::Radius, Herd::Generic::Age, Herd::Generic::AngularMomentum >;

/// Quantity construction and usage
BOOST_AUTO_TEST_CASE_TEMPLATE( TestQuantity, T, QuantityTypes )
{
  double value = GenerateNumber( 0., 10. );
  T quantity( value );
  BOOST_TEST( value == quantity.Value() );

  double newValue = GenerateNumber( 0., 10. );
  quantity.Set( newValue );
  BOOST_TEST( newValue == quantity.Value() );

  // Conversion operator
  double converted = quantity;
  BOOST_TEST( converted == quantity.Value() );

  // Equality operator
  BOOST_TEST( quantity == quantity );
  BOOST_TEST( !( quantity == T( quantity.Value() + 1 ) ) );
}

/// Helpers
BOOST_AUTO_TEST_CASE_TEMPLATE( TestQuantityHelpers, T, QuantityTypes )
{
  {
    double value = GenerateNumber( -10., 0. );
    BOOST_CHECK_THROW( Herd::Generic::ThrowIfNegative( T( value ), "NegativeQuantity" ), Herd::Exceptions::PreconditionError );
    Herd::Generic::ThrowIfNegative( T( 0 ), "ZeroQuantity" );
    Herd::Generic::ThrowIfNegative( T( -value ), "PositiveQuantity" );
  }

  {
    double value = GenerateNumber( -10., 0. );
    BOOST_CHECK_THROW( Herd::Generic::ThrowIfNotPositive( T( value ), "NegativeQuantity" ), Herd::Exceptions::PreconditionError );
    BOOST_CHECK_THROW( Herd::Generic::ThrowIfNotPositive( T( 0 ), "ZeroQuantity" ), Herd::Exceptions::PreconditionError );
    Herd::Generic::ThrowIfNotPositive( T( -value ), "PositiveQuantity" );
  }

}

BOOST_AUTO_TEST_SUITE_END( )
