/**
 * @file FormulasUnitTests.cpp
 * @author Evren Imre
 * @date 28 Apr 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <boost/test/unit_test.hpp>

#include <UnitTestUtils/RandomTestFixture.h>

#include <Generic/Quantities.h>
#include <Physics/Constants.h>
#include <Physics/LuminosityRadiusTemperature.h>

BOOST_FIXTURE_TEST_SUITE( Generic, Herd::UnitTestUtils::RandomTestFixture )

BOOST_AUTO_TEST_CASE( LuminosityRadiusTemperatureTest, *boost::unit_test::tolerance( 1e-8 ) )
{
  BOOST_TEST_CONTEXT( "Sun" )
  {
    Herd::Generic::Radius rSun( 1.0 );
    Herd::Generic::Temperature tSun( Herd::Physics::Constants::sunSurfaceTemperature );
    Herd::Generic::Luminosity lSun( 1.0 );

    auto luminosity = Herd::Physics::LuminosityRadiusTemperature::ComputeLuminosty( rSun, tSun );
    BOOST_TEST_REQUIRE( luminosity.has_value() );
    BOOST_TEST( *luminosity == lSun );

    auto radius = Herd::Physics::LuminosityRadiusTemperature::ComputeRadius( lSun, tSun );
    BOOST_TEST_REQUIRE( radius.has_value() );
    BOOST_TEST( *radius == rSun );

    auto temperature = Herd::Physics::LuminosityRadiusTemperature::ComputeTemperature( lSun, rSun );
    BOOST_TEST_REQUIRE( temperature.has_value() );
    BOOST_TEST( *temperature == tSun );
  }

  BOOST_TEST_CONTEXT( "Invalid input" )
  {
    Herd::Generic::Radius rInvalid( GenerateNumber( -1000.0, 0.0 ) );
    Herd::Generic::Temperature tInvalid( GenerateNumber( -1000.0, 0.0 ) );
    Herd::Generic::Luminosity lInvalid( GenerateNumber( -1000.0, 0.0 ) );

    BOOST_TEST( !Herd::Physics::LuminosityRadiusTemperature::ComputeLuminosty( rInvalid, tInvalid ).has_value() );
    BOOST_TEST( !Herd::Physics::LuminosityRadiusTemperature::ComputeRadius( lInvalid, tInvalid ).has_value() );
    BOOST_TEST( !Herd::Physics::LuminosityRadiusTemperature::ComputeTemperature( lInvalid, rInvalid ).has_value() );
  }

  BOOST_TEST_CONTEXT( "Random" )
  {
    Herd::Generic::Radius rExpected( GenerateNumber( 0.1, 12.0 ) );
    Herd::Generic::Temperature tExpected( GenerateNumber( 2200.0, 50000.0 ) );
    Herd::Generic::Luminosity lExpected( GenerateNumber( 0.003, 800000.0 ) );

    {
      auto lComputed = Herd::Physics::LuminosityRadiusTemperature::ComputeLuminosty( rExpected, tExpected );
      BOOST_TEST_REQUIRE( lComputed.has_value() );

      auto rActual = Herd::Physics::LuminosityRadiusTemperature::ComputeRadius( *lComputed, tExpected );
      BOOST_TEST_REQUIRE( rActual.has_value() );
      BOOST_TEST( rActual->Value() == rExpected.Value() ); // @suppress("Invalid arguments") // @suppress("Method cannot be resolved")

      auto tActual = Herd::Physics::LuminosityRadiusTemperature::ComputeTemperature( *lComputed, rExpected );
      BOOST_TEST_REQUIRE( tActual.has_value() );
      BOOST_TEST( tActual->Value() == tExpected.Value() ); // @suppress("Invalid arguments") // @suppress("Method cannot be resolved")
    }

    {
      auto rComputed = Herd::Physics::LuminosityRadiusTemperature::ComputeRadius( lExpected, tExpected );
      BOOST_TEST_REQUIRE( rComputed.has_value() );

      auto lActual = Herd::Physics::LuminosityRadiusTemperature::ComputeLuminosty( *rComputed, tExpected );
      BOOST_TEST_REQUIRE( lActual.has_value() );
      BOOST_TEST( lActual->Value() == lExpected.Value() ); // @suppress("Invalid arguments") // @suppress("Method cannot be resolved")

      auto tActual = Herd::Physics::LuminosityRadiusTemperature::ComputeTemperature( lExpected, *rComputed );
      BOOST_TEST_REQUIRE( tActual.has_value() );
      BOOST_TEST( tActual->Value() == tExpected.Value() ); // @suppress("Invalid arguments") // @suppress("Method cannot be resolved")
    }

    {
      auto tComputed = Herd::Physics::LuminosityRadiusTemperature::ComputeTemperature( lExpected, rExpected );
      BOOST_TEST_REQUIRE( tComputed.has_value() );

      auto lActual = Herd::Physics::LuminosityRadiusTemperature::ComputeLuminosty( rExpected, *tComputed );
      BOOST_TEST_REQUIRE( lActual.has_value() );
      BOOST_TEST( lActual->Value() == lExpected.Value() ); // @suppress("Invalid arguments") // @suppress("Method cannot be resolved")

      auto rActual = Herd::Physics::LuminosityRadiusTemperature::ComputeRadius( lExpected, *tComputed );
      BOOST_TEST_REQUIRE( rActual.has_value() );
      BOOST_TEST( rActual->Value() == rExpected.Value() ); // @suppress("Invalid arguments") // @suppress("Method cannot be resolved")
    }
  }

}

BOOST_AUTO_TEST_SUITE_END( )
