/**
 * @file UnitTestUtilityFunctions.cpp
 * @author Evren Imre
 * @date 18 Apr 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "UnitTestUtilityFunctions.h"

namespace Herd::UnitTestUtils
{

bool CheckCurrentTestCaseStatus()
{
  auto testId = boost::unit_test::framework::current_test_case().p_id;
  return boost::unit_test::results_collector.results( testId ).passed();
}
}


