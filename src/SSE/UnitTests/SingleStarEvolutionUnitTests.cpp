/**
 * @file SingleStarEvolutionUnitTests.cpp
 * @author evren
 * @date 14 May 2023	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2023 evren 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include "SSETestDataManager.h"

#include <UnitTestUtils/DataLoaderFixture.h>
#include <UnitTestUtils/RandomTestFixture.h>
#include <UnitTestUtils/UnitTestUtilityFunctions.h>

#include <SSE/Constants.h>
#include <SSE/EvolutionStage.h>
#include <SSE/SingleStarEvolution.h>
#include <SSE/TrackPoint.h>

#include <Physics/Constants.h>

#include <cmath>
#include <cstddef>
#include <tuple>

#include <boost/container/flat_set.hpp>

namespace
{

/**
 * @brief Unit test fixture for SingleStarEvolution
 */
class SSETestFixture : public Herd::UnitTestUtils::RandomTestFixture, public Herd::UnitTestUtils::DataLoaderFixture, public Herd::SSE::SingleStarEvolutuionSpecs
{
public:

  std::vector< Herd::SSE::TrackPoint > MakeTestCase( std::size_t i_FileIndex ); ///< Makes a test case from a file

  void TestFidelity( const std::vector< Herd::SSE::TrackPoint >& i_rTrack ); ///< Compares the computed and the actual evolution tracks

  inline static std::string s_TrackRegex = "track\\.xml"; ///< Regex for track files

private:

  static void TestTrackPoint( const Herd::SSE::TrackPoint& i_rActual, const Herd::SSE::TrackPoint& i_rExpected ); ///< Tests whether the difference is within permissible bounds

  inline static std::string s_ParentTag = "Track"; ///< Parent tag for the track points
};

/**
 * @brief Makes a test case from a file
 * @param i_FileIndex Index of the file in the directory
 * @return An evolution track
 */
std::vector< Herd::SSE::TrackPoint > SSETestFixture::MakeTestCase( std::size_t i_FileIndex )
{

  // Load the file at the indicated index
  boost::property_tree::ptree data = ReadAsXML( s_TrackRegex, i_FileIndex );

  // Initialise the data manager
  Herd::SSE::UnitTests::SSETestDataManager dataManager;
  dataManager.SetData( data.get_child( s_ParentTag ) );
  dataManager.CheckVersionInfo();

  dataManager.PopulateTrackPoints();
  return dataManager.TrackPoints();
}

/**
 * @brief Compares the computed and the actual evolution tracks
 * @param i_rTrack
 */
void SSETestFixture::TestFidelity( const std::vector< Herd::SSE::TrackPoint >& i_rTrack )
{
  BOOST_TEST_REQUIRE( !i_rTrack.empty() );

  // Get the mass and the metallicity from the first track point
  Herd::Generic::Mass initialMass = i_rTrack[ 0 ].m_Mass;
  Herd::Generic::Metallicity initialMetallicity = i_rTrack[ 0 ].m_InitialMetallicity;

  // Evolve until the end of the track
  Herd::Generic::Age evolveUntil = i_rTrack.rbegin()->m_Age;

  // Evolve the star

  Herd::SSE::SingleStarEvolutuion sse;
  Herd::SSE::SingleStarEvolutuion::Parameters parameters;
  sse.Evolve( initialMass, initialMetallicity, evolveUntil, parameters );

  const auto& rTrajectory = sse.Trajectory();

  std::size_t trackLength = rTrajectory.size();

  // TODO When all evolutionary stages are implemented, this will be an equality test
  BOOST_TEST_REQUIRE( i_rTrack.size() >= trackLength );

  for( std::size_t idx = 0; idx < trackLength; ++idx )
  {

    BOOST_TEST_CONTEXT( "Track index "<< idx << " Initial mass " << initialMass << " Initial metallicity "<< initialMetallicity )
    {
      TestTrackPoint( rTrajectory[ idx ], i_rTrack[ idx ] );
    }
  }
}

/**
 * @param i_rActual Actual values
 * @param i_rExpected Expected values
 */
void SSETestFixture::TestTrackPoint( const Herd::SSE::TrackPoint& i_rActual, const Herd::SSE::TrackPoint& i_rExpected )
{

  double temperatureCorrectionFactor = Herd::SSE::Constants::s_SunSurfaceTemperatureSSE / Herd::Physics::Constants::s_SunSurfaceTemperature; //  AMUSE.SSE uses a slightly different value than IAU

  BOOST_TEST( i_rActual.m_Age.Value() == i_rExpected.m_Age.Value(), boost::test_tools::tolerance( 2.1e-4 ) );
  BOOST_TEST( i_rActual.m_Mass.Value() == i_rExpected.m_Mass.Value(), boost::test_tools::tolerance( 1e-4 ) );
  BOOST_TEST( i_rActual.m_InitialMetallicity == i_rExpected.m_InitialMetallicity );
  BOOST_TEST( i_rActual.m_Radius.Value() == i_rExpected.m_Radius.Value(), boost::test_tools::tolerance( 2e-4 ) );
  BOOST_TEST( i_rActual.m_Luminosity.Value() == i_rExpected.m_Luminosity.Value(), boost::test_tools::tolerance( 2e-4 ) );
  BOOST_TEST( i_rActual.m_Temperature.Value() * temperatureCorrectionFactor == i_rExpected.m_Temperature.Value(), boost::test_tools::tolerance( 2e-4 ) );
  BOOST_TEST( ( i_rActual.m_Stage == i_rExpected.m_Stage ) );
  BOOST_TEST( i_rActual.m_EnvelopeMass.Value() == i_rExpected.m_EnvelopeMass.Value(), boost::test_tools::tolerance( 1e-4 ) );

  BOOST_TEST( i_rActual.m_CoreMass.Value() == i_rExpected.m_CoreMass.Value(), boost::test_tools::tolerance( 1e-12 ) );

  // TODO Angular momentum -> Angular speed
  //BOOST_TEST( i_rActual.m_AngularMomentum.Value() == i_rExpected.m_AngularMomentum.Value(), boost::test_tools::tolerance( 1e-12 ) );
}

}

BOOST_FIXTURE_TEST_SUITE(SSETests, SSETestFixture )

BOOST_AUTO_TEST_CASE( InvalidParameters, *Herd::UnitTestUtils::Labels::s_Compile )
{
}

/// Test single star evolution on a random track
BOOST_AUTO_TEST_CASE( RandomReferenceTrack, *Herd::UnitTestUtils::Labels::s_Compile )
{
  std::size_t fileCount = GetFileCount( s_TrackRegex );
  BOOST_TEST_REQUIRE( fileCount != 0 );
  std::size_t fileIndex = GenerateNumber( static_cast< std::size_t >( 0 ), fileCount - 1 );

  TestFidelity( MakeTestCase( fileIndex ) );
}

/// Test single star evolution over the entire reference set
BOOST_AUTO_TEST_CASE( AllReferenceTracks, *Herd::UnitTestUtils::Labels::s_Continuous )
{
  // TODO Review the decision to load one file at a time
  // Reading one file at a time. Alternative is loading all files, which is more amenable to parallel processing, but with a higher up-front cost. More suitable when we are not expecting any failures

  std::size_t fileCount = GetFileCount( s_TrackRegex );
  BOOST_TEST_REQUIRE( fileCount != 0 );
  for( std::size_t idx = 0; idx < fileCount; ++idx )
  {
    BOOST_TEST_CONTEXT( "File index "<<idx )
    {
      TestFidelity( MakeTestCase( idx ) );
    }
  }
}

BOOST_AUTO_TEST_SUITE_END( )



