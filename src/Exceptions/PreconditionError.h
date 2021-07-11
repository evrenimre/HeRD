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

#include <Concepts/GenericConcepts.h>

#include <string>

#include <boost/format.hpp>
#include <boost/stacktrace.hpp>

namespace Herd::Exceptions
{
/**
 * @brief Exception class indicating that a precondition was not met
 * @remarks Stacktrace is not called in \c ComposeMessage so that the frame does not unnecessarily include an implementation detail
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
      Herd::Exceptions::RuntimeError( ComposeMessage( i_rElement, i_rExpected, i_rActual ) + boost::stacktrace::to_string( boost::stacktrace::stacktrace() ) )
  {
  }

  /**
   * @brief Constructor
   * @param i_pElement Element tested for precondition, often a variable
   * @param i_pExpected Precondition
   * @param i_pActual Actual value
   */
  PreconditionError( const char* i_pElement, const char* i_pExpected, const char* i_pActual ) :
      Herd::Exceptions::RuntimeError( ComposeMessage( i_pElement, i_pExpected, i_pActual ) + boost::stacktrace::to_string( boost::stacktrace::stacktrace() ) )
  {
  }

  /**
   * @brief Constructor for arithmetic types
   * @tparam T An arithmetic type
   * @param i_rElement Element tested for precondition
   * @param i_rExpected Precondition
   * @param i_Actual Actual value
   */
  template< Herd::Concepts::Arithmetic T > // @suppress("Type cannot be resolved")
  PreconditionError( const std::string& i_rElement, const std::string& i_rExpected, T i_Actual ) : // @suppress("Type cannot be resolved")
      Herd::Exceptions::RuntimeError( // @suppress("Symbol is not resolved")
          ComposeMessage( i_rElement, i_rExpected, std::to_string( i_Actual ) + boost::stacktrace::to_string( boost::stacktrace::stacktrace() ) ) ) // @suppress("Invalid arguments")
  {
  }

private:
  /**
   * @brief Composes the error message for the exception
   * @param i_rElement Element being tested
   * @param i_rExpected Precondition
   * @param i_rActual Actual value
   * @return Error message
   */
  static std::string ComposeMessage( const std::string& i_rElement, const std::string& i_rExpected, const std::string& i_rActual )
  {
    return boost::str( boost::format( "%s: Expected %s got %s\n" ) % i_rElement % i_rExpected % i_rActual );
  }
};

}

#endif /* H09964061_1CF4_4FC5_ADE4_FE0282B49412 */
