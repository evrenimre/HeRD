/**
 * @file UnitTestUtilityFunctions.h
 * @author Evren Imre
 * @date 18 Apr 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HDC594884_9ADE_4590_9B02_A26526B728C3
#define HDC594884_9ADE_4590_9B02_A26526B728C3

#include <boost/test/results_collector.hpp>
#include <boost/test/unit_test.hpp>

namespace Herd::UnitTestUtils
{
bool CheckCurrentTestCaseStatus();  ///< Checks whether the current test case passed

}



#endif /* HDC594884_9ADE_4590_9B02_A26526B728C3 */
