/**
 * @file TrackPointUnitTests.cpp
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
#include <SSE/TrackPoint.h>

#include <Exceptions/PreconditionError.h>
#include <UnitTestUtils/RandomTestFixture.h>
#include <UnitTestUtils/UnitTestUtilityFunctions.h>

BOOST_FIXTURE_TEST_SUITE( TrackPointUnitTests, Herd::UnitTestUtils::RandomTestFixture )

/// Tests for the validator function
BOOST_AUTO_TEST_CASE( ValidatorTests, *Herd::UnitTestUtils::Labels::s_Compile )
{
  Herd::SSE::TrackPoint Valid = Herd::SSE::UnitTests::GenerateRandomTrackPoint( Rng() );
  BOOST_CHECK_NO_THROW( Herd::SSE::ValidateTrackPoint( Valid ) );

  BOOST_CHECK_THROW( Herd::SSE::ValidateTrackPoint( Herd::SSE::TrackPoint() ), Herd::Exceptions::PreconditionError );

  // @formatter:off
  auto Invalidate = [ & ]( auto& i_rValue ){ i_rValue.Set( -i_rValue - 0.1 );};
    // @formatter:on

  {
    Herd::SSE::TrackPoint Invalid = Valid;
    Invalidate( Invalid.m_Mass );
    BOOST_CHECK_THROW( Herd::SSE::ValidateTrackPoint( Invalid ), Herd::Exceptions::PreconditionError );
  }

  {
    Herd::SSE::TrackPoint Invalid = Valid;
    Invalidate( Invalid.m_InitialMetallicity );
    BOOST_CHECK_THROW( Herd::SSE::ValidateTrackPoint( Invalid ), Herd::Exceptions::PreconditionError );
  }

  {
    Herd::SSE::TrackPoint Invalid = Valid;
    Invalidate( Invalid.m_Radius );
    BOOST_CHECK_THROW( Herd::SSE::ValidateTrackPoint( Invalid ), Herd::Exceptions::PreconditionError );
  }

  {
    Herd::SSE::TrackPoint Invalid = Valid;
    Invalidate( Invalid.m_Luminosity );
    BOOST_CHECK_THROW( Herd::SSE::ValidateTrackPoint( Invalid ), Herd::Exceptions::PreconditionError );
  }

  {
    Herd::SSE::TrackPoint Invalid = Valid;
    Invalidate( Invalid.m_Temperature );
    BOOST_CHECK_THROW( Herd::SSE::ValidateTrackPoint( Invalid ), Herd::Exceptions::PreconditionError );
  }

  {
    Herd::SSE::TrackPoint Invalid = Valid;
    Invalidate( Invalid.m_Age );
    BOOST_CHECK_THROW( Herd::SSE::ValidateTrackPoint( Invalid ), Herd::Exceptions::PreconditionError );
  }

  {
    Herd::SSE::TrackPoint Invalid = Valid;
    Invalid.m_Stage = Herd::SSE::EvolutionStage::e_Undefined;
    BOOST_CHECK_THROW( Herd::SSE::ValidateTrackPoint( Invalid ), Herd::Exceptions::PreconditionError );
  }

  {
    Herd::SSE::TrackPoint Invalid = Valid;
    Invalidate( Invalid.m_CoreMass );
    BOOST_CHECK_THROW( Herd::SSE::ValidateTrackPoint( Invalid ), Herd::Exceptions::PreconditionError );

    Invalid.m_CoreMass.Set( Valid.m_Mass + 0.1 );
    BOOST_CHECK_THROW( Herd::SSE::ValidateTrackPoint( Invalid ), Herd::Exceptions::PreconditionError );
  }

  {
    Herd::SSE::TrackPoint Invalid = Valid;
    Invalidate( Invalid.m_EnvelopeMass );
    BOOST_CHECK_THROW( Herd::SSE::ValidateTrackPoint( Invalid ), Herd::Exceptions::PreconditionError );

    Invalid.m_EnvelopeMass.Set( Valid.m_Mass + 0.1 );
    BOOST_CHECK_THROW( Herd::SSE::ValidateTrackPoint( Invalid ), Herd::Exceptions::PreconditionError );
  }

  {
    Herd::SSE::TrackPoint Invalid = Valid;
    Invalidate( Invalid.m_AngularMomentum );
    BOOST_CHECK_THROW( Herd::SSE::ValidateTrackPoint( Invalid ), Herd::Exceptions::PreconditionError );
  }
}

BOOST_AUTO_TEST_SUITE_END( )
