/**
 * @file ConvectiveEnvelopeUnitTests.cpp
 * @author Evren Imre
 * @date 31 Aug 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include "SSETestUtils.h"

#include <Exceptions/PreconditionError.h>
#include <SSE/ConvectiveEnvelope.h>
#include <SSE/EvolutionStage.h>
#include <UnitTestUtils/RandomTestFixture.h>
#include <UnitTestUtils/UnitTestUtilityFunctions.h>

BOOST_FIXTURE_TEST_SUITE( ConvectiveEnvelopeTests, Herd::UnitTestUtils::RandomTestFixture )

// Operation is not tested, as this would require many intermediate parameter values not available in the test data
// Envelope mass and radius is used in single star evolution computations
// Radius of gyration is used in binary star evolution computations

/// Valid and invalid input
BOOST_AUTO_TEST_CASE( InputTest, *Herd::UnitTestUtils::Labels::s_Compile )
{
  Herd::SSE::EvolutionState valid = Herd::SSE::UnitTests::GenerateRandomEvolutionState( Rng() );

  Herd::SSE::ConvectiveEnvelope envelopeComputer;
  Herd::SSE::ConvectiveEnvelope::Envelope Output = envelopeComputer.Compute( valid );

  if( Herd::SSE::IsRemnant( valid.m_TrackPoint.m_Stage ) )
  {
    BOOST_TEST( Output.m_Mass == 0. );
    BOOST_TEST( Output.m_Radius == 0. );
    BOOST_TEST( Output.m_RadiusOfGyration == 0. );
  }

  BOOST_CHECK_THROW( envelopeComputer.Compute( Herd::SSE::EvolutionState() ), Herd::Exceptions::PreconditionError );

}

BOOST_AUTO_TEST_SUITE_END( )
