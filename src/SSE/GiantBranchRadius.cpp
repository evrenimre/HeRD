/**
 * @file GiantBranchRadius.cpp
 * @author Evren Imre
 * @date 1 Sep 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "GiantBranchRadius.h"

#include "Constants.h"
#include "MathHelpers.h"

#include <algorithm>
#include <cmath>
#include <iterator>

#include <range/v3/algorithm.hpp>

namespace
{

// @formatter:off

std::array< double, 24 > s_ZRGB { 9.960283e-01, 8.164393e-01, 2.383830e+00, 2.223436e+00, 8.638115e-01, 1.231572e-01,
  2.561062e-01, 7.072646e-02, -5.444596e-02, -5.798167e-02, -1.349129e-02, 0.,
  1.157338e+00, 1.467883e+00, 4.299661e+00, 3.130500e+00, 6.992080e-01, 1.640687e-02,
  4.022765e-01, 3.050010e-01, 9.962137e-01, 7.914079e-01, 1.728098e-01, 0.
};
// @formatter:on
}

namespace Herd::SSE
{

using Herd::SSE::ApBXhC;
using Herd::SSE::BXhC;

/**
 * @param i_Z Initial metallicity
 * @pre \c i_Z>0
 * @throws PreconditionError If preconditions are violated
 */
GiantBranchRadius::GiantBranchRadius( Herd::Generic::Metallicity i_Z )
{
  Herd::Generic::ThrowIfNotPositive( i_Z, "i_Z" );

  m_A.first.Set( 0. );
  m_A.second = 0.;

  std::array< double, 6 > zetaPowers5;
  double zeta = std::log10( i_Z / Herd::SSE::Constants::s_SolarMetallicityTout96 );
  Herd::SSE::ComputePowers( zetaPowers5, zeta );

  std::array< double, 3 > zetaPowers2;
  ranges::cpp20::copy_n( zetaPowers5.begin(), 3, zetaPowers2.begin() );

  double logZ = std::log10( i_Z );
  m_B[ 0 ] = std::max( std::pow( 10., -4.6739 - 0.9394 * logZ ), -0.04167 + 55.67 * i_Z );
  m_B[ 1 ] = std::min( 0.54, Herd::SSE::ComputeInnerProduct( { 0.397, 0.28826, 0.5293 }, zetaPowers2 ) );

  {
    m_B[ 2 ] = std::pow( 10., std::max( -0.1451, -2.2794 - 1.5175 * logZ + 0.254 * logZ * logZ ) );
    if( i_Z > 0.004 )
    {
      m_B[ 2 ] = std::max( m_B[ 2 ], ApBXhC( i_Z, 0.7307, 14265.1, 3.395 ) );
    }
  }

  std::array< double, 4 > tempB;
  Herd::SSE::MultiplyMatrixVector( tempB, s_ZRGB, zetaPowers5 );
  ranges::cpp20::copy( tempB, std::next( m_B.begin(), 3 ) ); // @suppress("Invalid arguments")
}

/**
 * @param i_Mass Mass
 * @param i_Luminosity Luminosity
 * @return Giant branch radius
 * @pre \c i_Mass>0
 * @pre \c i_Luminosity>0
 * @throws PreconditionError If preconditions are violated
 */
Herd::Generic::Radius GiantBranchRadius::Compute( Herd::Generic::Mass i_Mass, Herd::Generic::Luminosity i_Luminosity )
{
  Herd::Generic::ThrowIfNotPositive( i_Mass, "i_Mass" );
  Herd::Generic::ThrowIfNotPositive( i_Luminosity, "i_Luminosity" );

  if( i_Mass != m_A.first )
  {
    m_A.first = i_Mass;
    m_A.second = std::min( BXhC( i_Mass, m_B[ 3 ], m_B[ 4 ] ), BXhC( i_Mass, m_B[ 5 ], m_B[ 6 ] ) );
  }

  return Herd::Generic::Radius( m_A.second * ( std::pow( i_Luminosity, m_B[ 1 ] ) + BXhC( i_Mass, m_B[ 0 ], m_B[ 2 ] ) ) );
}

}

