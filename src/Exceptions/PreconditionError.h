/**
 * @file PreconditionError.h
 * @author Evren Imre
 * @date 16 May 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef H09964061_1CF4_4FC5_ADE4_FE0282B49412
#define H09964061_1CF4_4FC5_ADE4_FE0282B49412

#include "RuntimeError.h"

#include <string>
#include <type_traits>

#include <boost/format.hpp>
#include <boost/stacktrace.hpp>

namespace Herd::Exceptions
{
/**
 * @brief Exception class indicating that a precondition was not met
 */
class PreconditionError : public Herd::Exceptions::RuntimeError
{
public:
  /**
   * @brief Constructor
   * @param i_rElement Element tested for precondition, often a variable
   * @param i_rExpected Precondition
   * @param i_rActual Actual value
   */
  PreconditionError( const std::string& i_rElement, const std::string& i_rExpected, const std::string& i_rActual ) :
      Herd::Exceptions::RuntimeError( ComposeMessage( i_rElement, i_rExpected, i_rActual ) )
  {
  }

  /**
   * @brief Constructor for arithmetic types
   * @tparam Arithmetic An arithmetic type
   * @param i_rElement Element tested for precondition
   * @param i_rExpected Precondition
   * @param i_Actual Actual value
   */
  template< class Arithmetic >
  PreconditionError( const std::string& i_rElement, const std::string& i_rExpected, Arithmetic i_Actual ) :
      Herd::Exceptions::RuntimeError( ComposeMessage( i_rElement, i_rExpected, std::to_string( i_Actual ) ) )
  {
    static_assert( std::is_arithmetic< Arithmetic >::value, "Arihtmetic must be an arithmetic type");
  }

protected:
  /**
   * @brief Composes the error message for the exception
   * @param i_rElement Element being tested
   * @param i_rExpected Precondition
   * @param i_rActual Actual value
   * @return Error message
   */
  static std::string ComposeMessage( const std::string& i_rElement, const std::string& i_rExpected, const std::string& i_rActual )
  {
    return boost::str(
        boost::format( "%s: Expected %s got %s\n %s" ) % i_rElement % i_rExpected % i_rActual
            % boost::stacktrace::to_string( boost::stacktrace::stacktrace() ) );
  }
};

}




#endif /* H09964061_1CF4_4FC5_ADE4_FE0282B49412 */
