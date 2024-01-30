/**
 * @file RgComputerUnitTests.cpp
 * @author Evren Imre
 * @date 29 Jan 2024	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2024 Evren Imre
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include "SSETestUtils.h"

#include <Exceptions/PreconditionError.h>
#include <SSE/EvolutionStage.h>
#include <SSE/RgComputer.h>
#include <UnitTestUtils/RandomTestFixture.h>
#include <UnitTestUtils/UnitTestUtilityFunctions.h>

BOOST_FIXTURE_TEST_SUITE( RgComputerTestsTests, Herd::UnitTestUtils::RandomTestFixture )

// Only testing input validation
// Correctness tests are performed through the single star evolution unit tests
BOOST_AUTO_TEST_CASE( ValidationTest, *Herd::UnitTestUtils::Labels::s_Compile )
{
  Herd::Generic::Metallicity validMetallicity( GenerateMetallicity() );
  Herd::Generic::Metallicity invalidMetallicity( -validMetallicity );

  // Invalid construction
  BOOST_CHECK_THROW( std::make_unique< Herd::SSE::RgComputer >( invalidMetallicity ), Herd::Exceptions::PreconditionError );

  // Valid construction
  Herd::SSE::RgComputer rgComputer( validMetallicity );

  // Interface
  Herd::SSE::EvolutionState validState = Herd::SSE::UnitTests::GenerateRandomEvolutionState( Rng() );
  Herd::SSE::EvolutionState invalidState;

  BOOST_CHECK_THROW( rgComputer.ComputeRg( invalidState ), Herd::Exceptions::PreconditionError );
  BOOST_CHECK_NO_THROW( rgComputer.ComputeRg( validState ) );
}

BOOST_AUTO_TEST_SUITE_END( )
