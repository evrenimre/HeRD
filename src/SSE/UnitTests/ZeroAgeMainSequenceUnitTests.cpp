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

#include <Exceptions/PreconditionError.h>
#include <Generic/Quantities.h>
#include <SSE/StarState.h>
#include <SSE/ZeroAgeMainSequence.h>

#include <iterator>
#include <optional>
#include <string>

#include <boost/property_tree/ptree.hpp>

namespace
{
class ZAMSTestFixture : public Herd::UnitTestUtils::RandomTestFixture, Herd::UnitTestUtils::DataLoaderFixture
{
public:

  Herd::SSE::StarState GetRandomStar(); ///< Returns a random star from the catalogue

  Herd::Generic::Mass GenerateRandomMass(); ///< Generates a random valid mass value
  Herd::Generic::Metallicity GenerateRandomZ(); ///< Generates a random valid metallicity value

  static bool IsValid( Herd::Generic::Mass i_Mass ); ///< Returns true if valid
  static bool IsValid( Herd::Generic::Metallicity i_Z ); ///< Returns true if valid

private:

  void LoadTestData();  ///< Loads the test data

  boost::property_tree::ptree m_Catalogue;  ///< Test data. May contain elements outside of the range for which the ZAMS algorithm is defined
  unsigned int m_StarCount = 0; ///< Number of elements in the test data
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

  Herd::SSE::StarState Star;
  try
  {
    Star.m_Age.Set( 0.0 );
    Star.m_Luminosity.Set( iStar->second.get< double >( "<xmlattr>.L" ) ); // @suppress("Invalid arguments") @suppress("Field cannot be resolved") @suppress("Method cannot be resolved") @suppress("Symbol is not resolved")
    Star.m_Mass.Set( iStar->second.get< double >( "<xmlattr>.M" ) ); // @suppress("Invalid arguments") @suppress("Field cannot be resolved") @suppress("Method cannot be resolved") @suppress("Symbol is not resolved")
    Star.m_Radius.Set( iStar->second.get< double >( "<xmlattr>.R" ) ); // @suppress("Invalid arguments") @suppress("Field cannot be resolved") @suppress("Method cannot be resolved") @suppress("Symbol is not resolved")
    Star.m_Temperature.Set( iStar->second.get< double >( "<xmlattr>.T" ) ); // @suppress("Invalid arguments") @suppress("Field cannot be resolved") @suppress("Method cannot be resolved") @suppress("Symbol is not resolved")
    Star.m_Z.Set( iStar->second.get< double >( "<xmlattr>.Z" ) ); // @suppress("Invalid arguments") @suppress("Field cannot be resolved") @suppress("Method cannot be resolved") @suppress("Symbol is not resolved")
  } catch( ... )
  {
    BOOST_TEST_REQUIRE( false, "Invalid or missing star attribute" );
  }

  return Star;
}

Herd::Generic::Mass ZAMSTestFixture::GenerateRandomMass()
{
  return Herd::Generic::Mass( GenerateNumber( Herd::SSE::Detail::ZAMS::s_MinMass, Herd::SSE::Detail::ZAMS::s_MaxMass ) );
}

Herd::Generic::Metallicity ZAMSTestFixture::GenerateRandomZ()
{
  return Herd::Generic::Metallicity( GenerateNumber( Herd::SSE::Detail::ZAMS::s_MinZ, Herd::SSE::Detail::ZAMS::s_MaxZ ) );
}

bool ZAMSTestFixture::IsValid( Herd::Generic::Mass i_Mass )
{
  return i_Mass.Value() >= Herd::SSE::Detail::ZAMS::s_MinMass && i_Mass.Value() <= Herd::SSE::Detail::ZAMS::s_MaxMass;
}

bool ZAMSTestFixture::IsValid( Herd::Generic::Metallicity i_Z )
{
  return i_Z.Value() >= Herd::SSE::Detail::ZAMS::s_MinZ && i_Z.Value() <= Herd::SSE::Detail::ZAMS::s_MaxZ;
}

void ZAMSTestFixture::LoadTestData()
{
  m_Catalogue = ReadXML();
  m_StarCount = std::distance( m_Catalogue.get_child( "Catalogue" ).begin(), m_Catalogue.get_child( "Catalogue" ).end() ); // @suppress("Invalid arguments")
  BOOST_TEST_REQUIRE( m_StarCount != 0 );
}

}

BOOST_FIXTURE_TEST_SUITE( ZAMS, ZAMSTestFixture )

BOOST_AUTO_TEST_CASE( ZerAgeMainSequenceTest, *boost::unit_test::tolerance( 1e-8 ) )
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

BOOST_AUTO_TEST_CASE( InvalidParameters )
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

BOOST_AUTO_TEST_CASE( ReferenceData )
{

  std::optional< Herd::SSE::StarState > Expected;

  // Try getting a valid star from the catalogue
  for( int c = 0; c < 25; ++c )
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

    BOOST_TEST( Actual.m_Radius.Value() == Expected->m_Radius.Value(), boost::test_tools::tolerance( Herd::SSE::Detail::ZAMS::s_MaxRadiusError ) );
    BOOST_TEST( Actual.m_Temperature.Value() == Expected->m_Temperature.Value(),
        boost::test_tools::tolerance( Herd::SSE::Detail::ZAMS::s_MaxTemperatureError ) );
    BOOST_TEST( Actual.m_Luminosity.Value() == Expected->m_Luminosity.Value(), boost::test_tools::tolerance( Herd::SSE::Detail::ZAMS::s_MaxLuminosityError ) );
  }
}

BOOST_AUTO_TEST_SUITE_END( )

