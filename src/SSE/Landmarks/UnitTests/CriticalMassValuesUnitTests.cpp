/**
 * @file CriticalMassValuesUnitTests.cpp
 * @author Evren Imre
 * @date 11 Dec 2023	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2023 Evren Imre
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <SSE/Landmarks/CriticalMassValues.h>

#include <Exceptions/PreconditionError.h>
#include <Generic/Quantity.h>
#include <UnitTestUtils/RandomTestFixture.h>
#include <UnitTestUtils/UnitTestUtilityFunctions.h>

BOOST_FIXTURE_TEST_SUITE( CriticalMassValuesTests, Herd::UnitTestUtils::RandomTestFixture )

// Testing only validation
// Correctness tests are performed through the single star evolution unit tests
BOOST_AUTO_TEST_CASE( ValidationTest, *Herd::UnitTestUtils::Labels::s_Compile )
{
  Herd::Generic::Metallicity valid( GenerateMetallicity() );
  Herd::Generic::Metallicity invalid( -valid );

  BOOST_CHECK_NO_THROW( Herd::SSE::ComputeMhook( valid ) );
  BOOST_CHECK_THROW( Herd::SSE::ComputeMhook( invalid ), Herd::Exceptions::PreconditionError );

  BOOST_CHECK_NO_THROW( Herd::SSE::ComputeMHeF( valid ) );
  BOOST_CHECK_THROW( Herd::SSE::ComputeMHeF( invalid ), Herd::Exceptions::PreconditionError );

  BOOST_CHECK_NO_THROW( Herd::SSE::ComputeMFGB( valid ) );
  BOOST_CHECK_THROW( Herd::SSE::ComputeMFGB( invalid ), Herd::Exceptions::PreconditionError );
}

BOOST_AUTO_TEST_SUITE_END( )
