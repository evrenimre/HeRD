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

#include <string>

#include "Exception.h"

namespace Herd::Exceptions
{
/**
 * @brief Exception class for generic runtime errors
 */
class RuntimeError : public Herd::Exceptions::Exception
{
public:
  /**
   * @brief Constructor
   * @param i_rMessage Exception message
   */
  RuntimeError( const std::string& i_rMessage ) :
      Herd::Exceptions::Exception( i_rMessage )
  {
  }

  virtual ~RuntimeError() = default;  ///< Destructor
};
}

#endif /* HBAFA1110_ABC4_4A22_889E_83AC9BA6E9D7 */
