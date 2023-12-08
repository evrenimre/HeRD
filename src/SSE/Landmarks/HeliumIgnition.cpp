/**
 * @file HeliumIgnition.cpp
 * @author Evren Imre
 * @date 28 Oct 2023	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2023 Evren Imre
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "HeliumIgnition.h"

#include "Constants.h"
#include "Utilities.h"

#include <Exceptions/PreconditionError.h>
#include <Generic/MathHelpers.h>

#include <cmath>

namespace
{

// @formatter:off
const std::array< double, 15 > s_ZLHeI { 2.751631e+03, 3.557098e+02, 0.,
  -3.820831e-02, 5.872664e-02, 0.,
  1.071738e+02, -8.970339e+01, -3.949739e+01,
  7.348793e+02, -1.531020e+02, -3.793700e+01,
  9.219293e+00, -2.005865e+00, -5.561309e-01
};  ///< Coefficients for \f$ L_{HeI} \f$ calculations

// @formatter:on
}

namespace Herd::SSE
{

/**
 * @param i_Z Metallicity
 * @pre \c i_Z is positive
 * @throws PreconditionError If the precondition is violated
 */
HeliumIgnition::HeliumIgnition( Herd::Generic::Metallicity i_Z )
{
  Herd::Generic::ThrowIfNotPositive( i_Z, "i_Z" );

  ComputeMetallicityDependents( i_Z );
}

/**
 * @param i_Mass Mass
 * @returns \f$ t_{HeI}\f$
 * @pre \c i_Mass is positive
 * @throws PreconditionError If the precondition is violated
 */
Herd::Generic::Time HeliumIgnition::Age( Herd::Generic::Mass i_Mass )
{
  Herd::Generic::ThrowIfNotPositive( i_Mass, "i_Mass" );

  // @formatter:off
  return Herd::SSE::UpdateCache( m_MDependents.m_Age, i_Mass, [ & ]( auto i_Mass ){ return ComputeAge(i_Mass);} ); // @suppress("Invalid arguments")
    // @formatter:on
}

/**
   * @param i_Mass Mass
   * @returns \f$ L_{HeI}\f$
 * @pre \c i_Mass is positive
 * @throws PreconditionError If the precondition is violated
 */
Herd::Generic::Luminosity HeliumIgnition::Luminosity( Herd::Generic::Mass i_Mass )
{
  Herd::Generic::ThrowIfNotPositive( i_Mass, "i_Mass" );

  // @formatter:off
  return Herd::SSE::UpdateCache( m_MDependents.m_Luminosity, i_Mass, [ & ]( auto i_Mass ){ return ComputeLuminosity(i_Mass);} ); // @suppress("Invalid arguments")
          // @formatter:on
}

/**
   * @param i_Mass Mass
   * @returns \f$ R_{HeI}\f$
   * @pre \c i_Mass is positive
   * @throws PreconditionError If the precondition is violated
   */
Herd::Generic::Radius HeliumIgnition::Radius( Herd::Generic::Mass i_Mass )
{
  Herd::Generic::ThrowIfNotPositive( i_Mass, "i_Mass" );

  // @formatter:off
  return Herd::SSE::UpdateCache( m_MDependents.m_Radius, i_Mass, [ & ]( auto i_Mass ){ return ComputeRadius( i_Mass); } ); // @suppress("Invalid arguments")
        // @formatter:on
}

/**
   * @param i_Z Metallicity
   */
void HeliumIgnition::ComputeMetallicityDependents( Herd::Generic::Metallicity i_Z )
{
  Herd::Generic::Metallicity relativeZ( i_Z / Herd::SSE::Constants::s_SolarMetallicityTout96 ); // Metallicity relative to the Sun

  std::array< double, 3 > zetaPowers2;
  double zeta = std::log10( relativeZ );
  Herd::SSE::ComputePowers( zetaPowers2, zeta );

  // MHeF
  m_ZDependents.m_MHeF.Set( Herd::SSE::ComputeInnerProduct( { 1.995, 0.25, 0.087 }, zetaPowers2 ) );  // Eq. 2

  // LHeI
  std::array< double, 5 > tempLHeI;
  Herd::SSE::MultiplyMatrixVector( tempLHeI, s_ZLHeI, zetaPowers2 );

  {
    auto& rB = m_ZDependents.m_LHeI;
    rB[ 0 ] = tempLHeI[ 0 ];
    rB[ 1 ] = tempLHeI[ 1 ];
    rB[ 2 ] = 15.;

    rB[ 4 ] = tempLHeI[ 2 ];
    rB[ 5 ] = tempLHeI[ 3 ] * tempLHeI[ 3 ];
    rB[ 6 ] = tempLHeI[ 4 ] * tempLHeI[ 4 ];

    rB[ 3 ] = 0.; // This value is just an initialiser. The next call goes down a branch that does not use rB[3]
    Herd::Generic::Luminosity lHeI = ComputeLuminosity( m_ZDependents.m_MHeF );  // L_HeI at M_HeF
    rB[ 3 ] = ( Herd::SSE::BXhC( m_ZDependents.m_MHeF, rB[ 0 ], rB[ 1 ] ) - lHeI ) / ( lHeI * std::exp( m_ZDependents.m_MHeF * rB[ 2 ] ) ); // AMASS.SSE implements this differently from the paper
  }
}

/**
 * @param i_Mass Mass
 * @return Age at HeI
 */
Herd::Generic::Time HeliumIgnition::ComputeAge( Herd::Generic::Mass i_Mass ) const
{
  return Herd::Generic::Time( i_Mass ); // Placeholder
}

/**
 * @param i_Mass Mass
 * @return \f$ L_{HeI}\f$
 */
Herd::Generic::Luminosity HeliumIgnition::ComputeLuminosity( Herd::Generic::Mass i_Mass ) const
{
  auto& rB = m_ZDependents.m_LHeI;

  // Eq. 49
  if( i_Mass < m_ZDependents.m_MHeF )
  {
    double num = BXhC( i_Mass, rB[ 0 ], rB[ 1 ] );
    double den = 1. + rB[ 3 ] * std::exp( i_Mass * rB[ 2 ] ); // AMASS.SSE implementation: MHeF is not subtracted
    return Herd::Generic::Luminosity( num / den );
  }

  double num = ApBXhC( i_Mass, rB[ 4 ], rB[ 5 ], 3.8 );
  double den = rB[ 6 ] + i_Mass * i_Mass;
  return Herd::Generic::Luminosity( num / den );
}

/**
 * @param i_Mass Mass
 * @return \f$ R_{BGB}\f$
 */
Herd::Generic::Radius HeliumIgnition::ComputeRadius( Herd::Generic::Mass i_Mass ) const
{
  return Herd::Generic::Radius( i_Mass ); // Placeholder
}

}


