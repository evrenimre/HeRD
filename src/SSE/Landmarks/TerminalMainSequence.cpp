/**
 * @file TerminalMainSequence.cpp
 * @author Evren Imre
 * @date 30 Oct 2023	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2023 Evren Imre
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "TerminalMainSequence.h"

#include "BaseOfGiantBranch.h"
#include "ZeroAgeMainSequence.h"

#include <Exceptions/PreconditionError.h>
#include <Generic/MathHelpers.h>
#include <SSE/Constants.h>

#include <algorithm>
#include <cmath>
#include <type_traits>

#include <range/v3/algorithm.hpp>

namespace
{

// @formatter:off

const std::array< double, 30 > s_ZLTMS { 1.031538e+00, -2.434480e-01, 7.732821e+00, 6.460705e+00, 1.374484e+00,
  1.043715e+00, -1.577474e+00, -5.168234e+00, -5.596506e+00, -1.299394e+00,
  7.859573e+02, -8.542048e+00, -2.642511e+01, -9.585707e+00, 0.,
  3.858911e+03, 2.459681e+03, -7.630093e+01, -3.486057e+02, -4.861703e+01,
  2.888720e+02, 2.952979e+02, 1.850341e+02, 3.797254e+01, 0.,
  7.196580e+00, 5.613746e-01, 3.805871e-01, 8.398728e-02, 0.
};  ///< Coefficients for \f$ L_{TMS}(z) \f$ calculations


const std::array< double, 50 > s_ZRTMS { 2.187715e-01, -2.154437e+00, -3.768678e+00, -1.975518e+00, -3.021475e-01,
  1.466440e+00, 1.839725e+00, 6.442199e+00, 4.023635e+00, 6.957529e-01,
  2.652091e+01, 8.178458e+01, 1.156058e+02, 7.633811e+01, 1.950698e+01,
  1.472103e+00, -2.947609e+00, -3.312828e+00, -9.945065e-01, 0.,
  3.071048e+00, -5.679941e+00, -9.745523e+00, -3.594543e+00, 0.,
  -8.672073e-02, 0., 0., 0., 0.,
  2.617890e+00, 1.019135e+00, -3.292551e-02, -7.445123e-02, 0.,
  1.075567e-02, 1.773287e-02, 9.610479e-03, 1.732469e-03, 0.,
  1.476246e+00, 1.899331e+00, 1.195010e+00, 3.035051e-01, 0.,
  5.502535e+00, -6.601663e-02, 9.968707e-02, 3.599801e-02, 0.
};  ///< Coefficients for \f$ R_{TMS}(z) \f$ calculations

const std::array< double, 20 > s_ZThook { 1.949814e+01, 1.758178e+00, -6.008212e+00, -4.470533e+00,
  4.903830e+00, 0., 0., 0.,
  5.212154e-02, 3.166411e-02, -2.750074e-03, -2.271549e-03,
  1.312179e+00, -3.294936e-01, 9.231860e-02, 2.610989e-02,
  8.073972e-01, 0., 0., 0.
};  ///< Coefficients for \f$ t_{hook}(z) \f$ calculations

// @formatter:on

/**
 * @brief Updates a cache entry
 * @tparam TValue Type of the quantity being computed
 * @tparam TCallable Computing function
 * @param[in, out] i_rCache Current cached value. Updated
 * @param i_Mass Mass
 * @param i_Computer Computing function. Passed by reference as it carries captured data
 * @return Computed value
 * @pre \c TCallable can be called with an argument of type \c Mass and return a value
 */
template< class TValue, class TCallable >
auto UpdateCache( std::pair< std::optional< Herd::Generic::Mass >, TValue >& io_rCache, Herd::Generic::Mass i_Mass, const TCallable& i_Computer )
{
  static_assert( std::is_invocable_v< TCallable, Herd::Generic::Mass >);
  static_assert( !std::is_same_v< std::invoke_result<TCallable, Herd::Generic::Mass>, void> );

  auto& [ rKey, rValue ] = io_rCache;

  if( !rKey || *rKey != i_Mass )
  {
    rKey = i_Mass;
    rValue = i_Computer( i_Mass );
  }

  return rValue;
}
}

namespace Herd::SSE
{

/**
 * @param i_Z Metallicity
 * @pre \c \c i_Z is positive
 * @throws PreconditionError If the precondition is violated
 */
TerminalMainSequence::TerminalMainSequence( Herd::Generic::Metallicity i_Z )
{
  Herd::Generic::ThrowIfNotPositive( i_Z, "i_Z" );

  ComputeMetallicityDependents( i_Z );
}

/**
 * @remarks Without a user-defined destructor forward declaration and unique_ptr do not work together
 */
TerminalMainSequence::~TerminalMainSequence() = default;

/**
 * @param i_Mass Mass
 * @returns \f$ t_{MS}\f$
 * @pre \c i_Mass is positive
 * @throws PreconditionError If the precondition is violated
 */
Herd::Generic::Time TerminalMainSequence::Age( Herd::Generic::Mass i_Mass )
{
  Herd::Generic::ThrowIfNotPositive( i_Mass, "i_Mass" );

  // @formatter:off
  return UpdateCache( m_MDependents.m_Age, i_Mass, [ & ]( auto i_Mass ){ return ComputeAge(i_Mass);} ); // @suppress("Invalid arguments")
      // @formatter:on
}

/**
   * @param i_Mass Mass
   * @returns \f$ L_{TMS}\f$
   * @pre \c i_Mass is positive
   * @throws PreconditionError If the precondition is violated
 */
Herd::Generic::Luminosity TerminalMainSequence::Luminosity( Herd::Generic::Mass i_Mass )
{
  Herd::Generic::ThrowIfNotPositive( i_Mass, "i_Mass" );

  // @formatter:off
  return UpdateCache( m_MDependents.m_Luminosity, i_Mass, [ & ]( auto i_Mass ){ return ComputeLuminosity(i_Mass);} ); // @suppress("Invalid arguments")
        // @formatter:on
}

/**
   * @param i_Mass Mass
   * @returns \f$ R_{TMS}\f$
   * @pre \c i_Mass is positive
   * @throws PreconditionError If the precondition is violated
 */
Herd::Generic::Radius TerminalMainSequence::Radius( Herd::Generic::Mass i_Mass )
{
  Herd::Generic::ThrowIfNotPositive( i_Mass, "i_Mass" );

  // @formatter:off
  return UpdateCache( m_MDependents.m_Radius, i_Mass, [ & ]( auto i_Mass ){ return ComputeRadius( i_Mass); } ); // @suppress("Invalid arguments")
        // @formatter:on
}

/**
   * @param i_Mass Mass
   * @returns \f$ t_{hook}\f$
   * @pre \c i_Mass is positive
   * @throws PreconditionError If the precondition is violated
 */
Herd::Generic::Time TerminalMainSequence::THook( Herd::Generic::Mass i_Mass )
{
  Herd::Generic::ThrowIfNotPositive( i_Mass, "i_Mass" );

  // @formatter:off
  return UpdateCache( m_MDependents.m_THook, i_Mass, [ & ]( auto i_Mass ){ return ComputeTHook( i_Mass); } ); // @suppress("Invalid arguments")
        // @formatter:on
}

/**
 * @param i_Z Metallicity
 */
void TerminalMainSequence::ComputeMetallicityDependents( Herd::Generic::Metallicity i_Z )
{
  Herd::Generic::Metallicity relativeZ( i_Z / Herd::SSE::Constants::s_SolarMetallicityTout96 ); // Metallicity relative to the Sun

  std::array< double, 5 > zetaPowers4;
  double zeta = std::log10( relativeZ );
  Herd::SSE::ComputePowers( zetaPowers4, zeta );

  std::array< double, 4 > zetaPowers3;
  ranges::cpp20::copy_n( zetaPowers4.begin(), 4, zetaPowers3.begin() );

  // Landmarks
  m_ZDependents.m_pZAMSComputer = std::make_unique< Herd::SSE::ZeroAgeMainSequence >( i_Z );
  m_ZDependents.m_pBGBComputer = std::make_unique< Herd::SSE::BaseOfGiantBranch >( i_Z );

  // LTMS
  Herd::SSE::MultiplyMatrixVector( m_ZDependents.m_LTMS, s_ZLTMS, zetaPowers4 );
  m_ZDependents.m_LTMS[ 0 ] *= m_ZDependents.m_LTMS[ 3 ];
  m_ZDependents.m_LTMS[ 1 ] *= m_ZDependents.m_LTMS[ 3 ];

  // RTMS
  std::array< double, 10 > tempRTMS;
  Herd::SSE::MultiplyMatrixVector( tempRTMS, s_ZRTMS, zetaPowers4 );
  ranges::cpp20::copy( tempRTMS, m_ZDependents.m_RTMS.begin() );
  m_ZDependents.m_RTMS[ 0 ] *= m_ZDependents.m_RTMS[ 2 ];
  m_ZDependents.m_RTMS[ 1 ] *= m_ZDependents.m_RTMS[ 2 ];

  double sigma = log10( i_Z );
  m_ZDependents.m_RTMS[ 10 ] = std::pow( 10., std::max( { 0.097 - 0.1072 * ( sigma + 3. ), 0.097, std::min( 0.1461, 0.1461 + 0.1237 * ( sigma + 2 ) ) } ) );

  auto& rA = m_ZDependents.m_RTMS;
  // Evaluated at RZAMS = 0. At this point we do not know RZAMS, but it is only used if the mass < rA[10]
  rA[ 11 ] = ComputeRadius( Herd::Generic::Mass( rA[ 10 ] ) ); // Eq. 9a, evaluated at a17
  rA[ 12 ] = ComputeRadius( Herd::Generic::Mass( rA[ 10 ] + 0.1 ) );  //Eq. 9b, evaluated at Mstar

  // Eq. 6, but AMUSE.SSE adds an extra term
  {
    double extra = std::min( 0.99, 0.98 - ( 100. / 7. ) * ( i_Z - 0.001 ) );
    double left = 0.95 - ( 10. / 3. ) * ( i_Z - 0.01 );
    m_ZDependents.m_X = std::max( { 0.95, left, extra } );

  }

  // ThookCoefficients
  Herd::SSE::MultiplyMatrixVector( m_ZDependents.m_Thook, s_ZThook, zetaPowers3 );
}
/**
 * @param i_Mass Mass
 * @return \f$ t_{MS} \f$
 */
Herd::Generic::Time TerminalMainSequence::ComputeAge( Herd::Generic::Mass i_Mass ) const
{
  Herd::Generic::Time tHook = ComputeTHook( i_Mass );
  Herd::Generic::Time tBGB = m_ZDependents.m_pBGBComputer->Age( i_Mass );
  return std::max( Herd::Generic::Time( m_ZDependents.m_X * tBGB ), tHook );
}

/**
 * @param i_Mass Mass
 * @return \f L_{TMS}\f$
 */
Herd::Generic::Luminosity TerminalMainSequence::ComputeLuminosity( Herd::Generic::Mass i_Mass ) const
{
  // Eq. 8
  auto& rA = m_ZDependents.m_LTMS;

  double m20 = i_Mass * i_Mass;
  double m30 = m20 * i_Mass;
  double m40 = m20 * m20;
  double m50 = m40 * i_Mass;

  double num = Herd::SSE::ComputeInnerProduct( { rA[ 0 ], rA[ 1 ] }, std::array< double, 2 > { m30, m40 } ) + rA[ 2 ] * std::pow( i_Mass, rA[ 5 ] + 1.8 );
  double den = Herd::SSE::ComputeInnerProduct( { rA[ 3 ], rA[ 4 ] }, std::array< double, 2 > { 1., m50 } ) + std::pow( i_Mass, rA[ 5 ] ); // @suppress("Invalid arguments")

  return Herd::Generic::Luminosity( num / den );
}

/**
 * @param i_Mass Mass
 * @return \f R_{TMS}\f$
 */
Herd::Generic::Radius TerminalMainSequence::ComputeRadius( Herd::Generic::Mass i_Mass ) const
{
  auto& rA = m_ZDependents.m_RTMS;

  if( i_Mass <= rA[ 10 ] )
  {
    // Eq. 9a
    double num = Herd::SSE::ApBXhC( i_Mass, rA[ 0 ], rA[ 1 ], rA[ 3 ] );
    double den = Herd::SSE::ApBXhC( i_Mass, rA[ 2 ], 1., rA[ 4 ] );
    Herd::Generic::Radius rZAMS = m_ZDependents.m_pZAMSComputer->Radius( i_Mass );
    return Herd::Generic::Radius( std::max( 1.5 * rZAMS, num / den ) ); // AMUSE.SSE added a check to ensure that RTMS > RZAMS
  }

  if( i_Mass >= rA[ 10 ] + 0.1 )
  {
    double m15 = i_Mass * std::sqrt( i_Mass );
    double m20 = i_Mass * i_Mass;
    double m30 = m20 * i_Mass;
    double m50 = m20 * m30;

    double num = rA[ 5 ] * m30 + std::pow( i_Mass, rA[ 9 ] ) * ( rA[ 6 ] + rA[ 7 ] * m15 );
    return Herd::Generic::Radius( num / ( rA[ 8 ] + m50 ) );
  }

  // Between rA[10] and rA[10] + 0.1, interpolate
  return Herd::Generic::Radius( std::lerp( rA[ 11 ], rA[ 12 ], Herd::SSE::ComputeBlendWeight( i_Mass, rA[ 10 ], rA[ 10 ] + 0.1 ) ) );
}

/**
 * @param i_Mass Mass
 * @return \f$ t_{hook}\f$
 */
Herd::Generic::Time TerminalMainSequence::ComputeTHook( Herd::Generic::Mass i_Mass ) const
{
  // Eq. 7
  double mu = 0;
  const auto& rA = m_ZDependents.m_Thook;
  double left = Herd::SSE::BXhC( i_Mass, rA[ 0 ], -rA[ 1 ] );
  double right = Herd::SSE::ApBXhC( i_Mass, rA[ 2 ], rA[ 3 ], -rA[ 4 ] );
  mu = std::max( 0.5, 1. - 0.01 * std::max( left, right ) );

  Herd::Generic::Time tBGB = m_ZDependents.m_pBGBComputer->Age( i_Mass );

  return Herd::Generic::Time( mu * tBGB );
}

}



