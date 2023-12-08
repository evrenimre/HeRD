/**
 * @file GiantBranchRadiusUnitTests.cpp
 * @author Evren Imre
 * @date 8 Dec 2023	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2023 Evren Imre
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <SSE/Landmarks/GiantBranchRadius.h>

#include <Exceptions/PreconditionError.h>
#include <Generic/Quantity.h>
#include <UnitTestUtils/RandomTestFixture.h>
#include <UnitTestUtils/UnitTestUtilityFunctions.h>

BOOST_FIXTURE_TEST_SUITE( GiantBranchRadiusTests, Herd::UnitTestUtils::RandomTestFixture )

// Testing only validation
// Correctness tests are performed through the single star evolution unit tests
BOOST_AUTO_TEST_CASE( ValidationTest, *Herd::UnitTestUtils::Labels::s_Compile )
{
  Herd::Generic::Metallicity validMetallicity( GenerateNumber( 1e-4, 3e-2 ) );  // @suppress("Invalid arguments")
  Herd::Generic::Mass validMass( GenerateNumber( 0.5, 50. ) );  // @suppress("Invalid arguments")
  Herd::Generic::Luminosity validLuminosity( GenerateNumber( 0.000126, 6166000. ) );  // @suppress("Invalid arguments")

  Herd::Generic::Metallicity invalidMetallicity( -validMetallicity );
  Herd::Generic::Mass invalidMass( -validMass );
  Herd::Generic::Luminosity invalidLuminosity( -validLuminosity );

  // Construction
  // Does not compile if not pointer
  BOOST_CHECK_NO_THROW( std::make_unique< Herd::SSE::GiantBranchRadius >( validMetallicity ) );
  BOOST_CHECK_THROW( std::make_unique< Herd::SSE::GiantBranchRadius >( invalidMetallicity ), Herd::Exceptions::PreconditionError );

  // Operation
  Herd::SSE::GiantBranchRadius computer( validMetallicity );

  BOOST_CHECK_NO_THROW( computer.Compute( validMass, validLuminosity ) );
  BOOST_CHECK_THROW( computer.Compute( invalidMass, validLuminosity ), Herd::Exceptions::PreconditionError );
  BOOST_CHECK_THROW( computer.Compute( validMass, invalidLuminosity ), Herd::Exceptions::PreconditionError );
}

BOOST_AUTO_TEST_SUITE_END( )
