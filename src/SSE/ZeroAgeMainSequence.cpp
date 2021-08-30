/**
 * @file ZeroAgeMainSequence.cpp
 * @author Evren Imre
 * @date 4 Nov 2020	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "ZeroAgeMainSequence.h"

#include "Constants.h"
#include "MathHelpers.h"

#include <Generic/Quantity.h>
#include <Generic/QuantityRange.h>
#include <Physics/LuminosityRadiusTemperature.h>

#include <algorithm>
#include <cmath>
#include <cstddef>

namespace
{
// @formatter:off
const std::array<double, 35 > s_ZL { 3.970417e-01, -3.2913574e-01, 3.4776688e-01, 3.7470851e-01, 9.011915e-02,
    8.527626e+00, -2.441225973e+01, 5.643597107e+01, 3.706152575e+01, 5.4562406e+00,
    2.5546e-04, -1.23461e-03, -2.3246e-04, 4.5519e-04, 1.6176e-04,
    5.432889e+00, -8.62157806e+00, 1.344202049e+01, 1.451584135e+01, 3.39793084e+00,
    5.563579e+00, -1.032345224e+01, 1.944322980e+01, 1.897361347e+01, 4.16903097e+00,
    7.8866060e-01, -2.90870942e+00, 6.54713531e+00, 4.05606657e+00, 5.3287322e-01,
    5.86685e-03, -1.704237e-02, 3.872348e-02, 2.570041e-02, 3.83376e-03
};  ///< Coefficients for \f$ L_{ZAMS}(z) \f$

const std::array< double, 45 > s_ZR { 1.715359e+00, 6.2246212e-01, -9.2557761e-01, -1.16996966e+00, -3.0631491e-01,
    6.597788e+00, -4.2450044e-01, -1.213339427e+01, -1.073509484e+01, -2.51487077e+00,
    1.008855000e+01, -7.11727086e+00, -3.167119479e+01, -2.424848322e+01, -5.33608972e+00,
    1.012495e+00, 3.2699690e-01, -9.23418e-03, -3.876858e-02, -4.12750e-03,
    7.490166e-02, 2.410413e-02, 7.233664e-02, 3.040467e-02, 1.97741e-03,
    1.077422e-02, 0, 0, 0, 0,
    3.082234e+00, 9.447205e-01, -2.15200882e+00, -2.49219496e+00, -6.3848738e-01,
    1.784778e+01, -7.4534569e+00, -4.896066856e+01, -4.005386135e+01, -9.09331816e+00,
    2.2582e-04, -1.86899e-03, 3.88783e-03, 1.42402e-03, -7.671e-05
};  ///< Coefficients for \f$ R_{ZAMS}(z) \f$
// @formatter:on

}

namespace Herd::SSE
{

struct ZeroAgeMainSequenceAlgorithmSpecs;

/**
 * @param i_Z Initial metallicity
 * @pre \c i_Z is within the permissible range
 * @throws PreconditionError If the precondition is violated
 */
ZeroAgeMainSequence::ZeroAgeMainSequence( Herd::Generic::Metallicity i_Z )
{
  ZeroAgeMainSequenceSpecs::s_ZRange.ThrowIfNotInRange( i_Z, "i_Z" );  // Metallicity is within the allowed range
  ComputeMetallicityDependents( i_Z );
}

/**
 * @param i_Mass Mass
 * @return Track point with radius, luminosity and temperature at ZAMS
 * @pre \c i_Mass is within the permissible range
 * @throws PreconditionError If the precondition is violated
 */
Herd::SSE::TrackPoint ZeroAgeMainSequence::Compute( Herd::Generic::Mass i_Mass )
{
  ZeroAgeMainSequenceSpecs::s_MassRange.ThrowIfNotInRange( i_Mass, "i_Mass" );  // Mass is within the allowed range

  if( i_Mass != m_MDependents.m_EvaluatedAt )
  {
    ComputeMassDependents( i_Mass );
  }

  Herd::SSE::TrackPoint output;
  output.m_Age.Set( 0 );
  output.m_Luminosity = m_MDependents.m_LZAMS;
  output.m_Radius = m_MDependents.m_RZAMS;
  output.m_Mass = i_Mass;
  output.m_InitialMetallicity = m_ZDependents.m_EvaluatedAt;
  output.m_Temperature = m_MDependents.m_TZAMS;
  output.m_Stage = Herd::SSE::EvolutionStage::e_Undefined;

  return output;
}

/**
 * @param i_Z Metallicity
 */
void ZeroAgeMainSequence::ComputeMetallicityDependents( Herd::Generic::Metallicity i_Z )
{
  m_ZDependents.m_EvaluatedAt = i_Z;

  std::array< double, 5 > zetaPowers;
  double logZeta = log10( i_Z / Herd::SSE::Constants::s_SolarMetallicityTout96 );
  Herd::SSE::ComputePowers( zetaPowers, logZeta );

  // Eq. 3
  Herd::SSE::MultiplyMatrixVector( m_ZDependents.m_LZAMS, s_ZL, zetaPowers );
  Herd::SSE::MultiplyMatrixVector( m_ZDependents.m_RZAMS, s_ZR, zetaPowers );
}

/**
 * @param i_Mass Mass
 */
void ZeroAgeMainSequence::ComputeMassDependents( Herd::Generic::Mass i_Mass )
{
  m_MDependents.m_LZAMS = ComputeLuminosity( i_Mass );
  m_MDependents.m_RZAMS = ComputeRadius( i_Mass );
  m_MDependents.m_TZAMS = Herd::Physics::ComputeAbsoluteTemperature( m_MDependents.m_LZAMS, m_MDependents.m_RZAMS );
}

/**
 * @param i_Mass Mass
 * @return \f$ L_{ZAMS} \f$
 */
Herd::Generic::Luminosity ZeroAgeMainSequence::ComputeLuminosity( Herd::Generic::Mass i_Mass ) const
{
  auto& rA = m_ZDependents.m_LZAMS;

  // Powers of mass
  std::array< double, 6 > massPowers;

  double m05 = std::sqrt( i_Mass );
  double m20 = i_Mass * i_Mass;
  massPowers[ 0 ] = 1;
  massPowers[ 1 ] = m20 * i_Mass;  // m^3
  massPowers[ 2 ] = massPowers[ 1 ] * m20;  // m^5
  massPowers[ 3 ] = massPowers[ 2 ] * m20;  // m^7
  massPowers[ 4 ] = massPowers[ 3 ] * i_Mass; // m^8
  massPowers[ 5 ] = massPowers[ 4 ] * i_Mass * m05; // m^9.5

  double m55 = massPowers[ 2 ] * m05; // m5.5

  // Eq. 1
  double num = m55 * ( rA[ 0 ] + m55 * rA[ 1 ] );
  double den = Herd::SSE::ComputeInnerProduct( { rA[ 2 ], 1., rA[ 3 ], rA[ 4 ], rA[ 5 ], rA[ 6 ] }, massPowers );

  return Herd::Generic::Luminosity( num / den );
}

/**
 * @param i_Mass Mass
 * @return \f$ R_{ZAMS}\f$
 */
Herd::Generic::Radius ZeroAgeMainSequence::ComputeRadius( Herd::Generic::Mass i_Mass ) const
{
  auto& rA = m_ZDependents.m_RZAMS;

  // Powers of mass
  double m05 = std::sqrt( i_Mass );
  double m20 = i_Mass * i_Mass;
  double m30 = m20 * i_Mass;
  double m50 = m30 * m20;

  std::array< double, 5 > massPowersNum;
  massPowersNum[ 0 ] = m20 * m05; // m^2.5
  massPowersNum[ 1 ] = m30 * m30 * m05; // m^6.5
  massPowersNum[ 2 ] = m30 * m30 * m50; // m^11
  massPowersNum[ 3 ] = massPowersNum[ 1 ] * massPowersNum[ 1 ] * m50 * i_Mass; // m^19
  massPowersNum[ 4 ] = massPowersNum[ 3 ] * m05;  // m^19.5

  std::array< double, 5 > massPowersDen;
  massPowersDen[ 0 ] = 1;
  massPowersDen[ 1 ] = m20;
  massPowersDen[ 2 ] = massPowersNum[ 1 ] * m20;  // m^8.5
  massPowersDen[ 3 ] = massPowersNum[ 2 ] * massPowersNum[ 1 ] * i_Mass; // m^18.5
  massPowersDen[ 4 ] = massPowersNum[ 4 ];  // m^19.5

  // Eq. 2
  double num = Herd::SSE::ComputeInnerProduct( { rA[ 0 ], rA[ 1 ], rA[ 2 ], rA[ 3 ], rA[ 4 ] }, massPowersNum );
  double den = Herd::SSE::ComputeInnerProduct( { rA[ 5 ], rA[ 6 ], rA[ 7 ], 1., rA[ 8 ] }, massPowersDen );

  return Herd::Generic::Radius( num / den );
}

} // namespace Herd::SSE

