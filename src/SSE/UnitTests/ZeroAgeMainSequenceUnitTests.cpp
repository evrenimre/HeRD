/**
 * @file ZeroAgeMainSequenceUnitTests.cpp
 * @author Evren Imre
 * @date 29 Apr 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <boost/test/unit_test.hpp>

#include <UnitTestUtils/DataLoaderFixture.h>
#include <UnitTestUtils/RandomTestFixture.h>
#include <UnitTestUtils/UnitTestUtilityFunctions.h>

#include <Exceptions/PreconditionError.h>
#include <Generic/Quantities.h>
#include <SSE/Star.h>
#include <SSE/ZeroAgeMainSequence.h>

#include <cstddef>
#include <string>
#include <utility>

#include <boost/property_tree/ptree.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm.hpp>

#include "SSETestDataManager.h"

namespace
{
/**
 * @brief Unit test fixture for \c ZeroAgeMainSequence
 */
class ZAMSTestFixture : public Herd::UnitTestUtils::RandomTestFixture, public Herd::UnitTestUtils::DataLoaderFixture, public Herd::SSE::ZeroAgeMainSequenceSpecs
{
public:

  Herd::SSE::Star GetRandomStar(); ///< Returns a random star from the catalogue
  const std::vector< Herd::SSE::Star >& GetTrackPoints();  ///< Accessor for the track points in ZAMSTestFixture::m_DataManager

  std::pair< Herd::Generic::Mass, Herd::Generic::Metallicity > GenerateRandomInput();  ///< Generates a random mass and metallicity pair

  static void IsWithinErrorTolerance( const Herd::SSE::Star& i_rActual, const Herd::SSE::Star& i_rExpected ); ///< Tests whether the difference is within permissible bounds

private:

  void LoadTestData();  ///< Loads the test data

  Herd::SSE::UnitTests::SSETestDataManager m_DataManager;

  inline static std::string s_ParentTag = "ZAMS"; ///< Parent tag for the track points
};

Herd::SSE::Star ZAMSTestFixture::GetRandomStar()
{
  // Lazy initialisation
  // Initialisation at constructor loads the file even when for cases that do not need it
  if( m_DataManager.TrackPointCount() == 0 )
  {
    LoadTestData();
  }

  // Rationale: unit tests need only a small number of entries from the catalogue. Hence, on demand, rather than constructing Star items for all entries
  return m_DataManager.MakeTrackPoint( GenerateNumber( static_cast< std::size_t >( 0 ), m_DataManager.TrackPointCount() - 1 ) );
}

/**
 * @param i_rActual Actual values
 * @param i_rExpected Expected values
 */
void ZAMSTestFixture::IsWithinErrorTolerance( const Herd::SSE::Star& i_rActual, const Herd::SSE::Star& i_rExpected )
{
  BOOST_TEST( i_rActual.m_Radius.Value() == i_rExpected.m_Radius.Value(), boost::test_tools::tolerance( 2e-4 ) );
  BOOST_TEST( i_rActual.m_Temperature.Value() == i_rExpected.m_Temperature.Value(),
      boost::test_tools::tolerance( 2e-4 ) );
  BOOST_TEST( i_rActual.m_Luminosity.Value() == i_rExpected.m_Luminosity.Value(), boost::test_tools::tolerance( 2e-4 ) );
}

/**
 * @return A constant reference to track points
 */
const std::vector< Herd::SSE::Star >& ZAMSTestFixture::GetTrackPoints()
{
  if( m_DataManager.TrackPoints().empty() )
  {
    m_DataManager.PopulateTrackPoints();
  }

  return m_DataManager.TrackPoints();
}

void ZAMSTestFixture::LoadTestData()
{
  boost::property_tree::ptree document = ReadXML();

  BOOST_TEST_REQUIRE( !document.get_child( s_ParentTag ).empty() );
  m_DataManager.SetData( document.get_child( s_ParentTag ) ); // @suppress("Invalid arguments") // @suppress("Field cannot be resolved")

  m_DataManager.CheckVersionInfo();

  BOOST_TEST_REQUIRE( m_DataManager.TrackPointCount() != 0 );
}

std::pair< Herd::Generic::Mass, Herd::Generic::Metallicity > ZAMSTestFixture::GenerateRandomInput()
{
  //@formatter:off
  return { Herd::Generic::Mass( GenerateNumber( s_MassRange.Lower(), s_MassRange.Upper() ) ),
      Herd::Generic::Metallicity( GenerateNumber( s_ZRange.Lower(), s_ZRange.Upper() ) ) };
    //@formatter:on
}

}

BOOST_FIXTURE_TEST_SUITE( ZAMS, ZAMSTestFixture )

/// Test ZAMS computation with a random input
BOOST_AUTO_TEST_CASE( ZerAgeMainSequenceTest, *Herd::UnitTestUtils::Labels::s_Compile )
{
  auto [ mass, z ] = GenerateRandomInput();
  auto star = Herd::SSE::ZeroAgeMainSequence::Compute( mass, z );

  BOOST_TEST( star.m_Age == 0 );
  BOOST_TEST( star.m_Luminosity > 0.0 );
  BOOST_TEST( star.m_Mass == mass );
  BOOST_TEST( star.m_Radius > 0 );
  BOOST_TEST( star.m_Temperature > 0 );
  BOOST_TEST( star.m_InitialMetallicity == z );
}

/// Tests for invalid cases
BOOST_AUTO_TEST_CASE( InvalidParameters, *Herd::UnitTestUtils::Labels::s_Compile )
{
  auto [ mass, z ] = GenerateRandomInput();

  Herd::Generic::Metallicity invalidZ( z + ( GenerateBool() ? s_ZRange.Upper() : s_ZRange.Lower() - s_ZRange.Upper() ) );
  BOOST_CHECK_THROW( Herd::SSE::ZeroAgeMainSequence::Compute( mass, invalidZ ), Herd::Exceptions::PreconditionError );

  Herd::Generic::Mass invalidMass( mass + ( GenerateBool() ? s_MassRange.Upper() : s_MassRange.Lower() - s_MassRange.Upper() ) );
  BOOST_CHECK_THROW( Herd::SSE::ZeroAgeMainSequence::Compute( invalidMass, z ), Herd::Exceptions::PreconditionError );
}

/// Test ZAMS computation with a random pick from external data
BOOST_AUTO_TEST_CASE( ReferenceData, *Herd::UnitTestUtils::Labels::s_Compile )
{
  bool bFound = false;
  for( int c = 0; c < 5; ++c )
  {
    Herd::SSE::Star Expected = GetRandomStar();
    Herd::Generic::Mass mass = Expected.m_Mass;
    Herd::Generic::Metallicity z = Expected.m_InitialMetallicity;

    if( s_MassRange.Contains( mass ) && s_ZRange.Contains( z ) )
    {
      BOOST_TEST_CONTEXT( "Mass " << mass << " Metallicity "<< z )
      {
        auto Actual = Herd::SSE::ZeroAgeMainSequence::Compute( mass, z );
        IsWithinErrorTolerance( Actual, Expected );
        bFound = true;
        break;
      }
    }
  }

  BOOST_TEST( bFound, "Unable to get a star within the algorithn specs. There is a very small chance that this is a spurious failure" );
}

/// Test ZAMS computation over the entire reference data set
BOOST_AUTO_TEST_CASE( CatalogueTest, *Herd::UnitTestUtils::Labels::s_Continuous )
{

  auto Filter = [ & ]( const auto& i_rStar )
  { return s_MassRange.Contains( i_rStar.value().m_Mass) && s_ZRange.Contains( i_rStar.value().m_InitialMetallicity );};
  
  for( const auto& Current : GetTrackPoints() | boost::adaptors::indexed() | boost::adaptors::filtered( Filter ) )
  {
    const Herd::SSE::Star& Expected = Current.value(); // @suppress("Method cannot be resolved")
    std::size_t Idx = Current.index(); // @suppress("Method cannot be resolved")
    BOOST_TEST_CONTEXT( "Index "<< Idx << " Mass " << Expected.m_Mass << " Metallicity "<< Expected.m_InitialMetallicity )
    {
      auto Actual = Herd::SSE::ZeroAgeMainSequence::Compute( Expected.m_Mass, Expected.m_InitialMetallicity );
      IsWithinErrorTolerance( Actual, Expected );
    }
  }
}

BOOST_AUTO_TEST_SUITE_END( )


