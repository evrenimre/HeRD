/**
 * @file ConvectiveEnvelope.cpp
 * @author Evren Imre
 * @date 29 Aug 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "ConvectiveEnvelope.h"

#include "EvolutionStage.h"
#include "EvolutionState.h"
#include "MathHelpers.h"
#include "TrackPoint.h"

#include <Physics/LuminosityRadiusTemperature.h>

#include <algorithm>
#include <cmath>

#include <boost/math/special_functions/pow.hpp>

namespace Herd::SSE
{

using Herd::SSE::ComputeBlendWeight;

/**
 * @param i_rState State
 * @return Envelope properties
 * @pre \c i_rState is valid
 */
ConvectiveEnvelope::Envelope ConvectiveEnvelope::Compute( const Herd::SSE::EvolutionState& i_rState )
{
  Herd::SSE::ValidateEvolutionState( i_rState );

  auto& rTrackPoint = i_rState.m_TrackPoint;

  // Remnants do not have an envelope
  if( Herd::SSE::IsRemnant( rTrackPoint.m_Stage ) )
  {
    return
    {};
  }

  ComputeInitialMassDependents( DetermineInitialMass( i_rState ) );

  double tauEnv = ComputeProximityToHayashi( i_rState );
  auto [ fractionalM, fractionalR ] = ComputeMassAndRadius( i_rState, tauEnv );

  ConvectiveEnvelope::Envelope Output;
  Output.m_Mass.Set( std::max( 1e-10, fractionalM * ( rTrackPoint.m_Mass - rTrackPoint.m_CoreMass ) ) );
  Output.m_Radius.Set( std::max( 1e-10, fractionalR * ( rTrackPoint.m_Radius - i_rState.m_CoreRadius ) ) );

  Output.m_RadiusOfGyration = ComputeRadiusOfGyration( i_rState, tauEnv );

  Output.m_K2 = ComputeK2( i_rState, tauEnv );

  return Output;
}

/**
 * @param i_Mass Initial mass
 */
void ConvectiveEnvelope::ComputeInitialMassDependents( Herd::Generic::Mass i_Mass )
{
  if( i_Mass == m_M0Dependents.m_EvaluatedAt )
  {
    return;
  }

  m_M0Dependents.m_EvaluatedAt = i_Mass;

  double logM = std::log10( i_Mass );
  m_M0Dependents.m_LogM0 = logM;

  m_M0Dependents.m_A = std::clamp( 0.68 + 0.4 * logM, 0.68, 0.81 );
  m_M0Dependents.m_C = std::clamp( -2.5 + 5 * logM, -2.5, -1.5 );

  {
    double x = std::clamp( ( 0.1 - logM ) / 0.55, 0., 1. );
    double x5 = boost::math::pow< 5 >( x );
    m_M0Dependents.m_MCEZAMS.Set( 0.18 * x + 0.82 * x5 );
    m_M0Dependents.m_RCEZAMS.Set( 0.4 * std::pow( x, 0.25 ) + 0.6 * x5 * x5 );

    m_M0Dependents.m_Y = 2. + 8. * x;
  }

  m_M0Dependents.m_RGZAMS.Set( std::min( 0.21, std::max( 0.09 - 0.27 * logM, 0.037 + 0.033 * logM ) ) );
  if( logM >= 1.3 )
  {
    m_M0Dependents.m_RGZAMS -= Herd::Generic::Radius( 0.055 * boost::math::pow< 2 >( logM - 1.3 ) );
  }

  m_M0Dependents.m_RGBGB.Set( std::min( { 0.15, 0.147 + 0.03 * logM, 0.162 - 0.04 * logM } ) );

  m_M0Dependents.m_K2ZAMS = std::min( 0.21, std::max( 0.09 - 0.27 * logM, 0.037 + 0.033 * logM ) );
  if( logM > 1.3 )
  {
    m_M0Dependents.m_K2ZAMS -= 0.055 * boost::math::pow< 2 >( logM - 1.3 );
  }

  m_M0Dependents.m_K2BGB = std::min( { 0.15, 0.147 + 0.03 * logM, 0.162 - 0.04 * logM } );
}

/**
 * @param i_rState State
 * @return Effective initial mass
 */
Herd::Generic::Mass ConvectiveEnvelope::DetermineInitialMass( const Herd::SSE::EvolutionState& i_rState )
{
  auto stage = i_rState.m_TrackPoint.m_Stage;

  // For MS and HeMS, the effective initial mass is the current mass
  if( Herd::SSE::IsMS( stage ) || stage == Herd::SSE::EvolutionStage::e_HeMS )
  {
    return i_rState.m_TrackPoint.m_Mass;
  }

  if( stage == Herd::SSE::EvolutionStage::e_HG )
  {
    if( i_rState.m_TrackPoint.m_CoreMass > i_rState.m_MCHeI || i_rState.m_MZAMS > i_rState.m_MFGB )
    {
      return i_rState.m_MZAMS;
    } else
    {
      return i_rState.m_TrackPoint.m_Mass;
    }
  }

  // Between HG and HeMS
  if( Herd::SSE::IsAGB( stage ) || stage == Herd::SSE::EvolutionStage::e_FGB || stage == Herd::SSE::EvolutionStage::e_CHeB )
  {
    return i_rState.m_MZAMS;
  }

  // After HeMS until remnant
  return i_rState.m_MZHe;
}

/**
 * @param i_rState State
 * @return Proximity to the Hayashi track
 */
double ConvectiveEnvelope::ComputeProximityToHayashi( const Herd::SSE::EvolutionState& i_rState )
{
  auto& rTrackPoint = i_rState.m_TrackPoint;

  double x = 0;
  if( Herd::SSE::IsPreFGB( rTrackPoint.m_Stage ) )
  {
    Herd::Generic::Temperature teBGB = Herd::Physics::ComputeAbsoluteTemperature( i_rState.m_LBGB, i_rState.m_Rg );
    x = teBGB / rTrackPoint.m_Temperature;
  } else
  {
    x = std::sqrt( rTrackPoint.m_Radius / i_rState.m_Rg );
  }

  return std::clamp( ComputeBlendWeight( x, m_M0Dependents.m_A, 1. ), 0., 1. );
}

/**
 * @param i_rState State
 * @param i_TauEnv Proximity to the Hayashi track
 * @return Radius of gyration, in \f$ R_{\odot}\f$
 */
Herd::Generic::Radius ConvectiveEnvelope::ComputeRadiusOfGyration( const Herd::SSE::EvolutionState& i_rState, double i_TauEnv )
{
  auto& rTrackPoint = i_rState.m_TrackPoint;
  auto stage = rTrackPoint.m_Stage;

  double rGG = m_M0Dependents.m_RGBGB;  // Giant-like radius of gyration

  // FGB to AGB
  if( stage == Herd::SSE::EvolutionStage::e_FGB || stage == Herd::SSE::EvolutionStage::e_CHeB || Herd::SSE::IsAGB( stage ) )
  {
    double logM = std::log10( rTrackPoint.m_Mass );
    double logM20 = logM * logM;
    double f = 0.208 + 0.125 * logM - 0.035 * logM20;

    double m15 = rTrackPoint.m_Mass * std::sqrt( rTrackPoint.m_Mass );
    double x05 = ( rTrackPoint.m_Luminosity - i_rState.m_LBGB ) / ( 10000. * m15 / ( 1. + 0.1 * m15 ) );
    double x = x05 * x05;

    double y = ( f - 0.033 * std::log10( i_rState.m_LBGB ) ) / m_M0Dependents.m_RGBGB - 1.;
    rGG = ( f - 0.033 * std::log10( rTrackPoint.m_Luminosity ) + 0.4 * x ) / ( 1 + y * ( i_rState.m_LBGB / rTrackPoint.m_Luminosity ) + x );
  }

  // HeGB
  if( stage == Herd::SSE::EvolutionStage::e_HeGB )
  {
    double m15 = rTrackPoint.m_Mass * std::sqrt( rTrackPoint.m_Mass );
    double x05 = std::max( 0., rTrackPoint.m_Luminosity / ( 30000. * m15 ) - 0.5 );
    double x = x05 * x05;
    rGG = ( m_M0Dependents.m_RGBGB + 0.4 * x ) / ( 1. + 0.4 * x );
  }

  double rG = rGG;

  // Radius of gyration for stars not on the Hayashi track
  if( rTrackPoint.m_Radius < i_rState.m_Rg )
  {
    // Up to He stars
    if( Herd::SSE::IsPreFGB( stage ) || stage == Herd::SSE::EvolutionStage::e_FGB || stage == Herd::SSE::EvolutionStage::e_CHeB || Herd::SSE::IsAGB( stage ) )
    {
      double radiusRatio = rTrackPoint.m_Radius / i_rState.m_RZAMS;
      rG = ( m_M0Dependents.m_RGZAMS - 0.025 ) * std::pow( radiusRatio, m_M0Dependents.m_C ) + 0.025 * std::pow( radiusRatio, -0.1 );
    }

    if( stage == Herd::SSE::EvolutionStage::e_HeMS )
    {
      rG = 0.08 - 0.03 * ( i_rState.m_EffectiveAge / i_rState.m_TMS );
    }

    if( stage == Herd::SSE::EvolutionStage::e_HeHG || stage == Herd::SSE::EvolutionStage::e_HeGB )
    {
      rG = 0.08 * i_rState.m_RZAMS / rTrackPoint.m_Radius;
    }

    if( i_TauEnv > 0. )
    {
      double tauEnv30 = boost::math::pow< 3 >( i_TauEnv );
      if( Herd::SSE::IsMS( stage ) )
      {
        double tau = i_rState.m_EffectiveAge / i_rState.m_TMS;
        rG += std::pow( tau, m_M0Dependents.m_Y ) * tauEnv30 * ( rGG - rG );
      } else
      {
        rG += tauEnv30 * ( rGG - rG );
      }
    }
  }
  
  return Herd::Generic::Radius( rG );
}

/**
 * @param i_rState State
 * @param i_TauEnv Proximity to the Hayashi track
 * @return M and R of the convective envelope, relative to the entire envelope
 */
std::pair< double, double > ConvectiveEnvelope::ComputeMassAndRadius( const Herd::SSE::EvolutionState& i_rState, double i_TauEnv )
{
  auto& rTrackPoint = i_rState.m_TrackPoint;

  double mCEG = 1.; // Convective envelope mass fraction on the giant branch
  double rCEG = 1.; // Convective envelope radius fraction on the giant branch

  if( Herd::SSE::IsPreFGB( rTrackPoint.m_Stage ) )
  {
    // At BGB
    mCEG = 0.5;
    rCEG = 0.65;
  }

  // Fresh FGB stars are yet to develop a fully-convective envelope
  if( rTrackPoint.m_Stage == Herd::SSE::EvolutionStage::e_FGB && rTrackPoint.m_Luminosity < 3. * i_rState.m_LBGB )
  {
    double x = std::min( 3., i_rState.m_LHeI.Value() / i_rState.m_LBGB );
    double tau = std::clamp( ComputeBlendWeight( rTrackPoint.m_Luminosity / i_rState.m_LBGB, x, 1. ), 0., 1. );
    mCEG = 1. - 0.5 * tau * tau;
    rCEG = 1. - 0.35 * tau * tau;
  }

  // Assume giant
  double mCE = mCEG;  // Fractional mass of the convective envelope
  double rCE = rCEG;  // Fractional radius of the convective envelope

  auto MassRelation = [ & ]( auto i_Tau )
  { return mCEG * boost::math::pow<5>( i_Tau);};
  auto RadiusRelation = [ & ]( auto i_Tau )
  { return rCEG * i_Tau * std::pow( i_Tau, 0.25);};
  
  // Stars not on the Hayashi track
  if( rTrackPoint.m_Radius < i_rState.m_Rg )
  {
    // Correction for stars close to the Hayashi track
    if( i_TauEnv > 0. )
    {
      mCE = MassRelation( i_TauEnv );
      rCE = RadiusRelation( i_TauEnv );
      if( Herd::SSE::IsMS( rTrackPoint.m_Stage ) )
      {
        //ComputeProximityToHayashi evaluated at teTMS
        Herd::Generic::Temperature teBGB = Herd::Physics::ComputeAbsoluteTemperature( i_rState.m_LBGB, i_rState.m_Rg );
        Herd::Generic::Temperature teTMS = Herd::Physics::ComputeAbsoluteTemperature( i_rState.m_LTMS, i_rState.m_RTMS );
        double tauTMS = std::clamp( ComputeBlendWeight( teBGB / teTMS, m_M0Dependents.m_A, 1. ), 0., 1. );

        if( tauTMS > 0. )
        {
          // CE at HG
          double mCEHG = MassRelation( tauTMS );
          double rCEHG = RadiusRelation( tauTMS );

          double tauhy = std::pow( i_rState.m_EffectiveAge / i_rState.m_TMS, m_M0Dependents.m_Y );
          mCE = m_M0Dependents.m_MCEZAMS + tauhy * mCE * ( 1. - m_M0Dependents.m_MCEZAMS / mCEHG );
          rCE = m_M0Dependents.m_RCEZAMS + tauhy * rCE * ( 1. - m_M0Dependents.m_RCEZAMS / rCEHG );
        }
        else
        {
          mCE = 0.;
          rCE = 0;
        }
      }
    } else
    {
      mCE = 0.;
      rCE = 0;
    }
  }

  return std::pair( mCE, rCE ); // @suppress("Ambiguous problem")
}

double ConvectiveEnvelope::ComputeK2( const Herd::SSE::EvolutionState& i_rState, double i_TauEnv )
{
  // Compute k2g
  const auto& rTrackPoint = i_rState.m_TrackPoint;
  double k2g = m_M0Dependents.m_K2BGB;
  double m15 = std::sqrt( boost::math::pow< 3 >( rTrackPoint.m_Mass ) );
  double logM = std::log10( rTrackPoint.m_Mass );
  
  if( rTrackPoint.m_Stage == Herd::SSE::EvolutionStage::e_CHeB || rTrackPoint.m_Stage == Herd::SSE::EvolutionStage::e_FGB
      || Herd::SSE::IsAGB( rTrackPoint.m_Stage ) )
  {
    double b = ( 1e4 * m15 ) / ( 1 + 0.1 * m15 );
    double x = boost::math::pow< 2 >( ( rTrackPoint.m_Luminosity - i_rState.m_LBGB ) / b );

    double f = 0.208 + 0.125 * logM - 0.035 * boost::math::pow< 2 >( logM );
    double y = ( f - 0.33 * log10( i_rState.m_LBGB ) + 0.4 * x ) / m_M0Dependents.m_K2BGB - 1.0;

    k2g = ( f - 0.33 * log10( rTrackPoint.m_Luminosity ) + 0.4 * x ) / ( 1.0 + y * ( i_rState.m_LBGB / rTrackPoint.m_Luminosity ) + x );
  }

  if( rTrackPoint.m_Stage == Herd::SSE::EvolutionStage::e_HeGB )
  {
    double b = 3e4 * m15;
    double x = boost::math::pow< 2 >( std::max( 0.0, rTrackPoint.m_Luminosity / ( b - 0.5 ) ) );
    k2g = ( m_M0Dependents.m_K2BGB + 0.4 * x ) / ( 1.0 + 0.4 * x );
  }


  // Compute k2
  double k2 = k2g;
  if( rTrackPoint.m_Radius < i_rState.m_Rg )
  {
    // AGB or earlier
    if( !Herd::SSE::IsHeStar( rTrackPoint.m_Stage ) || !Herd::SSE::IsRemnant( rTrackPoint.m_Stage ) )
    {
      double relativeR = rTrackPoint.m_Radius / i_rState.m_RZAMS;
      double term1 = Herd::SSE::BXhC( relativeR, m_M0Dependents.m_K2ZAMS - 0.025, m_M0Dependents.m_C );
      double term2 = Herd::SSE::BXhC( relativeR, 0.025, -0.1 );
      k2 = term1 + term2;
    }

    if( rTrackPoint.m_Stage == Herd::SSE::EvolutionStage::e_HeMS )
    {
      double tau = i_rState.m_EffectiveAge / i_rState.m_THeMS;
      k2 = 0.08 - 0.03 * tau;
    }

    if( rTrackPoint.m_Stage == Herd::SSE::EvolutionStage::e_HeHG || rTrackPoint.m_Stage == Herd::SSE::EvolutionStage::e_HeGB )
    {
      k2 = 0.08 * i_rState.m_RZAMS / rTrackPoint.m_Radius;
    }

    if( i_TauEnv > 0.0 )
    {
      if( Herd::SSE::IsMS( rTrackPoint.m_Stage ) )
      {
        double tau = i_rState.m_EffectiveAge / i_rState.m_TMS;
        double x = std::clamp( ( 0.1 - logM ) / 0.55, 0., 1. );
        double y = 2.0 + 8.0 * x;
        k2 += std::pow( tau, y ) * boost::math::pow< 3 >( i_TauEnv ) * ( k2g - k2 );
      }
      else
      {
        k2 += boost::math::pow< 3 >( i_TauEnv ) * ( k2g - k2 );
      }
    }
  }

  return k2;
}

}

