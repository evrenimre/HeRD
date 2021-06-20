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

#include <iterator>
#include <optional>
#include <string>
#include <utility>

#include <boost/property_tree/ptree.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm.hpp>

namespace
{
/**
 * @brief Unit test fixture for \c ZeroAgeMainSequence
 */
class ZAMSTestFixture : public Herd::UnitTestUtils::RandomTestFixture, public Herd::UnitTestUtils::DataLoaderFixture, public Herd::SSE::ZeroAgeMainSequenceSpecs
{
public:

  Herd::SSE::Star GetRandomStar(); ///< Returns a random star from the catalogue

  std::pair< Herd::Generic::Mass, Herd::Generic::Metallicity > GenerateRandomInput();  ///< Generates a random mass and metallicity pair

  static void IsWithinErrorTolerance( const Herd::SSE::Star& i_rActual, const Herd::SSE::Star& i_rExpected ); ///< Tests whether the difference is within permissible bounds

  const std::vector< Herd::SSE::Star > Stars();  ///< \c m_Stars accessor

private:

  void LoadTestData();  ///< Loads the test data

  Herd::SSE::Star MakeStar( const boost::property_tree::ptree& i_rNode ); ///< Makes a star from a tree node
  void InitialiseStars(); ///< Populates \c m_Stars from \c m_Catalogue

  boost::property_tree::ptree m_Catalogue;  ///< Test data. May contain elements outside of the ZAMS parameter domain
  unsigned int m_StarCount = 0; ///< Number of elements in the test data

  std::vector< Herd::SSE::Star > m_Stars;  ///< \c m_Catalogue in vector format
};

Herd::SSE::Star ZAMSTestFixture::GetRandomStar()
{
  // Lazy initialisation
  // Initialisation at constructor loads the file even when for cases that do not need it
  if( m_StarCount == 0 )
  {
    LoadTestData();
  }

  // Rationale: unit tests need only a small number of entries from the catalogue. Hence, on demand, rather than constructing Star items for all entries

  // Seek to the entry
  auto iStar = std::next( m_Catalogue.get_child( "Catalogue" ).begin(), GenerateNumber( 0u, m_StarCount - 1 ) ); // @suppress("Invalid arguments")
  return MakeStar( iStar->second ); // @suppress("Invalid arguments") // @suppress("Field cannot be resolved")
}

/**
 * @param i_rActual Actual values
 * @param i_rExpected Expected values
 */
void ZAMSTestFixture::IsWithinErrorTolerance( const Herd::SSE::Star& i_rActual, const Herd::SSE::Star& i_rExpected )
{
  BOOST_TEST( i_rActual.m_Radius.Value() == i_rExpected.m_Radius.Value(), boost::test_tools::tolerance( 2e-4 ) );
  BOOST_TEST( i_rActual.m_Temperature.Value() == i_rExpected.m_Temperature.Value(),
      boost::test_tools::tolerance( s_MaxTemperatureError ) );
  BOOST_TEST( i_rActual.m_Luminosity.Value() == i_rExpected.m_Luminosity.Value(), boost::test_tools::tolerance( 2e-4 ) );
}

void ZAMSTestFixture::LoadTestData()
{
  m_Catalogue = ReadXML();
  m_StarCount = std::distance( m_Catalogue.get_child( "Catalogue" ).begin(), m_Catalogue.get_child( "Catalogue" ).end() ); // @suppress("Invalid arguments")
  BOOST_TEST_REQUIRE( m_StarCount != 0 );
}

std::pair< Herd::Generic::Mass, Herd::Generic::Metallicity > ZAMSTestFixture::GenerateRandomInput()
{
  //@formatter:off
  return { Herd::Generic::Mass( GenerateNumber( s_MassRange.Lower(), s_MassRange.Upper() ) ),
      Herd::Generic::Metallicity( GenerateNumber( s_ZRange.Lower(), s_ZRange.Upper() ) ) };
    //@formatter:on
}

void ZAMSTestFixture::InitialiseStars()
{
  if( m_StarCount == 0 )
  {
    LoadTestData();
  }

  m_Stars.reserve( m_StarCount );
  boost::transform( m_Catalogue.get_child( "Catalogue" ), std::back_inserter( m_Stars ), [ & ]( const auto& i_rNode )
  { return MakeStar(i_rNode.second);} );
}

/**
 * @return A constant reference to \c m_Stars
 */
const std::vector< Herd::SSE::Star > ZAMSTestFixture::Stars()
{
  // Lazy implementation, as this is not needed for compile tests
  if( m_Stars.empty() )
  {
    InitialiseStars();
  }
  
  return m_Stars;
}

/**
 * @param i_rNode A property tree node
 * @return The corresponding star
 */
Herd::SSE::Star ZAMSTestFixture::MakeStar( const boost::property_tree::ptree& i_rNode )
{
  Herd::SSE::Star Star;
  try
  {
    Star.m_Age.Set( 0.0 );
    Star.m_Luminosity.Set( i_rNode.get< double >( "<xmlattr>.L" ) ); // @suppress("Invalid arguments") @suppress("Field cannot be resolved") @suppress("Method cannot be resolved") @suppress("Symbol is not resolved")
    Star.m_Mass.Set( i_rNode.get< double >( "<xmlattr>.M" ) ); // @suppress("Invalid arguments") @suppress("Field cannot be resolved") @suppress("Method cannot be resolved") @suppress("Symbol is not resolved")
    Star.m_Radius.Set( i_rNode.get< double >( "<xmlattr>.R" ) ); // @suppress("Invalid arguments") @suppress("Field cannot be resolved") @suppress("Method cannot be resolved") @suppress("Symbol is not resolved")
    Star.m_Temperature.Set( i_rNode.get< double >( "<xmlattr>.T" ) ); // @suppress("Invalid arguments") @suppress("Field cannot be resolved") @suppress("Method cannot be resolved") @suppress("Symbol is not resolved")
    Star.m_Z.Set( i_rNode.get< double >( "<xmlattr>.Z" ) ); // @suppress("Invalid arguments") @suppress("Field cannot be resolved") @suppress("Method cannot be resolved") @suppress("Symbol is not resolved")

    Star.m_InitialMass = Star.m_Mass;
  } catch( ... )
  {
    BOOST_TEST_REQUIRE( false, "Invalid or missing star attribute" );
  }

  return Star;
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
  BOOST_TEST( star.m_Z == z );
  BOOST_TEST( star.m_InitialMass == mass );
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
    Herd::Generic::Metallicity z = Expected.m_Z;

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
  const auto& rStars = Stars();

  auto Filter = [ & ]( const auto& i_rStar )
  { return s_MassRange.Contains( i_rStar.value().m_Mass) && s_ZRange.Contains( i_rStar.value().m_Z );};
  
  for( const auto& Current : rStars | boost::adaptors::indexed() | boost::adaptors::filtered( Filter ) )
  {
    const Herd::SSE::Star& Expected = Current.value(); // @suppress("Method cannot be resolved")
    std::size_t Idx = Current.index(); // @suppress("Method cannot be resolved")
    BOOST_TEST_CONTEXT( "Index "<< Idx << " Mass " << Expected.m_Mass << " Metallicity "<< Expected.m_Z )
    {
      auto Actual = Herd::SSE::ZeroAgeMainSequence::Compute( Expected.m_Mass, Expected.m_Z );
      IsWithinErrorTolerance( Actual, Expected );
    }
  }
}

BOOST_AUTO_TEST_SUITE_END( )


