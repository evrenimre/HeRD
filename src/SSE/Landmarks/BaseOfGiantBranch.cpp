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

#include <Exceptions/PreconditionError.h>
#include <Generic/MathHelpers.h>
#include <SSE/Constants.h>
#include <SSE/GiantBranchRadius.h>

#include <cmath>
#include <type_traits>

#include <range/v3/algorithm.hpp>

namespace
{
// @formatter:off

const std::array< double, 20 > s_ZTBGB { 1.593890e+03, 2.053038e+03, 1.231226e+03, 2.327785e+02,
  2.706708e+03, 1.483131e+03, 5.772723e+02, 7.411230e+01,
  1.466143e+02, -1.048442e+02, -6.795374e+01, -1.391127e+01,
  4.141960e-02, 4.564888e-02, 2.958542e-02, 5.571483e-03,
  3.426349e-01, 0., 0., 0.
};  ///< Coefficients for \f${ t_{BGB}(z) \f$ calculations


const std::array< double, 24 > s_ZLBGB { 9.511033e+01, 6.819618e+01, -1.045625e+01, -1.474939e+01,
  3.113458e+01, 1.012033e+01, -4.650511e+00, -2.463185e+00,
    1.413057e+00, 4.578814e-01, -6.850581e-02, -5.588658e-02,
    3.910862e+01, 5.196646e+01, 2.264970e+01, 2.873680e+00,
    4.597479e+00, -2.855179e-01, 2.709724e-01, 0.,
    6.682518e+00, 2.827718e-01, -7.294429e-02, 0.
};  ///< Coefficients for \f${ L_{BGB}(z) \f$ calculations

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
BaseOfGiantBranch::BaseOfGiantBranch( Herd::Generic::Metallicity i_Z )
{
  Herd::Generic::ThrowIfNotPositive( i_Z, "i_Z" );

  ComputeMetallicityDependents( i_Z );
}

/**
 * @remarks Without a user-defined destructor forward declaration and unique_ptr do not work together
 */
BaseOfGiantBranch::~BaseOfGiantBranch() = default;

/**
 * @param i_Mass Mass
 * @returns \f$ t_{BGB}\f$
 * @pre \c i_Mass is positive
 * @throws PreconditionError If the precondition is violated
 */
Herd::Generic::Time BaseOfGiantBranch::Age( Herd::Generic::Mass i_Mass )
{
  Herd::Generic::ThrowIfNotPositive( i_Mass, "i_Mass" );

  // @formatter:off
  return UpdateCache( m_MDependents.m_Age, i_Mass, [ & ]( auto i_Mass ){ return ComputeAge(i_Mass);} ); // @suppress("Invalid arguments")
    // @formatter:on
}

/**
 * @param i_Mass Mass
 * @returns \f$ L_{BGB}\f$
 * @pre \c i_Mass is positive
 * @throws PreconditionError If the precondition is violated
 */
Herd::Generic::Luminosity BaseOfGiantBranch::Luminosity( Herd::Generic::Mass i_Mass )
{
  Herd::Generic::ThrowIfNotPositive( i_Mass, "i_Mass" );

  // @formatter:off
  return UpdateCache( m_MDependents.m_Luminosity, i_Mass, [ & ]( auto i_Mass ){ return ComputeLuminosity(i_Mass);} ); // @suppress("Invalid arguments")
      // @formatter:on
}

/**
 * @param i_Mass Mass
 * @returns \f$ R_{BGB}\f$
 * @pre \c i_Mass is positive
 * @throws PreconditionError If the precondition is violated
 */
Herd::Generic::Radius BaseOfGiantBranch::Radius( Herd::Generic::Mass i_Mass )
{
  Herd::Generic::ThrowIfNotPositive( i_Mass, "i_Mass" );

  // @formatter:off
  return UpdateCache( m_MDependents.m_Radius, i_Mass, [ & ]( auto i_Mass ){ return ComputeRadius( i_Mass); } ); // @suppress("Invalid arguments")
      // @formatter:on
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

  auto& rB = m_ZDependents.m_LBGB;
  rB[ 2 ] = std::pow( rB[ 2 ], rB[ 5 ] );
  rB[ 6 ] = 4.637345e+00;
  rB[ 7 ] = 9.301992e+00;

  // RBGB
  m_ZDependents.m_pRGBComputer = std::make_unique< Herd::SSE::GiantBranchRadius >( i_Z );
}

/**
 * @param i_Mass Mass
 * @return Age at BGB
 */
Herd::Generic::Time BaseOfGiantBranch::ComputeAge( Herd::Generic::Mass i_Mass ) const
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

/**
 * @param i_Mass Mass
 * @return \f$ L_{BGB}\f$
 */
Herd::Generic::Luminosity BaseOfGiantBranch::ComputeLuminosity( Herd::Generic::Mass i_Mass ) const
{
  auto& rB = m_ZDependents.m_LBGB;

  // Eq. 10
  double num = Herd::SSE::BXhC( i_Mass, rB[ 0 ], rB[ 4 ] ) + BXhC( i_Mass, rB[ 1 ], rB[ 7 ] );
  double den = Herd::SSE::ApBXhC( i_Mass, rB[ 2 ], rB[ 3 ], rB[ 6 ] ) + std::pow( i_Mass, rB[ 5 ] );
  return Herd::Generic::Luminosity( num / den );
}

/**
 * @param i_Mass Mass
 * @return \f$ R_{BGB}\f$
 */
Herd::Generic::Radius BaseOfGiantBranch::ComputeRadius( Herd::Generic::Mass i_Mass ) const
{
  return m_ZDependents.m_pRGBComputer->Compute( i_Mass, ComputeLuminosity( i_Mass ) );
}

}


