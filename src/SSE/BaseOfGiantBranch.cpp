/**
 * @file BaseOfGiantBranch.cpp
 * @author Evren Imre
 * @date 28 Oct 2023	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2023 Evren Imre
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "BaseOfGiantBranch.h"

#include "Constants.h"

#include <Exceptions/PreconditionError.h>
#include <Generic/MathHelpers.h>

#include <range/v3/algorithm.hpp>

namespace
{
// @formatter:off

const std::array< double, 20 > s_ZTBGB { 1.593890e+03, 2.053038e+03, 1.231226e+03, 2.327785e+02,
  2.706708e+03, 1.483131e+03, 5.772723e+02, 7.411230e+01,
  1.466143e+02, -1.048442e+02, -6.795374e+01, -1.391127e+01,
  4.141960e-02, 4.564888e-02, 2.958542e-02, 5.571483e-03,
  3.426349e-01, 0., 0., 0.
};  ///< Coefficients for \f${ T_{BGB}(z) \f$ calculations


const std::array< double, 24 > s_ZLBGB { 9.511033e+01, 6.819618e+01, -1.045625e+01, -1.474939e+01,
  3.113458e+01, 1.012033e+01, -4.650511e+00, -2.463185e+00,
    1.413057e+00, 4.578814e-01, -6.850581e-02, -5.588658e-02,
    3.910862e+01, 5.196646e+01, 2.264970e+01, 2.873680e+00,
    4.597479e+00, -2.855179e-01, 2.709724e-01, 0.,
    6.682518e+00, 2.827718e-01, -7.294429e-02, 0.
};  ///< Coefficients for \f${ L_{BGB}(z) \f$ calculations

// @formatter:on
}
namespace Herd::SSE
{

using Herd::SSE::ApBXhC;
using Herd::SSE::BXhC;

/**
 * @param i_Z Metallicity
 * @pre \c \c i_Z is positive
 * @throws PreconditionError If the precondition is violated
 */
BaseOfGiantBranch::BaseOfGiantBranch( Herd::Generic::Metallicity i_Z )
{
  Herd::Generic::ThrowIfNotPositive( i_Z, "i_Z" );

  ComputeMetallicityDependents( i_Z );
}

/**
 * @param i_Mass Mass
 * @pre \c i_Mass is positive
 * @throws PreconditionError If the precondition is violated
 */
void BaseOfGiantBranch::Compute( Herd::Generic::Mass i_Mass )
{
  Herd::Generic::ThrowIfNotPositive( i_Mass, "i_Mass" );

  ComputeMassDependents( i_Mass );
}

/**
 * @param i_Z Metallicity
 */
void BaseOfGiantBranch::ComputeMetallicityDependents( Herd::Generic::Metallicity i_Z )
{
  Herd::Generic::Metallicity relativeZ( i_Z / Herd::SSE::Constants::s_SolarMetallicityTout96 ); // Metallicity relative to the Sun

  std::array< double, 4 > zetaPowers3;
  double zeta = std::log10( relativeZ );
  Herd::SSE::ComputePowers( zetaPowers3, zeta );

  // TBGB
  Herd::SSE::MultiplyMatrixVector( m_ZDependents.m_TBGB, s_ZTBGB, zetaPowers3 );

  // LBGB
  std::array< double, 6 > tempLBGB;
  Herd::SSE::MultiplyMatrixVector( tempLBGB, s_ZLBGB, zetaPowers3 );
  ranges::cpp20::copy( tempLBGB, m_ZDependents.m_LBGB.begin() );

  {
    auto& rB = m_ZDependents.m_LBGB;
    rB[ 2 ] = std::pow( rB[ 2 ], rB[ 5 ] );
    rB[ 6 ] = 4.637345e+00;
    rB[ 7 ] = 9.301992e+00;
  }
}

/**
 * @return Age at BGB
 */
Herd::Generic::Time BaseOfGiantBranch::TBGB() const
{
  return m_MDependents.m_TBGB;
}

/**
 * @return Luminosity at BGB
 */
Herd::Generic::Luminosity BaseOfGiantBranch::LBGB() const
{
  return m_MDependents.m_LBGB;
}

/**
 * @param i_Mass Mass
 */
void BaseOfGiantBranch::ComputeMassDependents( Herd::Generic::Mass i_Mass )
{
  if( i_Mass != m_MDependents.m_EvaluatedAt )
  {
    m_MDependents.m_EvaluatedAt = i_Mass;
    m_MDependents.m_TBGB = ComputeTBGB( i_Mass );
    m_MDependents.m_LBGB = ComputeLBGB( i_Mass );
  }
}

/**
 * @param i_Mass Mass
 * @return \f$ L_{BGB}\f$
 */
Herd::Generic::Luminosity BaseOfGiantBranch::ComputeLBGB( Herd::Generic::Mass i_Mass ) const
{
  auto& rB = m_ZDependents.m_LBGB;

  // Eq. 10
  double num = BXhC( i_Mass, rB[ 0 ], rB[ 4 ] ) + BXhC( i_Mass, rB[ 1 ], rB[ 7 ] );
  double den = ApBXhC( i_Mass, rB[ 2 ], rB[ 3 ], rB[ 6 ] ) + std::pow( i_Mass, rB[ 5 ] );
  return Herd::Generic::Luminosity( num / den );
}

/**
 * @param i_Mass Mass
 * @return Age at BGB
 */
Herd::Generic::Time BaseOfGiantBranch::ComputeTBGB( Herd::Generic::Mass i_Mass ) const
{
  // Eq. 4
  const auto& rA = m_ZDependents.m_TBGB;

  double m05 = std::sqrt( i_Mass );
  double m20 = i_Mass * i_Mass;
  double m50 = m20 * m20 * i_Mass;

  std::array< double, 4 > massPowersNum;
  massPowersNum[ 0 ] = 1;
  massPowersNum[ 1 ] = m20 * m20; // m^4
  massPowersNum[ 2 ] = m50 * m05; // m^5.5
  massPowersNum[ 3 ] = m50 * m20;  // m^7

  std::array< double, 4 > massPowersDen;
  massPowersDen[ 0 ] = m20;
  massPowersDen[ 1 ] = massPowersNum[ 3 ]; // m^7

  double num = Herd::SSE::ComputeInnerProduct( { rA[ 0 ], rA[ 1 ], rA[ 2 ], 1. }, massPowersNum );
  double den = Herd::SSE::ComputeInnerProduct( { rA[ 3 ], rA[ 4 ] }, massPowersDen );

  return Herd::Generic::Time( num / den );
}

}


