/**
 * @file DataLoaderFixture.h
 * @author Evren Imre
 * @date 30 May 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "DataLoaderFixture.h"

#include "UnitTestUtilityFunctions.h"

#include <boost/test/unit_test.hpp>

namespace Herd::UnitTestUtils
{

/**
 * @return Data as a property tree
 * @remarks The test framework will bail as the operation cannot be completed
 */
boost::property_tree::ptree DataLoaderFixture::ReadXML()
{
  std::optional< std::string > dataPath = Herd::UnitTestUtils::GetCommandLineArgument( s_DataArgumentName );
  if( !dataPath )
  {
    BOOST_TEST_REQUIRE( false, "Missing command line argument " + s_DataArgumentName );
  }

  boost::property_tree::ptree data;
  try
  {
    boost::property_tree::read_xml( *dataPath, data );
  } catch( ... )
  {
    BOOST_TEST_REQUIRE( false, "Unable to load test data " + *dataPath );
  }

  return data;
}

}


