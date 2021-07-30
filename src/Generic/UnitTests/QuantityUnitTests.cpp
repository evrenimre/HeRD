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

#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <UnitTestUtils/RandomTestFixture.h>
#include <UnitTestUtils/UnitTestUtilityFunctions.h>

#include <Exceptions/PreconditionError.h>
#include <Generic/Quantity.h>

#include <boost/mpl/list.hpp>

BOOST_FIXTURE_TEST_SUITE( Generic, Herd::UnitTestUtils::RandomTestFixture, *Herd::UnitTestUtils::Labels::s_Compile )

/// Quantity types under test
using QuantityTypes = boost::mpl::list< Herd::Generic::Luminosity, Herd::Generic::Mass, Herd::Generic::Metallicity, Herd::Generic::Radius, Herd::Generic::Age, Herd::Generic::AngularMomentum, Herd::Generic::AngularVelocity >;

/// Quantity construction
BOOST_AUTO_TEST_CASE_TEMPLATE( UsageTest, T, QuantityTypes )
{
  double value = GenerateNumber( 0., 10. ); // @suppress("Invalid arguments")
  T quantity( value );
  BOOST_TEST( value == quantity.Value() );

  double newValue = GenerateNumber( 0., 10. ); // @suppress("Invalid arguments")
  quantity.Set( newValue );
  BOOST_TEST( newValue == quantity.Value() );

  // Conversion operator
  double converted = quantity;
  BOOST_TEST( converted == quantity.Value() );
}

BOOST_AUTO_TEST_CASE_TEMPLATE( ComparisonOperatorsTest, T, QuantityTypes )
{
  T greater( GenerateNumber( 0., 10. ) ); // @suppress("Invalid arguments")
  T smaller( GenerateNumber( 0., greater * 0.99 ) );  // @suppress("Invalid arguments")

  // Equality operator
  BOOST_TEST( greater == greater );
  BOOST_TEST( greater != smaller );

  // Comparison operators
  BOOST_TEST( greater > smaller );
  BOOST_TEST( smaller < greater );

  BOOST_TEST( smaller <= greater );
  BOOST_TEST( smaller <= smaller );

  BOOST_TEST( greater >= greater );
  BOOST_TEST( greater >= smaller );
}

BOOST_AUTO_TEST_CASE_TEMPLATE( ArithmeticOperatorsTest, T, QuantityTypes )
{
  T left( GenerateNumber( 0.1, 10. ) ); // @suppress("Invalid arguments")
  T right( GenerateNumber( 0.1, left * 0.99 ) );  // @suppress("Invalid arguments")

  BOOST_TEST( left + right == left.Value() + right.Value() );
  BOOST_TEST( left - right == left.Value() - right.Value() );
  BOOST_TEST( left * right == left.Value() * right.Value() );
  BOOST_TEST( left / right == left.Value() / right.Value() );
}

/// Helpers
BOOST_AUTO_TEST_CASE_TEMPLATE( QuantityHelpersTest, T, QuantityTypes )
{
  {
    double value = GenerateNumber( -10., 0. ); // @suppress("Invalid arguments")
    BOOST_CHECK_THROW( Herd::Generic::ThrowIfNegative( T( value ), "NegativeQuantity" ), Herd::Exceptions::PreconditionError );
    Herd::Generic::ThrowIfNegative( T( 0 ), "ZeroQuantity" );
    Herd::Generic::ThrowIfNegative( T( -value ), "PositiveQuantity" );
  }

  {
    double value = GenerateNumber( -10., 0. ); // @suppress("Invalid arguments")
    BOOST_CHECK_THROW( Herd::Generic::ThrowIfNotPositive( T( value ), "NegativeQuantity" ), Herd::Exceptions::PreconditionError );
    BOOST_CHECK_THROW( Herd::Generic::ThrowIfNotPositive( T( 0 ), "ZeroQuantity" ), Herd::Exceptions::PreconditionError );
    Herd::Generic::ThrowIfNotPositive( T( -value ), "PositiveQuantity" );
  }

}

BOOST_AUTO_TEST_SUITE_END( )
