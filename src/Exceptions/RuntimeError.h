/**
 * @file RuntimeError.h
 * @author Evren Imre
 * @date 16 May 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HBAFA1110_ABC4_4A22_889E_83AC9BA6E9D7
#define HBAFA1110_ABC4_4A22_889E_83AC9BA6E9D7

#include <stdexcept>
#include <string>

namespace Herd::Exceptions
{
/**
 * @brief Exception class for generic runtime errors
 * @remarks Inherits \c what from \c std::runtime_error
 */
class RuntimeError : public std::runtime_error
{
public:
  RuntimeError( const std::string& i_rMessage ) :
      std::runtime_error( i_rMessage )
  {
  }
};
}

#endif /* HBAFA1110_ABC4_4A22_889E_83AC9BA6E9D7 */
