/**
 * @file EvolutionStateUnitTests.cpp
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

#include <SSE/EvolutionStage.h>
#include <SSE/EvolutionState.h>
#include <SSE/TrackPoint.h>

#include <Exceptions/PreconditionError.h>
#include <UnitTestUtils/RandomTestFixture.h>
#include <UnitTestUtils/UnitTestUtilityFunctions.h>

BOOST_FIXTURE_TEST_SUITE( EvolutionStateUnitTests, Herd::UnitTestUtils::RandomTestFixture )

/// Tests for the validator function
BOOST_AUTO_TEST_CASE( validatorTests, *Herd::UnitTestUtils::Labels::s_Compile )
{
  Herd::SSE::EvolutionState valid = Herd::SSE::UnitTests::GenerateRandomEvolutionState( Rng() );
  BOOST_CHECK_NO_THROW( Herd::SSE::ValidateEvolutionState( valid ) );

  BOOST_CHECK_THROW( Herd::SSE::ValidateEvolutionState( Herd::SSE::EvolutionState() ), Herd::Exceptions::PreconditionError );

  // @formatter:off
  auto InvalidateQuantity = [ & ]( auto& i_rValue ){ i_rValue.Set( -i_rValue - 0.1 );};
          // @formatter:on

  {
    Herd::SSE::EvolutionState invalid = valid;
    invalid.m_TrackPoint = Herd::SSE::TrackPoint();
    BOOST_CHECK_THROW( Herd::SSE::ValidateEvolutionState( invalid ), Herd::Exceptions::PreconditionError );
  }

  {
    Herd::SSE::EvolutionState invalid = valid;
    InvalidateQuantity( invalid.m_CoreRadius );
    BOOST_CHECK_THROW( Herd::SSE::ValidateEvolutionState( invalid ), Herd::Exceptions::PreconditionError );

    invalid.m_CoreRadius.Set( valid.m_TrackPoint.m_Radius + 0.1 );
    BOOST_CHECK_THROW( Herd::SSE::ValidateEvolutionState( invalid ), Herd::Exceptions::PreconditionError );
  }

  {
    Herd::SSE::EvolutionState invalid = valid;
    InvalidateQuantity( invalid.m_InitialMass );
    BOOST_CHECK_THROW( Herd::SSE::ValidateEvolutionState( invalid ), Herd::Exceptions::PreconditionError );
  }

  {
    Herd::SSE::EvolutionState invalid = valid;
    InvalidateQuantity( invalid.m_MassLossRate );
    BOOST_CHECK_THROW( Herd::SSE::ValidateEvolutionState( invalid ), Herd::Exceptions::PreconditionError );
  }

  {
    Herd::SSE::EvolutionState invalid = valid;
    InvalidateQuantity( invalid.m_AngularMomentumLossRate );
    BOOST_CHECK_THROW( Herd::SSE::ValidateEvolutionState( invalid ), Herd::Exceptions::PreconditionError );
  }
}

BOOST_AUTO_TEST_SUITE_END( )

