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
BOOST_AUTO_TEST_CASE( validatorTests, *Herd::UnitTestUtils::Labels::s_Compile )
{
  Herd::SSE::TrackPoint valid = Herd::SSE::UnitTests::GenerateRandomTrackPoint( Rng() );
  BOOST_CHECK_NO_THROW( Herd::SSE::ValidateTrackPoint( valid ) );

  BOOST_CHECK_THROW( Herd::SSE::ValidateTrackPoint( Herd::SSE::TrackPoint() ), Herd::Exceptions::PreconditionError );

  // @formatter:off
  auto Invalidate = [ & ]( auto& i_rValue ){ i_rValue.Set( -i_rValue - 0.1 );};
    // @formatter:on

  {
    Herd::SSE::TrackPoint invalid = valid;
    Invalidate( invalid.m_Mass );
    BOOST_CHECK_THROW( Herd::SSE::ValidateTrackPoint( invalid ), Herd::Exceptions::PreconditionError );
  }

  {
    Herd::SSE::TrackPoint invalid = valid;
    Invalidate( invalid.m_InitialMetallicity );
    BOOST_CHECK_THROW( Herd::SSE::ValidateTrackPoint( invalid ), Herd::Exceptions::PreconditionError );
  }

  {
    Herd::SSE::TrackPoint invalid = valid;
    Invalidate( invalid.m_Radius );
    BOOST_CHECK_THROW( Herd::SSE::ValidateTrackPoint( invalid ), Herd::Exceptions::PreconditionError );
  }

  {
    Herd::SSE::TrackPoint invalid = valid;
    Invalidate( invalid.m_Luminosity );
    BOOST_CHECK_THROW( Herd::SSE::ValidateTrackPoint( invalid ), Herd::Exceptions::PreconditionError );
  }

  {
    Herd::SSE::TrackPoint invalid = valid;
    Invalidate( invalid.m_Temperature );
    BOOST_CHECK_THROW( Herd::SSE::ValidateTrackPoint( invalid ), Herd::Exceptions::PreconditionError );
  }

  {
    Herd::SSE::TrackPoint invalid = valid;
    Invalidate( invalid.m_Age );
    BOOST_CHECK_THROW( Herd::SSE::ValidateTrackPoint( invalid ), Herd::Exceptions::PreconditionError );
  }

  {
    Herd::SSE::TrackPoint invalid = valid;
    invalid.m_Stage = Herd::SSE::EvolutionStage::e_Undefined;
    BOOST_CHECK_THROW( Herd::SSE::ValidateTrackPoint( invalid ), Herd::Exceptions::PreconditionError );
  }

  {
    Herd::SSE::TrackPoint invalid = valid;
    Invalidate( invalid.m_CoreMass );
    BOOST_CHECK_THROW( Herd::SSE::ValidateTrackPoint( invalid ), Herd::Exceptions::PreconditionError );

    invalid.m_CoreMass.Set( valid.m_Mass + 0.1 );
    BOOST_CHECK_THROW( Herd::SSE::ValidateTrackPoint( invalid ), Herd::Exceptions::PreconditionError );
  }

  {
    Herd::SSE::TrackPoint invalid = valid;
    Invalidate( invalid.m_EnvelopeMass );
    BOOST_CHECK_THROW( Herd::SSE::ValidateTrackPoint( invalid ), Herd::Exceptions::PreconditionError );

    invalid.m_EnvelopeMass.Set( valid.m_Mass + 0.1 );
    BOOST_CHECK_THROW( Herd::SSE::ValidateTrackPoint( invalid ), Herd::Exceptions::PreconditionError );
  }

  {
    Herd::SSE::TrackPoint invalid = valid;
    Invalidate( invalid.m_AngularMomentum );
    BOOST_CHECK_THROW( Herd::SSE::ValidateTrackPoint( invalid ), Herd::Exceptions::PreconditionError );
  }
}

BOOST_AUTO_TEST_SUITE_END( )
