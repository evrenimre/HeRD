/**
 * @file StellarWindMassLossUnitTests.cpp
 * @author Evren Imre
 * @date 29 Jun 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <boost/test/unit_test.hpp>

#include "SSETestDataManager.h"

#include <Exceptions/PreconditionError.h>
#include <SSE/EvolutionStage.h>
#include <SSE/StellarWindMassLoss.h>
#include <SSE/TrackPoint.h>

#include <UnitTestUtils/DataLoaderFixture.h>
#include <UnitTestUtils/RandomTestFixture.h>
#include <UnitTestUtils/UnitTestUtilityFunctions.h>

#include <string>
#include <unordered_map>
#include <vector>

#include <boost/property_tree/ptree.hpp>

namespace
{
/**
 * @brief Unit test fixture for StellarWindMassLoss
 */
class StellarWindMassLossTestFixture : public Herd::UnitTestUtils::RandomTestFixture, public Herd::UnitTestUtils::DataLoaderFixture
{
public:

  static std::vector< Herd::SSE::TrackPoint > MakeTestCase( const boost::property_tree::ptree& i_rRoot ); ///< Makes a test case from a property tree
  std::vector< Herd::SSE::TrackPoint > MakeTestCase(); ///< Makes a test case from a random file

  static void TestFidelity( const std::vector< Herd::SSE::TrackPoint >& i_rTrack, std::size_t i_Start = 0, double i_SampleSize = 1. ); ///< Compares the computed and the actual mass loss

  static const std::string& TrackRegex(); ///< Returns a constant reference to StellarWindMassLossTestFixture::s_TrackRegex

private:

  inline static std::string s_ParentTag = "Track"; ///< Parent tag for the track points
  inline static std::string s_TrackRegex = "track\\.xml"; ///< Regex for track files
};

/**
 * @param i_rRoot Tree root
 * @return Test case
 */
std::vector< Herd::SSE::TrackPoint > StellarWindMassLossTestFixture::MakeTestCase( const boost::property_tree::ptree& i_rRoot )
{
  // Initialise the data manager
  Herd::SSE::UnitTests::SSETestDataManager dataManager;
  dataManager.SetData( i_rRoot.get_child( s_ParentTag ) );
  dataManager.CheckVersionInfo();

  // Track points
  Herd::SSE::TrackPoint zams = dataManager.MakeTrackPoint( 0 );
  dataManager.PopulateTrackPoints( zams.m_InitialMetallicity );

  return dataManager.TrackPoints();
}

/**
 * @param i_rTrack An evolutionary track
 * @param i_Start First track point to be tested
 * @param i_SampleSize Sample size as a percentage of the track length. (0,1]
 * @remarks The test compares the expected and the actual mass rather than mass loss rates. SSE outputs are floats. Comparing difference of two floats to a double cause very high relative errors when the quantities in question are small
 */
void StellarWindMassLossTestFixture::TestFidelity( const std::vector< Herd::SSE::TrackPoint >& i_rTrack, std::size_t i_Start, double i_SampleSize )
{
  // Default parameters
  double neta = 0.5;
  double heWind = 1.;
  double binaryWind = 0.;
  double rocheLobe = 0.;

  std::size_t length = i_rTrack.size() - 1;
  BOOST_TEST( length >= 2 );
  BOOST_TEST( i_Start < length );

  BOOST_TEST_REQUIRE( ( i_SampleSize > 0 && i_SampleSize <= 1 ) );

  std::size_t stride = static_cast< std::size_t >( std::max( 1., 1.0 / i_SampleSize ) );

  for( std::size_t idx = i_Start; idx < length; idx += stride )
  {
    BOOST_TEST_CONTEXT( "Track point "<< idx )
    {
      const auto& rCurrent = i_rTrack[ idx ];
      const auto& rNext = i_rTrack[ idx + 1 ];

      double massLossRate = Herd::SSE::StellarWindMassLoss::Compute( rCurrent, neta, heWind, binaryWind, rocheLobe ); // Per year
      double deltaAge = ( rNext.m_Age.Value() - rCurrent.m_Age.Value() );  // In Myears

      // Reference data is float. For rapidly evolving stars and densely sampled stages, time elapsed between two consecutive track points can be erroneous
      if( deltaAge < 1e-2 )
      {
        continue;
      }

      // Transition to remnant state involves sudden mass loss that is not explained by the stellar wind formulae
      if( !Herd::SSE::IsRemnant( rCurrent.m_Stage ) && Herd::SSE::IsRemnant( rNext.m_Stage ) )
      {
        continue;
      }

      double actual = rCurrent.m_Mass.Value() - massLossRate * deltaAge * 1e6;

      BOOST_TEST( actual == rNext.m_Mass.Value(), boost::test_tools::tolerance( 3e-4 ) );
    }
  }
}

std::vector< Herd::SSE::TrackPoint > StellarWindMassLossTestFixture::MakeTestCase()
{
  // Pick a random file
  std::size_t fileCount = GetFileCount( s_TrackRegex );
  BOOST_TEST_REQUIRE( fileCount != 0 );
  boost::property_tree::ptree data = ReadAsXML( s_TrackRegex, GenerateNumber( static_cast< std::size_t >( 0 ), fileCount - 1 ) );

  std::vector< Herd::SSE::TrackPoint > track = MakeTestCase( data );
  BOOST_TEST_REQUIRE( track.size() >= 2 );

  return track;
}

/**
 * @return A constant reference to StellarWindMassLossTestFixture::s_TrackRegex
 */
const std::string& StellarWindMassLossTestFixture::TrackRegex()
{
  return s_TrackRegex;
}

}

BOOST_FIXTURE_TEST_SUITE( StellarWindMassLossTests, StellarWindMassLossTestFixture )

/// Tests for invalid cases
BOOST_AUTO_TEST_CASE( InvalidParameters, *Herd::UnitTestUtils::Labels::s_Compile )
{
  Herd::SSE::TrackPoint validPoint;
  validPoint.m_Age.Set( 4401 );
  validPoint.m_AngularMomentum.Set( 566.2 );
  validPoint.m_CoreMass.Set( 0. );
  validPoint.m_EnvelopeMass.Set( 3.341e-2 );
  validPoint.m_InitialMetallicity.Set( 0.02 );
  validPoint.m_Luminosity.Set( 0.942 );
  validPoint.m_Mass.Set( 1.0 );
  validPoint.m_Radius.Set( 0.979 );
  validPoint.m_Temperature.Set( 5754.399 );
  validPoint.m_Stage = Herd::SSE::EvolutionStage::e_MS;

  double neta = GenerateNumber( 0., 1. );
  double heWind = GenerateNumber( 0., 2. );
  double binaryWind = GenerateNumber( 0., 2. );
  double rocheLobe = GenerateNumber( 0., 2. );

  // Invalid parameters
  {
    BOOST_CHECK_THROW( Herd::SSE::StellarWindMassLoss::Compute( validPoint, GenerateNumber( -1., -0.1 ) ), Herd::Exceptions::PreconditionError );
    BOOST_CHECK_THROW( Herd::SSE::StellarWindMassLoss::Compute( validPoint, GenerateNumber( -1., -0.1 ), heWind, binaryWind, rocheLobe ),
        Herd::Exceptions::PreconditionError );

    BOOST_CHECK_THROW( Herd::SSE::StellarWindMassLoss::Compute( validPoint, neta, GenerateNumber( -1., -0.1 ), binaryWind, rocheLobe ),
        Herd::Exceptions::PreconditionError );

    BOOST_CHECK_THROW( Herd::SSE::StellarWindMassLoss::Compute( validPoint, neta, heWind, GenerateNumber( -1., -0.1 ), rocheLobe ),
        Herd::Exceptions::PreconditionError );

    BOOST_CHECK_THROW( Herd::SSE::StellarWindMassLoss::Compute( validPoint, neta, heWind, binaryWind, GenerateNumber( -1., -0.1 ) ),
        Herd::Exceptions::PreconditionError );
  }

  // Check invalid track points
  //@formatter:off
  auto Caller = [ & ]( const auto& i_rPoint ){ Herd::SSE::StellarWindMassLoss::Compute( i_rPoint, neta, heWind, binaryWind, rocheLobe);};
                //@formatter:on

  {
    Herd::SSE::TrackPoint invalid = validPoint;
    invalid.m_Mass.Set( GenerateBool() ? 0. : GenerateNumber( -1., 0. ) );
    BOOST_CHECK_THROW( Caller( invalid ), Herd::Exceptions::PreconditionError );
  }

  {
    Herd::SSE::TrackPoint invalid = validPoint;
    invalid.m_Luminosity.Set( GenerateNumber( -1., -0.1 ) );
    BOOST_CHECK_THROW( Caller( invalid ), Herd::Exceptions::PreconditionError );
  }

  {
    Herd::SSE::TrackPoint invalid = validPoint;
    invalid.m_Radius.Set( GenerateNumber( -1., -0.1 ) );
    BOOST_CHECK_THROW( Caller( invalid ), Herd::Exceptions::PreconditionError );
  }

  {
    Herd::SSE::TrackPoint invalid = validPoint;
    invalid.m_InitialMetallicity.Set( GenerateNumber( -1., -0.1 ) );
    BOOST_CHECK_THROW( Caller( invalid ), Herd::Exceptions::PreconditionError );
  }

  {
    Herd::SSE::TrackPoint invalid = validPoint;
    invalid.m_CoreMass.Set( GenerateNumber( -1., -0.1 ) );
    BOOST_CHECK_THROW( Caller( invalid ), Herd::Exceptions::PreconditionError );
  }

  {
    Herd::SSE::TrackPoint invalid = validPoint;
    invalid.m_Stage = Herd::SSE::EvolutionStage::e_Undefined;
    BOOST_CHECK_THROW( Caller( invalid ), Herd::Exceptions::PreconditionError );
  }
}

/// Test on random points a single track
BOOST_AUTO_TEST_CASE( RandomReferenceTrackSampled, *Herd::UnitTestUtils::Labels::s_Compile )
{
  std::vector< Herd::SSE::TrackPoint > track = MakeTestCase();
  TestFidelity( track, GenerateNumber( static_cast< std::size_t >( 0 ), track.size() - 2 ), 0.02 );
}

/*
/// Test on a random track
BOOST_AUTO_TEST_CASE( RandomReferenceTrack, *Herd::UnitTestUtils::Labels::s_Continuous)
{
  std::vector< Herd::SSE::TrackPoint > track = MakeTestCase();
  TestFidelity( MakeTestCase(), GenerateNumber( static_cast< std::size_t >( 0 ), track.size() - 2 ) );
}
 */

/// Test over the entire dataset
BOOST_AUTO_TEST_CASE( AllReferenceTracks, *Herd::UnitTestUtils::Labels::s_Continuous )
{
  // Read all files
  std::unordered_map< std::string, boost::property_tree::ptree > data = ReadAsXML( TrackRegex() );
  BOOST_TEST_REQUIRE( !data.empty() );

  for( const auto& current : data )
  {
    BOOST_TEST_CONTEXT( current.first )
    {
      TestFidelity( MakeTestCase( current.second ) );
    }
  }
}

BOOST_AUTO_TEST_SUITE_END( )

