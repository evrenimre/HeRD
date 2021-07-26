/**
 * @file StellarRotationUnitTests.c[[
 * @author Evren Imre
 * @date 25 Jul 2021	
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
#include <SSE/EvolutionStage.h>
#include <SSE/StellarRotation.h>
#include <UnitTestUtils/RandomTestFixture.h>
#include <UnitTestUtils/UnitTestUtilityFunctions.h>

// Tests do not include a comparison to reference data
// Test data does not include Rc, so momentum loss can only be computed for MS and HeMS stars
// It is possible to compute the initial rotation on BH and NS, but would require seeking to the first track point of the phase. A bit onerous.
// All functions in the interface are ultimately covered by unit tests for the entire evolution procedure

BOOST_FIXTURE_TEST_SUITE( StellarRotationTests, Herd::UnitTestUtils::RandomTestFixture )

/// Tests for invalid cases
BOOST_AUTO_TEST_CASE( InvalidParametersTest, *Herd::UnitTestUtils::Labels::s_Compile )
{

  Herd::SSE::EvolutionState valid = Herd::SSE::UnitTests::GenerateRandomEvolutionState( Rng() );

  {
    BOOST_CHECK_NO_THROW( Herd::SSE::StellarRotation::ComputeAngularMomentumLossRate( valid ) );
    BOOST_CHECK_THROW( Herd::SSE::StellarRotation::ComputeAngularMomentumLossRate( Herd::SSE::EvolutionState() ), Herd::Exceptions::PreconditionError );
  }

  {
    Herd::SSE::EvolutionState zams = valid;
    if( Herd::SSE::IsRemnant( zams.m_TrackPoint.m_Stage ) )
    {
      zams.m_TrackPoint.m_Stage = GenerateBool() ? Herd::SSE::EvolutionStage::e_MS : Herd::SSE::EvolutionStage::e_MSLM;
    }
    BOOST_CHECK_NO_THROW( Herd::SSE::StellarRotation::InitialiseAtZAMS( zams ) );

    Herd::SSE::EvolutionState notZeroAge = valid;
    BOOST_CHECK_THROW( Herd::SSE::StellarRotation::InitialiseAtZAMS( notZeroAge ), Herd::Exceptions::PreconditionError );
  }

  {
    Herd::SSE::EvolutionState remnant = valid;
    remnant.m_TrackPoint.m_Stage = GenerateBool() ? Herd::SSE::EvolutionStage::e_BH : Herd::SSE::EvolutionStage::e_NS;
    BOOST_CHECK_NO_THROW( Herd::SSE::StellarRotation::InitialiseAtNSOrBH( remnant ) );

    Herd::SSE::EvolutionState notBHorNS = valid;
    if( notBHorNS.m_TrackPoint.m_Stage != Herd::SSE::EvolutionStage::e_BH || notBHorNS.m_TrackPoint.m_Stage != Herd::SSE::EvolutionStage::e_NS )
    {
      notBHorNS.m_TrackPoint.m_Stage = Herd::SSE::EvolutionStage::e_MS;
    }

    BOOST_CHECK_THROW( Herd::SSE::StellarRotation::InitialiseAtNSOrBH( notBHorNS ), Herd::Exceptions::PreconditionError );
  }
}

BOOST_AUTO_TEST_SUITE_END( )
