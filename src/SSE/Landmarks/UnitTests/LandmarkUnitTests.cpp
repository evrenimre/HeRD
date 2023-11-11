/**
 * @file LandmarkUnitTests.cpp
 * @author Evren Imre
 * @date 10 Nov 2023	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2023 Evren Imre
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <Exceptions/PreconditionError.h>
#include <Generic/Quantity.h>
#include <SSE/Landmarks/BaseOfGiantBranch.h>
#include <SSE/Landmarks/ILandmark.h>
#include <SSE/Landmarks/TerminalMainSequence.h>
#include <UnitTestUtils/RandomTestFixture.h>
#include <UnitTestUtils/UnitTestUtilityFunctions.h>

#include <memory>
#include <map>

namespace
{

/**
 * @brief Landmark names
 */
enum class ELandmarkType
{
  e_TMS,
  e_BGB,
};
}

BOOST_FIXTURE_TEST_SUITE( StellarRotationTests, Herd::UnitTestUtils::RandomTestFixture )

// Only testing input validation
// Correctness tests are performed through the single star evolution unit tests
BOOST_AUTO_TEST_CASE( ValidationTest, *Herd::UnitTestUtils::Labels::s_Compile )
{
  Herd::Generic::Metallicity validMetallicity( GenerateNumber( 1e-4, 3e-2 ) );  // @suppress("Invalid arguments")
  Herd::Generic::Mass validMass( GenerateNumber( 0.5, 50. ) );  // @suppress("Invalid arguments")

  Herd::Generic::Metallicity invalidMetallicity( -validMetallicity );
  Herd::Generic::Mass invalidMass( -validMass );

  std::map< ELandmarkType, std::unique_ptr< Herd::SSE::ILandmark > > Landmarks;

  // Invalid construction
  BOOST_CHECK_THROW( Landmarks.emplace( ELandmarkType::e_TMS, std::make_unique< Herd::SSE::TerminalMainSequence >( invalidMetallicity ) ),
      Herd::Exceptions::PreconditionError );
  BOOST_CHECK_THROW( Landmarks.emplace( ELandmarkType::e_BGB, std::make_unique< Herd::SSE::BaseOfGiantBranch >( invalidMetallicity ) ),
      Herd::Exceptions::PreconditionError );

  // Valid construction
  BOOST_CHECK_NO_THROW( Landmarks.emplace( ELandmarkType::e_TMS, std::make_unique< Herd::SSE::TerminalMainSequence >( validMetallicity ) ) );
  BOOST_CHECK_NO_THROW( Landmarks.emplace( ELandmarkType::e_BGB, std::make_unique< Herd::SSE::BaseOfGiantBranch >( validMetallicity ) ) );

  // Common interface
  for( auto& [ key, pLandmark ] : Landmarks )
  {
    // Valid input
    BOOST_CHECK_NO_THROW( pLandmark->Age( validMass ) );
    BOOST_CHECK_NO_THROW( pLandmark->Luminosity( validMass ) );
    BOOST_CHECK_NO_THROW( pLandmark->Radius( validMass ) );

    // Invalid input
    BOOST_CHECK_THROW( pLandmark->Age( invalidMass ), Herd::Exceptions::PreconditionError );
    BOOST_CHECK_THROW( pLandmark->Luminosity( invalidMass ), Herd::Exceptions::PreconditionError );
    BOOST_CHECK_THROW( pLandmark->Radius( invalidMass ), Herd::Exceptions::PreconditionError );
  }

  // Type-specific

  // TMS
  BOOST_CHECK_NO_THROW( dynamic_cast< Herd::SSE::TerminalMainSequence* >( Landmarks[ ELandmarkType::e_TMS ].get() )->THook( validMass ) );
  BOOST_CHECK_THROW( dynamic_cast< Herd::SSE::TerminalMainSequence* >( Landmarks[ ELandmarkType::e_TMS ].get() )->THook( invalidMass ),
      Herd::Exceptions::PreconditionError );
}

BOOST_AUTO_TEST_SUITE_END( )
