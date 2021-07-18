/**
 * @file EvolutionStageUnitTests.cpp
 * @author Evren Imre
 * @date 6 Jul 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <boost/test/unit_test.hpp>

#include <Exceptions/PreconditionError.h>
#include <SSE/EvolutionStage.h>
#include <UnitTestUtils/UnitTestUtilityFunctions.h>

#include <array>
#include <string>

#include <boost/container/flat_set.hpp>
#include <boost/range/adaptors.hpp>

BOOST_AUTO_TEST_SUITE( EvolutionStagesUnitTests )

/// Tests for string and stage mapping
BOOST_AUTO_TEST_CASE( StringAndStage, *Herd::UnitTestUtils::Labels::s_Compile )
{
  auto stages = Herd::SSE::EnumerateEvolutionStages();

  boost::container::flat_set< Herd::SSE::EvolutionStage > uniqued( stages.begin(), stages.end() );
  BOOST_TEST( uniqued.size() == stages.size() );

  // Round-trip test
  for( const auto& current : stages | boost::adaptors::indexed() )
  {
    BOOST_TEST_CONTEXT( current.index() ) // @suppress("Method cannot be resolved") // @suppress("Symbol is not resolved")
    {
      std::string stageString = Herd::SSE::EvolutionStageToString( current.value() ); // @suppress("Method cannot be resolved") // @suppress("Invalid arguments")
      Herd::SSE::EvolutionStage actual = Herd::SSE::StringToEvolutionStage( stageString );
      BOOST_TEST( ( actual == current.value() ) ); // @suppress("Invalid arguments") // @suppress("Method cannot be resolved")
    }
  }

  // Invalid string
  BOOST_CHECK_THROW( Herd::SSE::StringToEvolutionStage( "Invalid" ), Herd::Exceptions::PreconditionError );
}

BOOST_AUTO_TEST_SUITE_END( )


