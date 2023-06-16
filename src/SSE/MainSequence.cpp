/**
 * @file MainSequence.cpp
 * @author Evren Imre
 * @date 30 Jul 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "MainSequence.h"

#include "Constants.h"
#include "EvolutionState.h"
#include "MathHelpers.h"

#include <Physics/LuminosityRadiusTemperature.h>

#include <cmath>
#include <iterator>

#include <boost/math/constants/constants.hpp>
#include <boost/math/special_functions/pow.hpp>
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

const std::array< double, 20 > s_ZThook { 1.949814e+01, 1.758178e+00, -6.008212e+00, -4.470533e+00,
  4.903830e+00, 0., 0., 0.,
  5.212154e-02, 3.166411e-02, -2.750074e-03, -2.271549e-03,
  1.312179e+00, -3.294936e-01, 9.231860e-02, 2.610989e-02,
  8.073972e-01, 0., 0., 0.
};  ///< Coefficients for \f$ T_{hook}(z) \f$ calculations

const std::array< double, 30 > s_ZLTMS { 1.031538e+00, -2.434480e-01, 7.732821e+00, 6.460705e+00, 1.374484e+00,
  1.043715e+00, -1.577474e+00, -5.168234e+00, -5.596506e+00, -1.299394e+00,
  7.859573e+02, -8.542048e+00, -2.642511e+01, -9.585707e+00, 0.,
  3.858911e+03, 2.459681e+03, -7.630093e+01, -3.486057e+02, -4.861703e+01,
  2.888720e+02, 2.952979e+02, 1.850341e+02, 3.797254e+01, 0.,
  7.196580e+00, 5.613746e-01, 3.805871e-01, 8.398728e-02, 0.
};  ///< Coefficients for \f$ L_{TMS}(z) \f$ calculations

const std::array< double, 44 > s_ZAlphaL { 2.321400e-01, 1.828075e-03, -2.232007e-02, -3.378734e-03,
  1.163659e-02, 3.427682e-03, 1.421393e-03, -3.710666e-03,
  1.048020e-02, -1.231921e-02, -1.686860e-02, -4.234354e-03,
  1.555590e+00, -3.223927e-01, -5.197429e-01, -1.066441e-01,
  0.0977e+00, -0.231e+00, -0.0753e+00, 0.,
  0.24e+00, 0.18e+00, 0.595e+00, 0.,
  0.33e+00, 0.132e+00, 0.218e+00, 0.,
  1.1064e+00, 0.415e+00, 0.18e+00, 0.,
  1.19e+00, 0.377e+00, 0.176e+00, 0.,
  0.306e+00, 0.053e+00, 0., 0.,
  0.3625e+00, 0.062e+00, 0., 0.
};  ///< Coefficients for \f$ alpha_L(z) \f$

std::array< double, 20 > s_ZBetaL { 3.855707e-01, -6.104166e-01, 5.676742e+00, 1.060894e+01, 5.284014e+00,
  3.579064e-01, -6.442936e-01, 5.494644e+00, 1.054952e+01, 5.280991e+00,
  9.587587e-01, 8.777464e-01, 2.017321e-01, 0., 0.,
  1.5135e+00, 0.3769e+00, 0., 0., 0.
};  ///< Coefficients for \f$ beta_L(z) \f$

std::array<double, 20 > s_ZLhook{1.910302e-01, 1.158624e-01, 3.348990e-02, 2.599706e-03,
  3.931056e-01, 7.277637e-02, -1.366593e-01,-4.508946e-02,
  3.267776e-01, 1.204424e-01, 9.988332e-02, 2.455361e-02,
  5.990212e-01, 5.570264e-02, 6.207626e-02, 1.777283e-02,
  1.5135e+00, 0.3769e+00, 0., 0.
};  ///< Coefficients for \f$ L_{hook}(z) \f$

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

const std::array< double, 65 > s_ZAlphaR { 4.907546e-01, -1.683928e-01, -3.108742e-01, -7.202918e-02, 0.,
  4.537070e+00, -4.465455e+00, -1.612690e+00, -1.623246e+00, 0.,
    1.796220e+00, 2.814020e-01, 1.423325e+00, 3.421036e-01, 0.,
    2.256216e+00, 3.773400e-01, 1.537867e+00, 4.396373e-01, 0.,
    1.564231e-03, 1.653042e-03, -4.439786e-03, -4.951011e-03, -1.216530e-03,
    5.210157e+00, -4.143695e+00, -2.120870e+00, 0., 0.,
    1.116e+00, 0.166e+00, 0., 0., 0.,
    1.477e+00, 0.296e+00, 0., 0., 0.,
    -0.308e+00, -1.046e+00, 0., 0., 0.,
    0.8e+00, -2.0e+00, 0., 0., 0.,
    0.0843e+00, -0.0475e+00, -0.0352e+00, 0., 0.,
    0.0736e+00, 0.0749e+00, 0.04426e+00, 0., 0.,
    0.136e+00, 0.0352e+00, 0., 0., 0.
};  ///< Coefficients for \f$ \alpha_R(z) \f$ calculations

const std::array< double, 24 > s_ZBetaR { 1.071489e+00, -1.164852e-01, -8.623831e-02, -1.582349e-02,
  7.108492e-01, 7.935927e-01, 3.926983e-01, 3.622146e-02,
    3.478514e+00, -2.585474e-02, -1.512955e-02, -2.833691e-03,
    3.969331e-03, 4.539076e-03, 1.720906e-03, 1.897857e-04,
    9.132108e-01, -1.653695e-01, 0., 3.636784e-02,
    1.6e+00, 0.764e+00, 0.3322e+00, 0.
};  ///< Coefficients for \f$ \beta_R(z) \f$ calculations

const std::array< double, 48 > s_ZGammaR { 1.192334e-02, 1.083057e-02, 1.230969e+00, 1.551656e+00,
  -1.668868e-01, 5.818123e-01, -1.105027e+01, -1.668070e+01,
  7.615495e-01, 1.068243e-01, -2.011333e-01, -9.371415e-02,
 -1.015564e-01, -2.161264e-01, -5.182516e-02, 0.,
 -3.868776e-01, -5.457078e-01, -1.463472e-01, 0.,
  9.409838e+00, 1.522928e+00, 0., 0.,
  7.454e+00, 9.046e+00, 0., 0.,
  -13.3e+00, -18.6e+00, 0., 0.,
  2.493e+00, 1.1475e+00, 0., 0.,
  0.8109e+00, -0.6282e+00, 0., 0.,
  0.6355e+00, -0.4192e+00, 0., 0.,
  -0.2711e+00, -0.5756e+00, -0.0838e+00, 0.
};   ///< Coefficients for \f$ \gamma_R(z) \f$ calculations

const std::array< double, 28 > s_ZRhook { 7.330122e-01, 5.192827e-01, 2.316416e-01, 8.346941e-03,
  1.172768e+00, -1.209262e-01, -1.193023e-01, -2.859837e-02,
  3.982622e-01, -2.296279e-01, -2.262539e-01, -5.219837e-02,
  3.571038e+00, -2.223625e-02, -2.611794e-02, -6.359648e-03,
  1.9848e+00, 1.1386e+00, 0.3564e+00, 0.,
  0.063e+00,  0.0481e+00, 0.00984e+00, 0.,
  1.2e+00, 2.45e+00, 0., 0.
}; ///< Coefficients for \f$ R_{hook}(z) \f$ calculations


std::array< double, 24 > s_ZLBGB { 9.511033e+01, 6.819618e+01, -1.045625e+01, -1.474939e+01,
  3.113458e+01, 1.012033e+01, -4.650511e+00, -2.463185e+00,
    1.413057e+00, 4.578814e-01, -6.850581e-02, -5.588658e-02,
    3.910862e+01, 5.196646e+01, 2.264970e+01, 2.873680e+00,
    4.597479e+00, -2.855179e-01, 2.709724e-01, 0.,
    6.682518e+00, 2.827718e-01, -7.294429e-02, 0.
};

std::array< double, 15 > s_ZLHeI { 2.751631e+03, 3.557098e+02, 0.,
  -3.820831e-02, 5.872664e-02, 0.,
  1.071738e+02, -8.970339e+01, -3.949739e+01,
  7.348793e+02, -1.531020e+02, -3.793700e+01,
  9.219293e+00, -2.005865e+00, -5.561309e-01
};

// @formatter:on


}

namespace Herd::SSE
{

using Herd::SSE::ComputeBlendWeight;
using Herd::SSE::ApBXhC;
using Herd::SSE::BXhC;

/**
 * @param i_InitialMetallicity Metallicity at ZAMS
 * @pre \c i_InitialMetallicity is in (0,1]
 * @throws PreconditionError If the precondition is violated
 */
MainSequence::MainSequence( Herd::Generic::Metallicity i_InitialMetallicity ) :
    m_InitialMetallicity( i_InitialMetallicity )
{
  Herd::Generic::ThrowIfNotPositive( i_InitialMetallicity, "i_InitialMetallicity" );

  ComputeMetallicityDependents( i_InitialMetallicity );
}

/**
 * @param[in, out] io_rState Evolution state
 * @return \c false if star is not in this evolution stage
 * @pre Mass in \c io_rState is positive
 * @pre Age in \c io_rState is non-negative
 * @throws PreconditionError If any preconditions are violated
 * @remarks Call at age=0 returns the ZAMS state
 */
bool MainSequence::Evolve( Herd::SSE::EvolutionState& io_rState )
{
  // Validation
  Herd::Generic::ThrowIfNotPositive( io_rState.m_TrackPoint.m_Mass, "m_Mass" );
  Herd::Generic::ThrowIfNegative( io_rState.m_EffectiveAge, "m_EffectiveAge" );

  auto& rTrackPoint = io_rState.m_TrackPoint;
  auto mass = rTrackPoint.m_Mass;

  // Still MS?
  Herd::Generic::Age tMS;
  Herd::Generic::Age thook;
  if( mass != m_MDependents.m_EvaluatedAt )
  {
    std::tie( tMS, thook ) = ComputeTimescales( mass );
  } else
  {
    tMS = m_MDependents.m_TMS;
    thook = m_MDependents.m_Thook;
  }

  // Change in mass changes the effective age of the star
  Herd::Generic::Age tMSOld = ( rTrackPoint.m_Mass == io_rState.m_MZAMS ) ? tMS : io_rState.m_TMS;

  // Handling the no tMS change separately is numerically more stable. Otherwise, at the end of the stage, effectiveAge can fall just short of tMS, causing insertion of an extra track point
  Herd::Generic::Age effectiveAge = io_rState.m_EffectiveAge;
  if( tMS == tMSOld )
  {
    effectiveAge += io_rState.m_DeltaT;
  } else
  {
    effectiveAge.Set( std::fma( effectiveAge, tMS / tMSOld, io_rState.m_DeltaT ) );
  }

  // Not a MS star
  if( effectiveAge >= tMS )
  {
    return false;
  }

  // Modify io_rState only after this point, as the call cannot fail
  
  // Need to update the mass dependents?
  if( mass != m_MDependents.m_EvaluatedAt )
  {
    m_MDependents.m_TMS = tMS;
    m_MDependents.m_Thook = thook;
    ComputeMassDependents( mass );
  }
  
  double tInthook = effectiveAge / thook;
  double tau1 = std::min( 1., tInthook );  // Eq. 14. This term linearly ramps up until hook
  double tau2 = std::clamp( 100. * tInthook - 99., 0., 1. ); // Eq. 15. This term swings sharply from (0.99, 0.) to ( 1.0, 1.), i.e. right before the hook

  double tau = effectiveAge / tMS; // Eq. 11.  Progress in MS

  // Eq. 12
  Herd::Generic::Luminosity luminosity;
  if( tau > 0 )
  {
    double term1 = m_MDependents.m_AlphaL * tau;
    double term2 = BXhC( tau, m_MDependents.m_BetaL, m_MDependents.m_Eta );
    double term3 = ( std::log10( m_MDependents.m_LTMS / m_MDependents.m_LZAMS ) - m_MDependents.m_AlphaL - m_MDependents.m_BetaL ) * tau * tau;
    double term4 = m_MDependents.m_DeltaL * ( ( tau1 - tau2 ) * ( tau1 + tau2 ) );
    luminosity.Set( std::pow( 10., term1 + term2 + term3 - term4 ) * m_MDependents.m_LZAMS );
  } else
  {
    [[unlikely]] luminosity.Set( m_MDependents.m_LZAMS );
  }

  Herd::Generic::Radius radius;
  if( tau > 0 )
  {
    double term1 = m_MDependents.m_AlphaR * tau;
    double term2 = m_MDependents.m_BetaR * boost::math::pow< 10 >( tau );
    double term3 = m_MDependents.m_GammaR * boost::math::pow< 40 >( tau );
    double term4 = ( std::log10( m_MDependents.m_RTMS / m_MDependents.m_RZAMS ) - m_MDependents.m_AlphaR - m_MDependents.m_BetaR - m_MDependents.m_GammaR )
        * boost::math::pow< 3 >( tau );
    double term5 = m_MDependents.m_DeltaR * ( boost::math::pow< 3 >( tau1 ) - boost::math::pow< 3 >( tau2 ) );
    radius.Set( std::pow( 10., term1 + term2 + term3 + term4 - term5 ) * m_MDependents.m_RZAMS );
  } else
  {
    [[unlikely]] radius.Set( m_MDependents.m_RZAMS );
  }

  // AMUSE.SSE, special case handling for low mass stars
  Herd::SSE::EvolutionStage stage;
  if( rTrackPoint.m_Mass < m_ZDependents.m_Mhook - 0.3 )
  {
    stage = Herd::SSE::EvolutionStage::e_MSLM;

    double hPercentage = 0.76 - 3 * rTrackPoint.m_InitialMetallicity;
    double rDegenerate = 0.0258 * std::pow( 1. + hPercentage, 5. / 3. ) / std::cbrt( mass );
    radius.Set( std::max( radius.Value(), rDegenerate ) );
  } else
  {
    stage = Herd::SSE::EvolutionStage::e_MS;
  }

  rTrackPoint.m_InitialMetallicity = m_InitialMetallicity;
  rTrackPoint.m_Luminosity = luminosity;
  rTrackPoint.m_Radius = radius;
  rTrackPoint.m_Temperature = Herd::Physics::ComputeAbsoluteTemperature( luminosity, radius );
  rTrackPoint.m_Stage = stage;
  rTrackPoint.m_CoreMass.Set( 0. );
  io_rState.m_CoreRadius.Set( 0. );

  io_rState.m_TMS = m_MDependents.m_TMS;

  io_rState.m_MFGB = m_ZDependents.m_MFGB;
  io_rState.m_LTMS = m_MDependents.m_LTMS;
  io_rState.m_RTMS = m_MDependents.m_RTMS;
  io_rState.m_RZAMS = m_MDependents.m_RZAMS;

  io_rState.m_LBGB = m_MDependents.m_LBGB;
  io_rState.m_LHeI = m_MDependents.m_LHeI;

  io_rState.m_EffectiveAge = effectiveAge;

  // Convective envelope

  // Reset the convective envelope properties. At this point they are outdated and can trigger a validation failure
  rTrackPoint.m_EnvelopeMass.Set( 0. );
  io_rState.m_EnvelopeRadius.Set( 0. );

  io_rState.m_Rg = m_MDependents.m_Rg;

  auto convectiveEnvelope = m_ConvectiveEnvelopeComputer.Compute( io_rState );
  rTrackPoint.m_EnvelopeMass = convectiveEnvelope.m_Mass;
  io_rState.m_EnvelopeRadius = convectiveEnvelope.m_Radius;

  return true;
}

/**
 * @param i_Z Metallicity
 */
void MainSequence::ComputeMetallicityDependents( Herd::Generic::Metallicity i_Z )
{
  m_ZDependents.m_EvaluatedAt = i_Z;

  Herd::Generic::Metallicity relativeZ( i_Z / Herd::SSE::Constants::s_SolarMetallicityTout96 ); // Metallicity relative to sun

  std::array< double, 5 > zetaPowers4;
  double zeta = std::log10( relativeZ );
  Herd::SSE::ComputePowers( zetaPowers4, zeta );

  std::array< double, 4 > zetaPowers3;
  ranges::cpp20::copy_n( zetaPowers4.begin(), 4, zetaPowers3.begin() );

  std::array< double, 3 > zetaPowers2;
  ranges::cpp20::copy_n( zetaPowers4.begin(), 3, zetaPowers2.begin() );

  m_ZDependents.m_Mhook.Set( Herd::SSE::ComputeInnerProduct( { 1.0185, 0.16015, 0.0892 }, zetaPowers2 ) );  // Eq. 1
  m_ZDependents.m_MHeF.Set( Herd::SSE::ComputeInnerProduct( { 1.995, 0.25, 0.087 }, zetaPowers2 ) );  // Eq. 2
  m_ZDependents.m_MFGB.Set( BXhC( relativeZ, 13.048, 0.06 ) / ApBXhC( relativeZ, 1., 1e-4, -1.27 ) );  // Eq. 3, but 0.0012 replaced by 1e-4 in AMUSE.SSE

  // TBGB
  Herd::SSE::MultiplyMatrixVector( m_ZDependents.m_TBGB, s_ZTBGB, zetaPowers3 );

  // ThookCoefficients
  Herd::SSE::MultiplyMatrixVector( m_ZDependents.m_Thook, s_ZThook, zetaPowers3 );

  // LTMS
  Herd::SSE::MultiplyMatrixVector( m_ZDependents.m_LTMS, s_ZLTMS, zetaPowers4 );
  m_ZDependents.m_LTMS[ 0 ] *= m_ZDependents.m_LTMS[ 3 ];
  m_ZDependents.m_LTMS[ 1 ] *= m_ZDependents.m_LTMS[ 3 ];

  // AlphaL
  std::array< double, 11 > tempAlphaL;
  Herd::SSE::MultiplyMatrixVector( tempAlphaL, s_ZAlphaL, zetaPowers3 );
  ranges::cpp20::copy_n( tempAlphaL.begin(), 4, m_ZDependents.m_AlphaL.begin() );
  {
    auto& rA = m_ZDependents.m_AlphaL;
    rA[ 4 ] = std::max( 0.9, tempAlphaL[ 7 ] );
    rA[ 5 ] = std::max( 1., tempAlphaL[ 8 ] );
    if( i_Z > 0.01 )
    {
      rA[ 4 ] = std::min( rA[ 4 ], 1. );
      rA[ 5 ] = std::min( rA[ 5 ], 1.1 );
    }

    rA[ 6 ] = std::max( 0.145, tempAlphaL[ 4 ] );
    rA[ 7 ] = std::min( tempAlphaL[ 5 ], tempAlphaL[ 9 ] );
    rA[ 8 ] = std::min( tempAlphaL[ 6 ], tempAlphaL[ 10 ] );
    rA[ 9 ] = ComputeAlphaL( Herd::Generic::Mass( 2. ) );
  }

  // BetaL
  Herd::SSE::MultiplyMatrixVector( m_ZDependents.m_BetaL, s_ZBetaL, zetaPowers4 );
  m_ZDependents.m_BetaL[ 3 ] = std::min( 1.4, m_ZDependents.m_BetaL[ 3 ] );
  m_ZDependents.m_BetaL[ 3 ] = std::max( { 0.6355e+00 - 0.4192e+00 * zeta, 1.25, m_ZDependents.m_BetaL[ 3 ] } );

  // Lhook
  Herd::SSE::MultiplyMatrixVector( m_ZDependents.m_Lhook, s_ZLhook, zetaPowers3 );
  m_ZDependents.m_Lhook[ 4 ] = std::min( 1.4, m_ZDependents.m_Lhook[ 4 ] );
  m_ZDependents.m_Lhook[ 4 ] = std::max( { 0.6355e+00 - 0.4192e+00 * zeta, 1.25, m_ZDependents.m_Lhook[ 4 ] } );

  // RTMS
  std::array< double, 10 > tempRTMS;
  Herd::SSE::MultiplyMatrixVector( tempRTMS, s_ZRTMS, zetaPowers4 );
  ranges::cpp20::copy( tempRTMS, m_ZDependents.m_RTMS.begin() );
  m_ZDependents.m_RTMS[ 0 ] *= m_ZDependents.m_RTMS[ 2 ];
  m_ZDependents.m_RTMS[ 1 ] *= m_ZDependents.m_RTMS[ 2 ];

  double sigma = log10( i_Z );
  m_ZDependents.m_RTMS[ 10 ] = std::pow( 10., std::max( { 0.097 - 0.1072 * ( sigma + 3. ), 0.097, std::min( 0.1461, 0.1461 + 0.1237 * ( sigma + 2 ) ) } ) );

  {
    auto& rA = m_ZDependents.m_RTMS;
    rA[ 11 ] = ComputeRTMS( Herd::Generic::Mass( rA[ 10 ] ) ); // Eq. 9a, evaluated at a17
    rA[ 12 ] = ComputeRTMS( Herd::Generic::Mass( rA[ 10 ] + 0.1 ) );  //Eq. 9b, evaluated at Mstar
  }

  // AlphaR
  std::array< double, 13 > tempAlphaR;
  Herd::SSE::MultiplyMatrixVector( tempAlphaR, s_ZAlphaR, zetaPowers4 );
  ranges::cpp20::copy_n( tempAlphaR.begin(), 5, m_ZDependents.m_AlphaR.begin() );

  {
    auto& rA = m_ZDependents.m_AlphaR;
    rA[ 5 ] = std::clamp( tempAlphaR[ 6 ], 0.9, 1.0 );
    rA[ 6 ] = std::max( tempAlphaR[ 7 ], std::min( 1.6, tempAlphaR[ 8 ] ) );
    rA[ 6 ] = std::max( 0.8, std::min( tempAlphaR[ 9 ], rA[ 6 ] ) );
    rA[ 7 ] = tempAlphaR[ 5 ];
    rA[ 8 ] = std::max( 0.065, tempAlphaR[ 10 ] );
    rA[ 9 ] = i_Z >= 0.004 ? tempAlphaR[ 11 ] : std::min( 0.055, tempAlphaR[ 11 ] );
    rA[ 10 ] = std::clamp( tempAlphaR[ 12 ], 0.091, 0.121 );
    rA[ 11 ] = BXhC( rA[ 6 ], rA[ 0 ], rA[ 2 ] ) / ApBXhC( rA[ 6 ], rA[ 1 ], 1., rA[ 3 ] ); // AMUSE.SSE. Paper uses alpha_R at rA[6], but this does not work correctly if rA[5] > rA[6]
    if( rA[ 5 ] > rA[ 6 ] )
    {
      rA[ 5 ] = rA[ 6 ];
      rA[ 10 ] = rA[ 11 ];
    }
  }

  // BetaR
  Herd::SSE::MultiplyMatrixVector( m_ZDependents.m_BetaR, s_ZBetaR, zetaPowers3 );
  m_ZDependents.m_BetaR[ 4 ] = i_Z <= 0.01 ? m_ZDependents.m_BetaR[ 4 ] : std::max( 0.95, m_ZDependents.m_BetaR[ 4 ] );
  m_ZDependents.m_BetaR[ 5 ] = std::clamp( m_ZDependents.m_BetaR[ 5 ], 1.4, 1.6 );

  // GammaR
  std::array< double, 12 > tempGammaR;
  Herd::SSE::MultiplyMatrixVector( tempGammaR, s_ZGammaR, zetaPowers3 );

  {
    auto& rA = m_ZDependents.m_GammaR;
    rA[ 0 ] = std::max( tempGammaR[ 0 ], tempGammaR[ 3 ] );
    rA[ 1 ] = std::max( tempGammaR[ 4 ], std::min( 0., tempGammaR[ 1 ] ) );
    rA[ 2 ] = std::max( 0., std::min( tempGammaR[ 2 ], tempGammaR[ 6 ] ) );
    rA[ 3 ] = std::min( tempGammaR[ 5 ], std::max( 2., tempGammaR[ 7 ] ) );
    rA[ 4 ] = std::clamp( tempGammaR[ 8 ], 0.4, 1.5 );
    rA[ 5 ] = std::max( tempGammaR[ 10 ], std::clamp( tempGammaR[ 9 ], 1., 1.27 ) );
    rA[ 6 ] = std::max( 5.855420e-02, tempGammaR[ 11 ] );
  }

  // Rhook
  Herd::SSE::MultiplyMatrixVector( m_ZDependents.m_Rhook, s_ZRhook, zetaPowers3 );
  m_ZDependents.m_Rhook[ 4 ] = std::clamp( m_ZDependents.m_Rhook[ 4 ], 1.1, 1.25 );
  m_ZDependents.m_Rhook[ 6 ] = std::clamp( m_ZDependents.m_Rhook[ 6 ], 0.45, 1.3 );

  // Maximum value of eta
  m_ZDependents.m_MaxEta = i_Z > 0.0009 ? 10. : 20.;

  // Eq. 6, but AMUSE.SSE adds an extra term
  {
    double extra = std::min( 0.99, 0.98 - ( 100. / 7. ) * ( i_Z - 0.001 ) );
    double left = 0.95 - ( 10. / 3. ) * ( i_Z - 0.01 );
    m_ZDependents.m_X = std::max( { 0.95, left, extra } );
  }

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
    Herd::Generic::Luminosity lHeI = ComputeLHeI( m_ZDependents.m_MHeF );  // L_HeI at M_HeF
    rB[ 3 ] = ( BXhC( m_ZDependents.m_MHeF, rB[ 0 ], rB[ 1 ] ) - lHeI ) / ( lHeI * std::exp( m_ZDependents.m_MHeF * rB[ 2 ] ) ); // AMASS.SSE implements this differently from the paper
  }

  // Initialise the ZAMS computer
  m_ZDependents.m_pZAMSComputer = std::make_unique< Herd::SSE::ZeroAgeMainSequence >( i_Z );

  // Initialise the RGB computer
  m_ZDependents.m_pRGBComputer = std::make_unique< Herd::SSE::GiantBranchRadius >( i_Z );
}

/**
 * @param i_Mass Mass
 * @return Main sequence duration; start of hook
 */
std::pair< Herd::Generic::Age, Herd::Generic::Age > MainSequence::ComputeTimescales( Herd::Generic::Mass i_Mass ) const
{
  // Eq. 4
  double tBGB = 0;
  {
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

    tBGB = num / den;
  }

  // Eq. 7
  double mu = 0;
  {
    const auto& rA = m_ZDependents.m_Thook;
    double left = BXhC( i_Mass, rA[ 0 ], -rA[ 1 ] );
    double right = ApBXhC( i_Mass, rA[ 2 ], rA[ 3 ], -rA[ 4 ] );
    mu = std::max( 0.5, 1. - 0.01 * std::max( left, right ) );
  }

  double thook = mu * tBGB;

  return std::pair( Herd::Generic::Age( std::max( m_ZDependents.m_X * tBGB, thook ) ), Herd::Generic::Age( thook ) ); // Eq. 5 // @suppress("Ambiguous problem")
}

/**
 * @param i_Mass Mass
 * @remarks Does not compute the timescales
 */
void MainSequence::ComputeMassDependents( Herd::Generic::Mass i_Mass )
{
  m_MDependents.m_EvaluatedAt = i_Mass;

  // ZAMS
  Herd::SSE::TrackPoint zams = m_ZDependents.m_pZAMSComputer->Compute( i_Mass );
  m_MDependents.m_LZAMS = zams.m_Luminosity;
  m_MDependents.m_RZAMS = zams.m_Radius;

  // Luminosity
  m_MDependents.m_LTMS = ComputeLTMS( i_Mass );

  m_MDependents.m_AlphaL = ComputeAlphaL( i_Mass );
  m_MDependents.m_BetaL = ComputeBetaL( i_Mass );
  m_MDependents.m_DeltaL = ComputeLHook( i_Mass );

  m_MDependents.m_Eta = std::clamp( std::lerp( 10., 20., ( i_Mass - 1 ) / 0.1 ), 10., m_ZDependents.m_MaxEta ); // Eq. 18 and linear interpolation for Z <= 0.0009 . If Z> 0.0009, since m_MaxEta = 10, eta becomes 10

  // Radius
  m_MDependents.m_RTMS = ComputeRTMS( i_Mass );
  m_MDependents.m_AlphaR = ComputeAlphaR( i_Mass );
  m_MDependents.m_BetaR = ComputeBetaR( i_Mass );
  m_MDependents.m_GammaR = ComputeGammaR( i_Mass );
  m_MDependents.m_DeltaR = ComputeRHook( i_Mass );

  //BGB
  m_MDependents.m_LBGB = ComputeLBGB( i_Mass );
  Herd::Generic::Radius rBGB = m_ZDependents.m_pRGBComputer->Compute( i_Mass, m_MDependents.m_LBGB );   // Also used to set m_Rg
  if( i_Mass < m_ZDependents.m_MFGB )
  {
    m_MDependents.m_RBGB = rBGB;
  }

  // HeI
  m_MDependents.m_LHeI = ComputeLHeI( i_Mass );

  // Rg
  m_MDependents.m_Rg = rBGB;
}

/**
 * @param i_Mass Mass
 * @return Luminosity at TMS
 */
Herd::Generic::Luminosity MainSequence::ComputeLTMS( Herd::Generic::Mass i_Mass ) const
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
 * @return \f$ \alpha_L \f$
 */
double MainSequence::ComputeAlphaL( Herd::Generic::Mass i_Mass ) const
{
  auto& rA = m_ZDependents.m_AlphaL;

  // Eq. 19b
  if( i_Mass <= 0.5 )
  {
    return rA[ 6 ];
  }

  if( i_Mass > 0.5 && i_Mass <= 0.7 )
  {
    return std::lerp( rA[ 6 ], 0.3, ComputeBlendWeight( i_Mass, 0.5, 0.7 ) );
  }

  if( i_Mass > 0.7 && i_Mass <= rA[ 4 ] )
  {
    return std::lerp( 0.3, rA[ 7 ], ComputeBlendWeight( i_Mass, 0.7, rA[ 4 ] ) );
  }

  if( i_Mass > rA[ 4 ] && i_Mass <= rA[ 5 ] )
  {
    return std::lerp( rA[ 7 ], rA[ 8 ], ComputeBlendWeight( i_Mass, rA[ 4 ], rA[ 5 ] ) );
  }

  if( i_Mass > rA[ 5 ] && i_Mass < 2. )
  {
    return lerp( rA[ 8 ], rA[ 9 ], ComputeBlendWeight( i_Mass, rA[ 5 ], 2. ) );
  }

  // Eq. 19a, mass >= 2
  double num = ApBXhC( i_Mass, rA[ 0 ], rA[ 1 ], rA[ 3 ] );
  double den = std::pow( i_Mass, 0.4 ) * ( 1 + rA[ 2 ] * i_Mass * std::sqrt( i_Mass ) );
  return num / den;
}

/**
 * @param i_Mass Mass
 * @return \f$ \beta_L \f$
 */
double MainSequence::ComputeBetaL( Herd::Generic::Mass i_Mass ) const
{
  // Eq. 20
  auto& rA = m_ZDependents.m_BetaL;

  double betaL = ApBXhC( i_Mass, rA[ 0 ], -rA[ 1 ], rA[ 2 ] );

  if( i_Mass > rA[ 3 ] && betaL > 0. )
  {
    betaL = ApBXhC( rA[ 3 ], rA[ 0 ], -rA[ 1 ], rA[ 2 ] ) * ( 1. - 10. * ( i_Mass - rA[ 3 ] ) );

  }

  return std::max( 0., betaL );
}

/**
 * @param i_Mass Mass
 * @return \f$ \Delta_L \f$
 */
double MainSequence::ComputeLHook( Herd::Generic::Mass i_Mass ) const
{
  // Eq. 16
  auto& rA = m_ZDependents.m_Lhook;

  if( i_Mass <= m_ZDependents.m_Mhook )
  {
    return 0.;
  }

  auto LuminosityComputer = [ & ]( auto i_M )
  { return std::min( BXhC( i_M, rA[ 0 ], -rA[ 1 ] ), BXhC( i_M, rA[ 2 ], -rA[ 3 ] ) );};

  if( i_Mass >= rA[ 4 ] )
  {
    return LuminosityComputer( i_Mass );
  }

  double b = LuminosityComputer( rA[ 4 ] );
  return BXhC( ComputeBlendWeight( i_Mass, m_ZDependents.m_Mhook, rA[ 4 ] ), b, 0.4 );
}

/**
 * @param i_Mass Mass
 * @return Radius at TMS
 */
Herd::Generic::Radius MainSequence::ComputeRTMS( Herd::Generic::Mass i_Mass ) const
{
  auto& rA = m_ZDependents.m_RTMS;

  if( i_Mass <= rA[ 10 ] )
  {
    // Eq. 9a
    double num = ApBXhC( i_Mass, rA[ 0 ], rA[ 1 ], rA[ 3 ] );
    double den = ApBXhC( i_Mass, rA[ 2 ], 1., rA[ 4 ] );
    return Herd::Generic::Radius( std::max( 1.5 * m_MDependents.m_RZAMS, num / den ) ); // AMUSE.SSE added a check to ensure that RTMS > RZAMS
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
  return Herd::Generic::Radius( std::lerp( rA[ 11 ], rA[ 12 ], ComputeBlendWeight( i_Mass, rA[ 10 ], rA[ 10 ] + 0.1 ) ) );
}

/**
 * @param i_Mass Mass
 * @return \f$ \alpha_R \f$
 */
double MainSequence::ComputeAlphaR( Herd::Generic::Mass i_Mass ) const
{
  auto& rA = m_ZDependents.m_AlphaR;

  // Eq. 21b
  if( i_Mass <= 0.5 )
  {
    return rA[ 8 ];
  }

  if( i_Mass > 0.5 && i_Mass <= 0.65 )
  {
    return std::lerp( rA[ 8 ], rA[ 9 ], ComputeBlendWeight( i_Mass, 0.5, 0.65 ) );
  }

  if( i_Mass > 0.65 && i_Mass <= rA[ 5 ] )
  {
    return std::lerp( rA[ 9 ], rA[ 10 ], ComputeBlendWeight( i_Mass, 0.65, rA[ 5 ] ) );
  }

  // This function is used in the computation of rA[11]. So we need a < instead of a <=. The next if clause does not need rA[11]
  if( i_Mass > rA[ 5 ] && i_Mass < rA[ 6 ] )
  {
    return std::lerp( rA[ 10 ], rA[ 11 ], ComputeBlendWeight( i_Mass, rA[ 5 ], rA[ 6 ] ) );
  }

  if( i_Mass >= rA[ 6 ] && i_Mass <= rA[ 7 ] )
  {
    return BXhC( i_Mass, rA[ 0 ], rA[ 2 ] ) / ApBXhC( i_Mass, rA[ 1 ], 1., rA[ 3 ] );
  }

  double num = BXhC( rA[ 7 ], rA[ 0 ], rA[ 2 ] );
  double den = ApBXhC( rA[ 7 ], rA[ 1 ], 1., rA[ 3 ] );
  return ( num / den ) + rA[ 4 ] * ( i_Mass - rA[ 7 ] );
}

/**
 * @param i_Mass Mass
 * @return \f$ \beta_R \f$
 */
double MainSequence::ComputeBetaR( Herd::Generic::Mass i_Mass ) const
{
  auto& rA = m_ZDependents.m_BetaR;

  double betaR = 0;

  // Eq. 22b
  if( i_Mass <= 1. )
  {
    betaR = 1.06;
  }

  if( i_Mass > 1. && i_Mass <= rA[ 5 ] )
  {
    betaR = std::lerp( 1.06, rA[ 4 ], ComputeBlendWeight( i_Mass, 1., rA[ 5 ] ) );
  }

  if( i_Mass > rA[ 5 ] && i_Mass <= 2. )
  {
    double betaRAt2 = ( 8. * boost::math::constants::root_two< double >() * rA[ 0 ] ) / ApBXhC( 2., rA[ 1 ], 1., rA[ 2 ] );
    betaR = std::lerp( rA[ 4 ], betaRAt2, ComputeBlendWeight( i_Mass, rA[ 5 ], 2 ) );
  }

  if( i_Mass > 2. && i_Mass <= 16. )
  {
    // Eq. 22a
    double num = rA[ 0 ] * boost::math::pow< 3 >( i_Mass.Value() ) * std::sqrt( i_Mass );
    double den = ApBXhC( i_Mass, rA[ 1 ], 1., rA[ 2 ] );
    betaR = num / den;
  }

  if( i_Mass > 16. )
  {
    double betaRAt16 = 16384. * rA[ 0 ] / ApBXhC( 16., rA[ 1 ], 1., rA[ 2 ] );
    betaR = betaRAt16 + rA[ 3 ] * ( i_Mass - 16. );
  }

  return betaR - 1;
}

/**
 * @param i_Mass Mass
 * @return \f$ \gamma_R \f$
 */
double MainSequence::ComputeGammaR( Herd::Generic::Mass i_Mass ) const
{
  auto& rA = m_ZDependents.m_GammaR;

  // Eq. 23

  if( i_Mass > rA[ 5 ] + 0.1 )
  {
    return 0.;
  }

  double gammaR = 0.;

  if( i_Mass <= 1. )
  {
    gammaR = ApBXhC( std::abs( i_Mass - rA[ 2 ] ), rA[ 0 ], rA[ 1 ], rA[ 3 ] );
  } else
  {
    double gammaRAt1 = std::max( 0., ApBXhC( std::abs( 1. - rA[ 2 ] ), rA[ 0 ], rA[ 1 ], rA[ 3 ] ) );
    if( i_Mass > 1. && i_Mass <= rA[ 5 ] )
    {
      gammaR = ApBXhC( ComputeBlendWeight( i_Mass, 1., rA[ 5 ] ), gammaRAt1, rA[ 6 ] - gammaRAt1, rA[ 4 ] );
    }

    if( i_Mass > rA[ 5 ] && i_Mass <= rA[ 5 ] + 0.1 )
    {
      double c = rA[ 5 ] > 1. ? rA[ 6 ] : gammaRAt1;
      gammaR = c - 10. * c * ( i_Mass - rA[ 5 ] );
    }
  }

  return std::max( gammaR, 0. );
}

/**
 * @param i_Mass Mass
 * @return \f$ \Delta_R \f$
 */
double MainSequence::ComputeRHook( Herd::Generic::Mass i_Mass ) const
{
  auto& rA = m_ZDependents.m_Rhook;

  // Eq. 17

  if( i_Mass <= m_ZDependents.m_Mhook )
  {
    return 0.;
  }

  if( i_Mass > m_ZDependents.m_Mhook && i_Mass <= rA[ 4 ] )
  {
    return rA[ 5 ] * std::sqrt( ComputeBlendWeight( i_Mass, m_ZDependents.m_Mhook, rA[ 4 ] ) );
  }

  if( i_Mass > rA[ 4 ] && i_Mass <= 2. )
  {
    double num = rA[ 0 ] + 8. * boost::math::constants::root_two< double >() * rA[ 1 ];
    double den = 8. * rA[ 2 ] + std::pow( 2., rA[ 3 ] );
    return ApBXhC( ComputeBlendWeight( i_Mass, rA[ 4 ], 2. ), rA[ 5 ], ( num / den - 1. - rA[ 5 ] ), rA[ 6 ] );
  }

  double m05 = std::sqrt( i_Mass );
  double m30 = boost::math::pow< 3 >( i_Mass );

  return ( rA[ 0 ] + rA[ 1 ] * m30 * m05 ) / ( rA[ 2 ] * m30 + std::pow( i_Mass, rA[ 3 ] ) ) - 1.;
}

/**
 * @param i_Mass Mass
 * @return \f$ L_{BGB}\f$
 */
Herd::Generic::Luminosity MainSequence::ComputeLBGB( Herd::Generic::Mass i_Mass ) const
{
  auto& rB = m_ZDependents.m_LBGB;

  // Eq. 10
  double num = BXhC( i_Mass, rB[ 0 ], rB[ 4 ] ) + BXhC( i_Mass, rB[ 1 ], rB[ 7 ] );
  double den = ApBXhC( i_Mass, rB[ 2 ], rB[ 3 ], rB[ 6 ] ) + std::pow( i_Mass, rB[ 5 ] );
  return Herd::Generic::Luminosity( num / den );
}

/**
 * @param i_Mass Mass
 * @return \f$ L_{HeI} \f$
 */
Herd::Generic::Luminosity MainSequence::ComputeLHeI( Herd::Generic::Mass i_Mass ) const
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

}

