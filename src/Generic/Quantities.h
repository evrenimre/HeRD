/**
 * @file Quantities.h
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

// Physical quantities
// @formatter:off
using Luminosity = Quantity< struct LuminosityTag >;  ///< Luminosity
using Mass = Quantity< struct MassTag >;  ///< Mass
using Metallicity = Quantity< struct MetallicityTag >;  ///< Metallicity
using Radius = Quantity< struct RadiusTag >;  ///< Radius
using Temperature = Quantity< struct TemperatureTag >;  ///< Temperature
using Age = Quantity< struct AgeTag >;  ///< Age
// @formatter:on

}

#endif /* HD075DEEE_908E_40CA_A6A7_83108CC172DD */
