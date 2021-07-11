/**
 * @file PreconditionErrorUnitTests.cpp
 * @author Evren Imre
 * @date 11 Jul 2021	
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

#include <Exceptions/PreconditionError.h>

#include <boost/mpl/list.hpp>

BOOST_FIXTURE_TEST_SUITE( Exceptions, Herd::UnitTestUtils::RandomTestFixture, *Herd::UnitTestUtils::Labels::s_Compile )

/// Types under test
using Types = boost::mpl::list< float, double, long double, int, long int>;

/// Helpers
BOOST_AUTO_TEST_CASE_TEMPLATE( TestExceptionHelpers, T, Types )
{
  {
    double value = GenerateNumber( -10., 0. ); // @suppress("Invalid arguments")
    BOOST_CHECK_THROW( Herd::Exceptions::PreconditionError::ThrowIfNegative( T( value ), "NegativeQuantity" ), Herd::Exceptions::PreconditionError );
    Herd::Exceptions::PreconditionError::ThrowIfNegative( T( 0 ), "ZeroValue" );
    Herd::Exceptions::PreconditionError::ThrowIfNegative( T( -value ), "PositiveValue" );
  }

  {
    double value = GenerateNumber( -10., 0. ); // @suppress("Invalid arguments")
    BOOST_CHECK_THROW( Herd::Exceptions::PreconditionError::ThrowIfNotPositive( T( value ), "NegativeValue" ), Herd::Exceptions::PreconditionError );
    BOOST_CHECK_THROW( Herd::Exceptions::PreconditionError::ThrowIfNotPositive( T( 0 ), "ZeroValue" ), Herd::Exceptions::PreconditionError );
    Herd::Exceptions::PreconditionError::ThrowIfNotPositive( T( -value ), "PositiveValue" );
  }

}

BOOST_AUTO_TEST_SUITE_END( )
