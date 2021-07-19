/**
 * @file ExceptionWrappersUnitTests.cpp
 * @author Evren Imre
 * @date 19 Jul 2021	
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

#include <Exceptions/ExceptionWrappers.h>
#include <Exceptions/PreconditionError.h>

BOOST_FIXTURE_TEST_SUITE( ExceptionWrappersTestSuite, Herd::UnitTestUtils::RandomTestFixture, *Herd::UnitTestUtils::Labels::s_Compile )

BOOST_AUTO_TEST_CASE( TestExceptionWrappers )
{
  BOOST_CHECK_THROW( Herd::Exceptions::ThrowPreconditionError( "Bad", "Good", "BadString" ), Herd::Exceptions::PreconditionError );
  BOOST_CHECK_THROW( Herd::Exceptions::ThrowPreconditionError( "Bad", "Good", 0. ), Herd::Exceptions::PreconditionError );

  {
    double value = GenerateNumber( -10., -1. ); // @suppress("Invalid arguments")
    BOOST_CHECK_THROW( Herd::Exceptions::ThrowPreconditionErrorIfNegative( value, "NegativeQuantity" ), Herd::Exceptions::PreconditionError );
    Herd::Exceptions::ThrowPreconditionErrorIfNegative( 0., "ZeroValue" );
    Herd::Exceptions::ThrowPreconditionErrorIfNegative( -value, "PositiveValue" );
  }

  {
    double value = GenerateNumber( -10., -1. ); // @suppress("Invalid arguments")
    BOOST_CHECK_THROW( Herd::Exceptions::ThrowPreconditionErrorIfNotPositive( value, "NegativeQuantity" ), Herd::Exceptions::PreconditionError );
    BOOST_CHECK_THROW( Herd::Exceptions::ThrowPreconditionErrorIfNotPositive( 0., "ZeroValue" ), Herd::Exceptions::PreconditionError );
    Herd::Exceptions::ThrowPreconditionErrorIfNotPositive( -value, "PositiveValue" );
  }
}

BOOST_AUTO_TEST_SUITE_END( )



