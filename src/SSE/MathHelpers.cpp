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

namespace Herd::SSE
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
}
