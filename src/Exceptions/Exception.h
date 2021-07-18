/**
 * @file Exception.h
 * @author Evren Imre
 * @date 18 Jul 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef H20F162CD_082B_4DD2_A5FC_577BBC1CF406
#define H20F162CD_082B_4DD2_A5FC_577BBC1CF406

#include <exception>
#include <string>

namespace Herd::Exceptions
{
/**
 * @brief Base for the HeRD exception tree
 */
class Exception : public std::exception
{
public:

  /**
   * @brief Constructor
   * @param i_rException Exception message
   */
  Exception( const std::string& i_rMessage ) :
      m_Message( i_rMessage )
  {
  }

  virtual ~Exception() = default; ///< Destructor

  /// Returns the contents of Exception::m_Message
  virtual const char* what() const noexcept override
  {
    return m_Message.c_str();
  }

private:
  std::string m_Message;
};
}




#endif /* H20F162CD_082B_4DD2_A5FC_577BBC1CF406 */
