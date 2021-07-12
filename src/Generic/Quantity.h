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

#include <Exceptions/PreconditionError.h>

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
  }

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

  /**
   * @brief Equality operator
   * @param i_Other Other value
   * @return \c true if the contained values are equal
   */
  bool operator==( Quantity< Tag > i_Other ) const
  {
    return m_Value == i_Other.m_Value;
  }

private:

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
using AngularMomentum = Quantity< struct AngularVelocityTag >; ///< Angular velocity
// @formatter:on

// Extern template declarations
extern template class Quantity< struct LuminosityTag > ;
extern template class Quantity< struct MassTag > ;
extern template class Quantity< struct MetallicityTag > ;
extern template class Quantity< struct RadiusTag > ;
extern template class Quantity< struct TemperatureTag > ;
extern template class Quantity< struct AgeTag > ;
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
    [[unlikely]] throw( Exceptions::PreconditionError( i_rName, ">=0", i_Quantity.Value() ) );
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
    [[unlikely]] throw( Exceptions::PreconditionError( i_rName, ">0", i_Quantity.Value() ) );
  }
}

}

#endif /* HD075DEEE_908E_40CA_A6A7_83108CC172DD */
