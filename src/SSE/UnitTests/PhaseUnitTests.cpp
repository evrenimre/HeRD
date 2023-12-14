/**
 * @file PhaseUnitTests.cpp
 * @author Evren Imre
 * @date 14 Dec 2023	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2023 Evren Imre
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#define BOOST_TEST_DYN_LINK

#include "SSETestUtils.h"

#include <boost/test/unit_test.hpp>

#include <Exceptions/PreconditionError.h>
#include <Generic/Quantity.h>
#include <SSE/EvolutionState.h>
#include <SSE/IPhase.h>
#include <SSE/MainSequence.h>
#include <UnitTestUtils/RandomTestFixture.h>
#include <UnitTestUtils/UnitTestUtilityFunctions.h>

#include <memory>
#include <map>

namespace
{

/**
 * @brief Phase names
 */
enum class EPhaseType
{
  e_MS
};
}

BOOST_FIXTURE_TEST_SUITE( LandmarkTests, Herd::UnitTestUtils::RandomTestFixture )

// Only testing input validation
// Correctness tests are performed through the single star evolution unit tests
BOOST_AUTO_TEST_CASE( ValidationTest, *Herd::UnitTestUtils::Labels::s_Compile )
{
  Herd::Generic::Metallicity validMetallicity( GenerateMetallicity() );
  Herd::Generic::Metallicity invalidMetallicity( -validMetallicity );

  std::map< EPhaseType, std::unique_ptr< Herd::SSE::IPhase > > Phases;

  // Invalid construction
  BOOST_CHECK_THROW( Phases.emplace( EPhaseType::e_MS, std::make_unique< Herd::SSE::MainSequence >( invalidMetallicity ) ),
      Herd::Exceptions::PreconditionError );

  // Valid construction
  BOOST_CHECK_NO_THROW( Phases.emplace( EPhaseType::e_MS, std::make_unique< Herd::SSE::MainSequence >( validMetallicity ) ) );

  // Interface
  Herd::SSE::EvolutionState validState = Herd::SSE::UnitTests::GenerateRandomEvolutionState( Rng() );
  Herd::SSE::EvolutionState invalidState;
  for( auto& [ rKey, pPhase ] : Phases )
  {
    BOOST_CHECK_NO_THROW( pPhase->Evolve( validState ) );
    BOOST_CHECK_NO_THROW( pPhase->EndsAt() );

    BOOST_CHECK_THROW( pPhase->Evolve( invalidState ), Herd::Exceptions::PreconditionError );
  }
}

BOOST_AUTO_TEST_SUITE_END( )
