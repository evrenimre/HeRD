/**
 * @file ZeroAgeMainSequenceUnitTests.cpp
 * @author Evren Imre
 * @date 29 Apr 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <boost/test/unit_test.hpp>

#include <UnitTestUtils/RandomTestFixture.h>

#include <Generic/Quantities.h>
#include <SSE/StarState.h>
#include <SSE/ZeroAgeMainSequence.h>

BOOST_FIXTURE_TEST_SUITE( ZAMS, Herd::UnitTestUtils::RandomTestFixture )

BOOST_AUTO_TEST_CASE( ZerAgeMainSequenceTest, *boost::unit_test::tolerance( 1e-8 ) )
{
  Herd::Generic::Metallicity z( GenerateReal( 1e-4, 0.03 ) );
  Herd::Generic::Mass m( GenerateReal( 0.1, 100.0 ) );

  auto starState = Herd::SSE::ZeroAgeMainSequence::ComputeStarState( m, z );

  BOOST_TEST_REQUIRE( starState.has_value() );
  BOOST_TEST( starState->m_Age == 0 );
  BOOST_TEST( starState->m_Lumonosity > 0.0 );
  BOOST_TEST( starState->m_Mass == m );
  BOOST_TEST( starState->m_Radius > 0 );
  BOOST_TEST( starState->m_Temperature > 0 );
  BOOST_TEST( starState->m_Z == z );
}

BOOST_AUTO_TEST_CASE( InvalidParameters )
{
  {
    Herd::Generic::Metallicity z( GenerateBool() ? GenerateReal( 0.0, 1e-4 ) : GenerateReal( 0.04, 1.0 ) );
    Herd::Generic::Mass m( GenerateReal( 0.1, 100.0 ) );
    BOOST_TEST( !Herd::SSE::ZeroAgeMainSequence::ComputeStarState( m, z ) );
  }

  {
    Herd::Generic::Metallicity z( GenerateReal( 1e-4, 0.03 ) );
    Herd::Generic::Mass m( GenerateBool() ? GenerateReal( 0.0, 0.1 ) : GenerateReal( 100.0, 500.0 ) );
    BOOST_TEST( !Herd::SSE::ZeroAgeMainSequence::ComputeStarState( m, z ) );
  }

}
BOOST_AUTO_TEST_SUITE_END( )


