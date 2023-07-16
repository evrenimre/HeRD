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

#include <concepts>
#include <string>
#include <string_view>

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
  PreconditionError( const std::string_view& i_rElement, const std::string_view& i_rExpected, const std::string_view& i_rActual ) :
      Herd::Exceptions::RuntimeError( ComposeMessage( i_rElement, i_rExpected, i_rActual ) )
  {
  }

  template< Herd::Concepts::Number T > // @suppress("Type cannot be resolved")
  PreconditionError( const std::string_view& i_rElement, const std::string_view& i_rExpected, T i_Actual ); // @suppress("Type cannot be resolved")

  virtual ~PreconditionError() = default; ///< Default constructor

  ///@name Helpers
  ///@{
  template< typename T >
  requires std::totally_ordered< T > // @suppress("Type cannot be resolved") // @suppress("Invalid template argument")
  static void ThrowIfNegative( T i_Value, const std::string_view& i_rName ); // @suppress("Type cannot be resolved")

  template< typename T >
  requires std::totally_ordered< T > // @suppress("Type cannot be resolved") // @suppress("Invalid template argument")
  static void ThrowIfNotPositive( T i_Value, const std::string_view& i_rName ); // @suppress("Type cannot be resolved")
  ///@}
private:

  static std::string ComposeMessage( const std::string_view& i_rElement, const std::string_view& i_rExpected, const std::string_view& i_rActual ); ///< Composes the error message for the exception
};

// Extern declarations
///@cond
// Suppress doxygen warnings
extern template PreconditionError::PreconditionError( const std::string_view&, const std::string_view&, double ); // @suppress("Member declaration not found")
///@endcond
extern template void PreconditionError::ThrowIfNegative( double, const std::string_view& ); // @suppress("Member declaration not found")
extern template void PreconditionError::ThrowIfNotPositive( double, const std::string_view& ); // @suppress("Member declaration not found")

/**
 * @brief Constructor for numerical types
 * @tparam T Satisfies Herd::Concepts::Number
 * @param i_rElement Element tested for precondition
 * @param i_rExpected Precondition
 * @param i_Actual Actual value
 */
template< Herd::Concepts::Number T > // @suppress("Type cannot be resolved")
PreconditionError::PreconditionError( const std::string_view& i_rElement, const std::string_view& i_rExpected, T i_Actual ) : // @suppress("Type cannot be resolved")
Herd::Exceptions::RuntimeError(// @suppress("Symbol is not resolved")
    ComposeMessage( i_rElement, i_rExpected, std::to_string( i_Actual ) ) )// @suppress("Invalid arguments")
{
}

/**
 * @brief Throws if value is not positive
 * @tparam T Satisfies std::totally_ordered
 * @param i_Value Value to be tested
 * @param i_rName Name of the variable
 * @throws PreconditionError If \c i_Value is not positive
 */
//@formatter:off
template< typename T >
requires std::totally_ordered< T > // @suppress("Invalid template argument") // @suppress("Type cannot be resolved")
//@formatter:on
void PreconditionError::ThrowIfNegative( T i_Value, const std::string_view& i_rName ) // @suppress("Type cannot be resolved")
{
  if( i_Value < 0 )
  {
    [[unlikely]] throw( Exceptions::PreconditionError( i_rName, ">=0", i_Value ) ); // @suppress("Symbol is not resolved")
  }
}

/**
 * @brief Throws if value is negative
 * @tparam T Satisfies std::totally_ordered
 * @param i_Value Value to be tested
 * @param i_rName Name of the variable
 * @throws PreconditionError If \c i_Value is negative
 */
//@formatter:off
template< typename T >
requires std::totally_ordered< T > // @suppress("Invalid template argument") // @suppress("Type cannot be resolved")
//@formatter:on
void PreconditionError::ThrowIfNotPositive( T i_Value, const std::string_view& i_rName ) // @suppress("Type cannot be resolved")
{
  if( i_Value <= 0 )
  {
    [[unlikely]] throw( Exceptions::PreconditionError( i_rName, ">0", i_Value ) ); // @suppress("Symbol is not resolved")
  }
}

}

#endif /* H09964061_1CF4_4FC5_ADE4_FE0282B49412 */
