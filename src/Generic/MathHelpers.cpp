/**
 * @file MathHelpers.cpp
 * @author Evren Imre
 * @date 30 Aug 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "MathHelpers.h"

#include <cmath>

namespace Herd::Generic
{
/**
 * @param i_X Variable
 * @param i_A a
 * @param i_B b
 * @return \f$ \frac{ x-a }{b-a}\f$
 */
double ComputeBlendWeight( double i_X, double i_A, double i_B )
{
  return ( i_X - i_A ) / ( i_B - i_A );
}

/**
 * @param i_X Variable
 * @param i_A Addend
 * @param i_B Scale
 * @param i_C Exponent
 * @return \f$ a + bx^c \f$
 */
double ApBXhC( double i_X, double i_A, double i_B, double i_C )
{
  return std::fma( i_B, std::pow( i_X, i_C ), i_A );
}

/**
 * @param i_X Variable
 * @param i_B Scale
 * @param i_C Exponent
 * @return \f$ bx^c \f$
 */
double BXhC( double i_X, double i_B, double i_C )
{
  return i_B * std::pow( i_X, i_C );
}

}
