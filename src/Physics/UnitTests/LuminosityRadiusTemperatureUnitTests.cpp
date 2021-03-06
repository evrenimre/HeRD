/**
 * @file LuminosityRadiusTemperatureUnitTests.cpp
 * @author Evren Imre
 * @date 28 Apr 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <UnitTestUtils/RandomTestFixture.h>
#include <UnitTestUtils/UnitTestUtilityFunctions.h>

#include <Exceptions/PreconditionError.h>
#include <Generic/Quantity.h>
#include <Physics/Constants.h>
#include <Physics/LuminosityRadiusTemperature.h>

namespace
{
/**
 * @brief Unit test fixture for \c LuminosityRadiusTemperature
 */
class LRTTestFixture : public Herd::UnitTestUtils::RandomTestFixture
{
public:

  void TestConsistency(); ///< Tests whether one of L, R and T can be computed correctly when the other two are known
};

/**
 * @remarks Equality operator for Quantity does not trigger a floating point comparison. So, need to call Quantity::Value
 */
void LRTTestFixture::TestConsistency()
{
  Herd::Generic::Radius rExpected( GenerateNumber( 0., 12. ) ); // @suppress("Invalid arguments")

  double lowerT = 2200. / Herd::Physics::Constants::s_SunSurfaceTemperature;
  double upperT = 50000. / Herd::Physics::Constants::s_SunSurfaceTemperature;
  Herd::Generic::Temperature tExpected( GenerateNumber( lowerT, upperT ) ); // @suppress("Invalid arguments")
  Herd::Generic::Luminosity lExpected( GenerateNumber( 0.003, 800000. ) ); // @suppress("Invalid arguments")

  BOOST_TEST_CONTEXT( "Luminosity "<< lExpected.Value() <<" Radius "<< rExpected.Value()<<" Temperature "<<tExpected.Value() )
  {
    {
      auto lComputed = Herd::Physics::LuminosityRadiusTemperature::ComputeLuminosty( rExpected, tExpected );

      auto rActual = Herd::Physics::LuminosityRadiusTemperature::ComputeRadius( lComputed, tExpected );
      BOOST_TEST( rActual.Value() == rExpected.Value() ); // @suppress("Invalid arguments") // @suppress("Method cannot be resolved")

      auto tActual = Herd::Physics::LuminosityRadiusTemperature::ComputeTemperature( lComputed, rExpected );
      BOOST_TEST( tActual.Value() == tExpected.Value() ); // @suppress("Invalid arguments") // @suppress("Method cannot be resolved")
    }

    {
      auto rComputed = Herd::Physics::LuminosityRadiusTemperature::ComputeRadius( lExpected, tExpected );

      auto lActual = Herd::Physics::LuminosityRadiusTemperature::ComputeLuminosty( rComputed, tExpected );
      BOOST_TEST( lActual.Value() == lExpected.Value() ); // @suppress("Invalid arguments") // @suppress("Method cannot be resolved")

      auto tActual = Herd::Physics::LuminosityRadiusTemperature::ComputeTemperature( lExpected, rComputed );
      BOOST_TEST( tActual.Value() == tExpected.Value() ); // @suppress("Invalid arguments") // @suppress("Method cannot be resolved")
    }

    {
      auto tComputed = Herd::Physics::LuminosityRadiusTemperature::ComputeTemperature( lExpected, rExpected );

      auto lActual = Herd::Physics::LuminosityRadiusTemperature::ComputeLuminosty( rExpected, tComputed );
      BOOST_TEST( lActual.Value() == lExpected.Value() ); // @suppress("Invalid arguments") // @suppress("Method cannot be resolved")

      auto rActual = Herd::Physics::LuminosityRadiusTemperature::ComputeRadius( lExpected, tComputed );
      BOOST_TEST( rActual.Value() == rExpected.Value() ); // @suppress("Invalid arguments") // @suppress("Method cannot be resolved")
    }
  }
}

}

BOOST_FIXTURE_TEST_SUITE( Physics, LRTTestFixture )

/// LRT unit tests
BOOST_AUTO_TEST_CASE( LuminosityRadiusTemperatureTests, *boost::unit_test::tolerance( 1e-8 ) *Herd::UnitTestUtils::Labels::s_Compile )
{
  BOOST_TEST_CONTEXT( "Sun" )
  {
    Herd::Generic::Radius rSun( 1. );
    Herd::Generic::Temperature tSun( 1. );
    Herd::Generic::Luminosity lSun( 1. );

    auto luminosity = Herd::Physics::LuminosityRadiusTemperature::ComputeLuminosty( rSun, tSun );
    BOOST_TEST( luminosity == lSun );

    auto radius = Herd::Physics::LuminosityRadiusTemperature::ComputeRadius( lSun, tSun );
    BOOST_TEST( radius == rSun );

    auto temperature = Herd::Physics::LuminosityRadiusTemperature::ComputeTemperature( lSun, rSun );
    BOOST_TEST( temperature == tSun );
  }

  BOOST_TEST_CONTEXT( "Invalid input" )
  {
    Herd::Generic::Radius rInvalid( GenerateNumber( -1000.0, 0.0 ) ); // @suppress("Invalid arguments")
    Herd::Generic::Temperature tInvalid( GenerateNumber( -1000.0, 0.0 ) ); // @suppress("Invalid arguments")
    Herd::Generic::Luminosity lInvalid( GenerateNumber( -1000.0, 0.0 ) ); // @suppress("Invalid arguments")

    BOOST_CHECK_THROW( Herd::Physics::LuminosityRadiusTemperature::ComputeLuminosty( rInvalid, tInvalid ), Herd::Exceptions::PreconditionError );
    BOOST_CHECK_THROW( Herd::Physics::LuminosityRadiusTemperature::ComputeRadius( lInvalid, tInvalid ), Herd::Exceptions::PreconditionError );
    BOOST_CHECK_THROW( Herd::Physics::LuminosityRadiusTemperature::ComputeTemperature( lInvalid, rInvalid ), Herd::Exceptions::PreconditionError );
  }

  BOOST_TEST_CONTEXT( "Random" )
  {
    TestConsistency();
  }
}

BOOST_AUTO_TEST_CASE( TestHelpers, *boost::unit_test::tolerance( 1e-8 ) *Herd::UnitTestUtils::Labels::s_Compile )
{
  Herd::Generic::Radius radius( GenerateNumber( 0., 12. ) ); // @suppress("Invalid arguments")
  Herd::Generic::Luminosity luminosity( GenerateNumber( 0.003, 800000. ) ); // @suppress("Invalid arguments")

  Herd::Generic::Temperature temperature = Herd::Physics::LuminosityRadiusTemperature::ComputeTemperature( luminosity, radius );
  Herd::Generic::Temperature temperatureInK = Herd::Physics::ComputeAbsoluteTemperature( luminosity, radius );

  BOOST_TEST( (temperatureInK / temperature).Value() == Herd::Physics::Constants::s_SunSurfaceTemperature ); // @suppress("Invalid arguments") // @suppress("Method cannot be resolved")
}

/// LRT domain exploration test
BOOST_AUTO_TEST_CASE( LRTDomainExploration, *boost::unit_test::tolerance( 1e-8 ) *Herd::UnitTestUtils::Labels::s_Nightly )
{
  for( int c = 0; c < 100; ++c )
  {
    BOOST_TEST_CONTEXT( "Run " << c)
    {
      TestConsistency();
    }
  }
}

BOOST_AUTO_TEST_SUITE_END( )

