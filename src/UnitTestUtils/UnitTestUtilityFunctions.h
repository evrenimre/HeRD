/**
 * @file UnitTestUtilityFunctions.h
 * @author Evren Imre
 * @date 18 Apr 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HDC594884_9ADE_4590_9B02_A26526B728C3
#define HDC594884_9ADE_4590_9B02_A26526B728C3

#include <optional>
#include <string>
#include <unordered_map>

#include <boost/test/unit_test.hpp>

namespace Herd::UnitTestUtils
{
std::unordered_map< std::string, std::string > ParseCommandLineArguments(); ///< Parses the command line arguments for the test executable
std::optional< std::string > GetCommandLineArgument( const std::string& i_rName ); ///< Returns a command line argument
}



#endif /* HDC594884_9ADE_4590_9B02_A26526B728C3 */
