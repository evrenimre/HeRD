/**
 * @file CriticalMassValues.cpp
 * @author Evren Imre
 * @date 8 Dec 2023	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2023 Evren Imre
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "CriticalMassValues.h"

#include "Constants.h"

#include <Exceptions/PreconditionError.h>
#include <Generic/MathHelpers.h>

#include <array>
#include <cmath>

namespace
{

/**
 * @brief Helper function for computing the powers of \f$ Z \f$
 * @param o_rPowers
 * @param i_Z
 */
void ComputeZetaPowers2( std::array< double, 3 > o_rPowers, Herd::Generic::Metallicity i_Z )
{
  Herd::Generic::Metallicity relativeZ( i_Z / Herd::SSE::Constants::s_SolarMetallicityTout96 ); // Metallicity relative to the Sun

  double zeta = std::log10( relativeZ );
  Herd::SSE::ComputePowers( o_rPowers, zeta );
}

}

namespace Herd::SSE
{

/**
 * @param i_Z Metallicity
 * @return \f$ M_{hook}\f$
 * @pre \c i_Z is positive
 * @throw \c PreconditionError if the precondition is violated
 */
Herd::Generic::Mass ComputeMhook( Herd::Generic::Metallicity i_Z )
{
  Herd::Generic::ThrowIfNotPositive( i_Z, "i_Z" );

  std::array< double, 3 > zetaPowers2;
  ComputeZetaPowers2( zetaPowers2, i_Z );
  return Herd::Generic::Mass( Herd::SSE::ComputeInnerProduct( { 1.0185, 0.16015, 0.0892 }, zetaPowers2 ) );  // Eq. 1
}

/**
 * @param i_Z Metallicity
 * @return \f$ M_{HeF}\f$
 * @pre \c i_Z is positive
 * @throw \c PreconditionError if the precondition is violated
 */
Herd::Generic::Mass ComputeMHeF( Herd::Generic::Metallicity i_Z )
{
  Herd::Generic::ThrowIfNotPositive( i_Z, "i_Z" );

  std::array< double, 3 > zetaPowers2;
  ComputeZetaPowers2( zetaPowers2, i_Z );
  return Herd::Generic::Mass( Herd::SSE::ComputeInnerProduct( { 1.995, 0.25, 0.087 }, zetaPowers2 ) );  // Eq. 2);
}

/**
 * @param i_Z Metallicity
 * @return \f$ M_{FGB}\f$
 * @pre \c i_Z is positive
 * @throw \c PreconditionError if the precondition is violated
 */
Herd::Generic::Mass ComputeMFGB( Herd::Generic::Metallicity i_Z )
{
  Herd::Generic::ThrowIfNotPositive( i_Z, "i_Z" );

  Herd::Generic::Metallicity relativeZ( i_Z / Herd::SSE::Constants::s_SolarMetallicityTout96 ); // Metallicity relative to the Sun

  // Eq. 3, but 0.0012 replaced by 1e-4 in AMUSE.SSE
  double num = Herd::SSE::BXhC( relativeZ, 13.048, 0.06 );
  double den = Herd::SSE::ApBXhC( relativeZ, 1., 1e-4, -1.27 );
  return Herd::Generic::Mass( num / den );
}
}


