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
#include <SSE/StarState.h>
#include <SSE/ZeroAgeMainSequence.h>

#include <iterator>
#include <optional>
#include <string>

#include <boost/property_tree/ptree.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm.hpp>

namespace
{
/**
 * @brief Unit test fixture for \c ZeroAgeMainSequence
 */
class ZAMSTestFixture : public Herd::UnitTestUtils::RandomTestFixture, Herd::UnitTestUtils::DataLoaderFixture
{
public:

  Herd::SSE::StarState GetRandomStar(); ///< Returns a random star from the catalogue

  Herd::Generic::Mass GenerateRandomMass(); ///< Generates a random valid mass value
  Herd::Generic::Metallicity GenerateRandomZ(); ///< Generates a random valid metallicity value

  static bool IsValid( Herd::Generic::Mass i_Mass ); ///< Returns true if valid
  static bool IsValid( Herd::Generic::Metallicity i_Z ); ///< Returns true if valid

  static void IsWithinErrorTolerance( const Herd::SSE::StarState& i_rActual, const Herd::SSE::StarState& i_rExpected ); ///< Tests whether the state difference is within permissible bounds

  const std::vector< Herd::SSE::StarState > Stars();  ///< \c m_Stars accessor

private:

  void LoadTestData();  ///< Loads the test data

  Herd::SSE::StarState MakeStar( const boost::property_tree::ptree& i_rNode ); ///< Makes a star from a tree node
  void InitialiseStars(); ///< Populates \c m_Stars from \c m_Catalogue

  boost::property_tree::ptree m_Catalogue;  ///< Test data. May contain elements outside of the ZAMS parameter domain
  unsigned int m_StarCount = 0; ///< Number of elements in the test data

  std::vector< Herd::SSE::StarState > m_Stars;  ///< \c m_Catalogue in vector format
};

Herd::SSE::StarState ZAMSTestFixture::GetRandomStar()
{
  // Lazy initialisation
  // Initialisation at constructor loads the file even when for cases that do not need it
  if( m_StarCount == 0 )
  {
    LoadTestData();
  }

  // Rationale: unit tests need only a small number of entries from the catalogue. Hence, on demand, rather than constructing StarState items for all entries

  // Seek to the entry
  auto iStar = std::next( m_Catalogue.get_child( "Catalogue" ).begin(), GenerateNumber( 0u, m_StarCount - 1 ) ); // @suppress("Invalid arguments")
  return MakeStar( iStar->second ); // @suppress("Invalid arguments") // @suppress("Field cannot be resolved")
}

Herd::Generic::Mass ZAMSTestFixture::GenerateRandomMass()
{
  return Herd::Generic::Mass( GenerateNumber( Herd::SSE::Detail::ZAMS::s_MinMass, Herd::SSE::Detail::ZAMS::s_MaxMass ) );
}

Herd::Generic::Metallicity ZAMSTestFixture::GenerateRandomZ()
{
  return Herd::Generic::Metallicity( GenerateNumber( Herd::SSE::Detail::ZAMS::s_MinZ, Herd::SSE::Detail::ZAMS::s_MaxZ ) );
}

/**
 * @param i_Mass Mass
 * @return \c true if the mass is within the permissible range
 */
bool ZAMSTestFixture::IsValid( Herd::Generic::Mass i_Mass )
{
  return i_Mass.Value() >= Herd::SSE::Detail::ZAMS::s_MinMass && i_Mass.Value() <= Herd::SSE::Detail::ZAMS::s_MaxMass;
}

/**
 * @param i_Z Metallicity
 * @return \c true if the metallicity is within the permissible range
 */
bool ZAMSTestFixture::IsValid( Herd::Generic::Metallicity i_Z )
{
  return i_Z.Value() >= Herd::SSE::Detail::ZAMS::s_MinZ && i_Z.Value() <= Herd::SSE::Detail::ZAMS::s_MaxZ;
}

/**
 * @param i_rActual Actual values
 * @param i_rExpected Expected values
 */
void ZAMSTestFixture::IsWithinErrorTolerance( const Herd::SSE::StarState& i_rActual, const Herd::SSE::StarState& i_rExpected )
{
  BOOST_TEST( i_rActual.m_Radius.Value() == i_rExpected.m_Radius.Value(), boost::test_tools::tolerance( Herd::SSE::Detail::ZAMS::s_MaxRadiusError ) );
  BOOST_TEST( i_rActual.m_Temperature.Value() == i_rExpected.m_Temperature.Value(),
      boost::test_tools::tolerance( Herd::SSE::Detail::ZAMS::s_MaxTemperatureError ) );
  BOOST_TEST( i_rActual.m_Luminosity.Value() == i_rExpected.m_Luminosity.Value(),
      boost::test_tools::tolerance( Herd::SSE::Detail::ZAMS::s_MaxLuminosityError ) );
}

void ZAMSTestFixture::LoadTestData()
{
  m_Catalogue = ReadXML();
  m_StarCount = std::distance( m_Catalogue.get_child( "Catalogue" ).begin(), m_Catalogue.get_child( "Catalogue" ).end() ); // @suppress("Invalid arguments")
  BOOST_TEST_REQUIRE( m_StarCount != 0 );
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
const std::vector< Herd::SSE::StarState > ZAMSTestFixture::Stars()
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
Herd::SSE::StarState ZAMSTestFixture::MakeStar( const boost::property_tree::ptree& i_rNode )
{
  Herd::SSE::StarState Star;
  try
  {
    Star.m_Age.Set( 0.0 );
    Star.m_Luminosity.Set( i_rNode.get< double >( "<xmlattr>.L" ) ); // @suppress("Invalid arguments") @suppress("Field cannot be resolved") @suppress("Method cannot be resolved") @suppress("Symbol is not resolved")
    Star.m_Mass.Set( i_rNode.get< double >( "<xmlattr>.M" ) ); // @suppress("Invalid arguments") @suppress("Field cannot be resolved") @suppress("Method cannot be resolved") @suppress("Symbol is not resolved")
    Star.m_Radius.Set( i_rNode.get< double >( "<xmlattr>.R" ) ); // @suppress("Invalid arguments") @suppress("Field cannot be resolved") @suppress("Method cannot be resolved") @suppress("Symbol is not resolved")
    Star.m_Temperature.Set( i_rNode.get< double >( "<xmlattr>.T" ) ); // @suppress("Invalid arguments") @suppress("Field cannot be resolved") @suppress("Method cannot be resolved") @suppress("Symbol is not resolved")
    Star.m_Z.Set( i_rNode.get< double >( "<xmlattr>.Z" ) ); // @suppress("Invalid arguments") @suppress("Field cannot be resolved") @suppress("Method cannot be resolved") @suppress("Symbol is not resolved")
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
  Herd::Generic::Metallicity z = GenerateRandomZ();
  Herd::Generic::Mass m = GenerateRandomMass();

  auto starState = Herd::SSE::ZeroAgeMainSequence::ComputeStarState( m, z );

  BOOST_TEST( starState.m_Age == 0 );
  BOOST_TEST( starState.m_Luminosity > 0.0 );
  BOOST_TEST( starState.m_Mass == m );
  BOOST_TEST( starState.m_Radius > 0 );
  BOOST_TEST( starState.m_Temperature > 0 );
  BOOST_TEST( starState.m_Z == z );
}

/// Tests for invalid cases
BOOST_AUTO_TEST_CASE( InvalidParameters, *Herd::UnitTestUtils::Labels::s_Compile )
{
  {
    Herd::Generic::Metallicity z(
        GenerateRandomZ() + ( GenerateBool() ? Herd::SSE::Detail::ZAMS::s_MaxZ : ( Herd::SSE::Detail::ZAMS::s_MinZ - Herd::SSE::Detail::ZAMS::s_MaxZ ) ) );
    Herd::Generic::Mass m = GenerateRandomMass();
    BOOST_CHECK_THROW( Herd::SSE::ZeroAgeMainSequence::ComputeStarState( m, z ), Herd::Exceptions::PreconditionError );
  }

  {
    Herd::Generic::Metallicity z = GenerateRandomZ();
    Herd::Generic::Mass m(
        GenerateRandomMass()
            + ( GenerateBool() ? Herd::SSE::Detail::ZAMS::s_MaxMass : ( Herd::SSE::Detail::ZAMS::s_MinMass - Herd::SSE::Detail::ZAMS::s_MaxMass ) ) );
    BOOST_CHECK_THROW( Herd::SSE::ZeroAgeMainSequence::ComputeStarState( m, z ), Herd::Exceptions::PreconditionError );
  }

}

/// Test ZAMS computation with a random pick from external data
BOOST_AUTO_TEST_CASE( ReferenceData, *Herd::UnitTestUtils::Labels::s_Compile )
{

  std::optional< Herd::SSE::StarState > Expected;

  // Try getting a valid star from the catalogue
  for( int c = 0; c < 5; ++c )
  {
    Expected = GetRandomStar();

    if( IsValid( Expected->m_Mass ) && IsValid( Expected->m_Z ) )
    {
      break;
    } else
    {
      Expected.reset();
    }
  }

  BOOST_TEST_REQUIRE( Expected.has_value(),
      "Unable to get a star within the specified parameter range. There is a very small chance that this is a spurious failure" );

  BOOST_TEST_CONTEXT( "Mass " << Expected->m_Mass << " Metallicity "<< Expected->m_Z )
  {
    auto Actual = Herd::SSE::ZeroAgeMainSequence::ComputeStarState( Expected->m_Mass, Expected->m_Z );
    IsWithinErrorTolerance( Actual, *Expected );
  }
}

// Calculate ZAMS and verify over the entire test data
/// Test ZAMS computation over the entire reference data set
BOOST_AUTO_TEST_CASE( CatalogueTest, *Herd::UnitTestUtils::Labels::s_Continuous )
{
  const auto& rStars = Stars();

  for( const auto& Current : rStars | boost::adaptors::indexed() )
  {
    const Herd::SSE::StarState& Expected = Current.value(); // @suppress("Method cannot be resolved")

    if( !IsValid( Expected.m_Mass ) || !IsValid( Expected.m_Z ) )
    {
      continue;
    }

    std::size_t Idx = Current.index(); // @suppress("Method cannot be resolved")
    BOOST_TEST_CONTEXT( "Index "<< Idx << " Mass " << Expected.m_Mass << " Metallicity "<< Expected.m_Z )
    {
      auto Actual = Herd::SSE::ZeroAgeMainSequence::ComputeStarState( Expected.m_Mass, Expected.m_Z );
      IsWithinErrorTolerance( Actual, Expected );
    }
  }
}

BOOST_AUTO_TEST_SUITE_END( )


