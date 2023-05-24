/**
 * @file Quantity.h
 * @author Evren Imre
 * @date 8 Nov 2020	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HD075DEEE_908E_40CA_A6A7_83108CC172DD
#define HD075DEEE_908E_40CA_A6A7_83108CC172DD

#include <Exceptions/ExceptionWrappers.h>

#include <string_view>

namespace Herd::Generic
{

/**
 * @brief Quantity template
 * @tparam Tag Tag for differentiation
 */
template< typename Tag >
class Quantity
{
public:

  /// Constructor
  Quantity() :
      m_Value( 0 )
  {
  } // cppcheck-suppress missingReturn

  /**
   * @brief Constructor
   * @param i_Value Quantity value
   */
  explicit Quantity( double i_Value ) :
      m_Value( i_Value )
  {
  }

  /**
   * @brief Implicit conversion operator
   */
  operator double() const
  {
    return m_Value;
  }

  /**
   * @brief Sets the contained value
   * @param i_Value New value
   */
  void Set( double i_Value )
  {
    m_Value = i_Value;
  }

  /**
   * @brief Returns the contained value
   * @return A copy of \c m_Value
   */
  double Value() const
  {
    return m_Value;
  }

  ///@name Comparison operators
  ///@{
  // Default member operators only work with const&
  bool operator==( const Quantity< Tag >& ) const = default; ///< Equality operator
  auto operator<=>( const Quantity< Tag >& ) const = default;///< Spaceship operator
  ///@}

  ///@name Arithmetic assignment operators
  ///@{

  /**
   * @brief Add-assign operator
   * @param i_rOther Addend
   */
  void operator+=( Quantity< Tag > i_rOther )
  {
    m_Value += i_rOther.m_Value;
  }

  /**
   * @brief Subtract-assign operator
   * @param i_rOther Subrrahend
   */
  void operator-=( Quantity< Tag > i_rOther )
  {
    m_Value -= i_rOther.m_Value;
  }

  /**
   * @brief Multiply-assign operator
   * @param i_rOther Multiplier
   */
  void operator*=( Quantity< Tag > i_rOther )
  {
    m_Value *= i_rOther.m_Value;
  }

  /**
   * @brief Divide-assign operator
   * @param i_rOther Divisor
   */
  void operator/=( Quantity< Tag > i_rOther )
  {
    m_Value /= i_rOther.m_Value;
  }
  ///@}

private:

  // Hidden friends: only considered in ADL. Fewer overloads means faster compile
  // This is not needed for == and <=>: as of c++20 they are reversible
  ///@name Arithmetic operators
  ///@{
  /**
   * @brief Subtraction operator
   * @param i_Left Minuend
   * @param i_Right Subtrahend
   * @return Difference
   */
  friend Quantity< Tag > operator-( Quantity< Tag > i_Left, Quantity< Tag > i_Right )
  {
    return Quantity< Tag >( i_Left.Value() - i_Right.Value() );
  }

  /**
   * @brief Addition operator
   * @param i_Left Addend
   * @param i_Right Addend
   * @return Sum
   */
  friend Quantity< Tag > operator+( Quantity< Tag > i_Left, Quantity< Tag > i_Right )
  {
    return Quantity< Tag >( i_Left.Value() + i_Right.Value() );
  }

  /**
   * @brief Multiplication operator
   * @param i_Left Multiplier
   * @param i_Right Multiplicand
   * @return Product
   */
  friend Quantity< Tag > operator*( Quantity< Tag > i_Left, Quantity< Tag > i_Right )
  {
    return Quantity< Tag >( i_Left.Value() * i_Right.Value() );
  }

  /**
   * @brief Division operator
   * @param i_Left Dividend
   * @param i_Right Divisor
   * @return Quotient
   */
  friend Quantity< Tag > operator/( Quantity< Tag > i_Left, Quantity< Tag > i_Right )
  {
    return Quantity< Tag >( i_Left.Value() / i_Right.Value() );
  }

  ///@}
  ///
  double m_Value = 0.0; ///< Value of the quantity
};

template< typename Tag >
void ThrowIfNegative( Quantity< Tag > i_Quantity, const std::string_view& i_rName );  ///< Throws a PreconditionError for a negative quantity

template< typename Tag >
void ThrowIfNotPositive( Quantity< Tag > i_Quantity, const std::string_view& i_rName );  ///< Throws a PreconditionError for a non-positive quantity

// Physical quantities
// @formatter:off
using Luminosity = Quantity< struct LuminosityTag >;  ///< Luminosity
using Mass = Quantity< struct MassTag >;  ///< Mass
using Metallicity = Quantity< struct MetallicityTag >;  ///< Metallicity
using Radius = Quantity< struct RadiusTag >;  ///< Radius
using Temperature = Quantity< struct TemperatureTag >;  ///< Temperature
using Age = Quantity< struct AgeTag >;  ///< Age
using AngularMomentum = Quantity< struct AngularMomentumTag >;  ///< Angular momentum
using AngularVelocity = Quantity< struct AngularVelocityTag >; ///< Angular velocity
// @formatter:on

// Extern template declarations
extern template class Quantity< struct LuminosityTag > ;
extern template class Quantity< struct MassTag > ;
extern template class Quantity< struct MetallicityTag > ;
extern template class Quantity< struct RadiusTag > ;
extern template class Quantity< struct TemperatureTag > ;
extern template class Quantity< struct AgeTag > ;
extern template class Quantity< struct AngularMomentumTag > ;
extern template class Quantity< struct AngularVelocityTag > ;

/**
 * @tparam Tag Quantity tag
 * @param i_Quantity Quantity under test
 * @param i_rName Name of the quantity, for the exception message
 * @throws PreconditionError If \c i_Quantity<0
 */
template< typename Tag >
void ThrowIfNegative( Quantity< Tag > i_Quantity, const std::string_view& i_rName )
{
  if( i_Quantity < 0 )
  {
    [[unlikely]] Herd::Exceptions::ThrowPreconditionError( i_rName, ">=0", i_Quantity.Value() );
  }
}

/**
 * @tparam Tag Quantity tag
 * @param i_Quantity Quantity under test
 * @param i_rName Name of the quantity, for the exception message
 * @throws PreconditionError If \c i_Quantity<=0
 */
template< typename Tag >
void ThrowIfNotPositive( Quantity< Tag > i_Quantity, const std::string_view& i_rName )
{
  if( i_Quantity <= 0 )
  {
    [[unlikely]] Herd::Exceptions::ThrowPreconditionError( i_rName, ">0", i_Quantity.Value() );
  }
}

}

#endif /* HD075DEEE_908E_40CA_A6A7_83108CC172DD */
