/**
 * @file SingleStarEvolution.cpp
 * @author Evren Imre
 * @date 15 Jun 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "SingleStarEvolution.h"

#include "EvolutionState.h"
#include "IPhase.h"
#include "MainSequence.h"
#include "StellarRotation.h"
#include "StellarWindMassLoss.h"

#include <Exceptions/ExceptionWrappers.h>
#include <Generic/Quantity.h>

#include <cmath>
#include <functional>

#include <range/v3/algorithm.hpp>
#include <range/v3/numeric.hpp>
#include <range/v3/view.hpp>

namespace Herd::SSE
{

/**
 * @param i_Mass Initial mass in \f$ M_{\odot}\f$
 * @param i_Z Metallicity
 * @param i_EvolveUntil Evolve until this age
 * @param i_rParameters %Parameters
 * @pre \c i_rParameters is valid
 * @pre \c i_Mass within SingleStarEvolutuionSpecs::s_MassRange
 * @pre \c i_Z within SingleStarEvolutuionSpecs::s_MetallicityRange
 * @pre \c i_EvolveUntil >= 0
 */
void SingleStarEvolutuion::Evolve( Herd::Generic::Mass i_Mass, Herd::Generic::Metallicity i_Z, Herd::Generic::Time i_EvolveUntil,
    const Parameters& i_rParameters )
{
  Validate( i_rParameters );
  Validate( i_Mass, i_Z, i_EvolveUntil );

  m_Trajectory.reserve( EstimateTrajectoryLength( i_rParameters ) );

  // ZAMS
  Herd::SSE::EvolutionState state;
  state.m_MZAMS = i_Mass;
  auto& rTrackPoint = state.m_TrackPoint;
  rTrackPoint.m_Mass = i_Mass;

  Herd::SSE::MainSequence ms( i_Z );
  ms.Evolve( state ); // Call at age zero initialises the state to ZAMS
  Herd::SSE::StellarRotation::InitialiseAtZAMS( state );
  m_Trajectory.push_back( rTrackPoint );

  while( rTrackPoint.m_Age < i_EvolveUntil )
  {
    Herd::Generic::Time TerminateAt = state.m_TMS; // This is a temporary variable, set at the end of the most advanced stage implemented so far
    if( rTrackPoint.m_Age >= TerminateAt )
    {
      break;
    }

    // Mass and angular momentum loss rate between the previous step and the current step
    state.m_MassLossRate = Herd::SSE::StellarWindMassLoss::Compute( rTrackPoint, i_rParameters.m_Eta, i_rParameters.m_HeWind, i_rParameters.m_BinaryWind,
        i_rParameters.m_RocheLobe );
    double angularMomentumLossRate = Herd::SSE::StellarRotation::ComputeAngularMomentumLossRate( state ); // Momentum loss from the angular velocity at the previous time point

    // Compute the size of the time step
    Herd::Generic::Time DeltaT = ComputeTimestep( ms, state, i_rParameters, i_EvolveUntil );

    state.m_DeltaT = DeltaT;
    rTrackPoint.m_Age += DeltaT;
    rTrackPoint.m_Mass -= Herd::Generic::Mass( ( state.m_MassLossRate * 1.0e6 ) * DeltaT ); // 1e6 to convert loss in year to Myr
    state.m_AngularMomentum -= Herd::Generic::AngularMomentum( ( angularMomentumLossRate * 1.0e6 ) * DeltaT );

    // Run the evolution step
    // TODO Stage transition to be implemented
    bool bInMS = ms.Evolve( state );
    if( !bInMS )
    {
      break;
    }

    rTrackPoint.m_AngularVelocity = Herd::SSE::StellarRotation::ComputeAngularVelocity( state );

    m_Trajectory.push_back( rTrackPoint );
  }

  // TODO Correct the temperature: AMUSE.SSE and IAU use slightly different values. But do this only when all computations are finished. menv uses temperature ratios, so it is not affected

  // Loop
  // Compute the new mass
  // Compute the new spin
  // Seek to the correct segment on the HR curve, and evaluate
  // Update the HR trajectory

}

/**
 * @return A constant reference to SingleStarEvolutuion::m_Trajectory
 */
const std::vector< Herd::SSE::TrackPoint >& SingleStarEvolutuion::Trajectory() const
{
  return m_Trajectory;
}

/**
 * @param i_rParameters %Parameters to be validated
 * @throws PreconditionError
 */
void SingleStarEvolutuion::Validate( const Parameters& i_rParameters )
{
  Herd::Exceptions::ThrowPreconditionErrorIfNegative( i_rParameters.m_Eta, "m_Eta" ); // @suppress("Invalid arguments")
  Herd::Exceptions::ThrowPreconditionErrorIfNegative( i_rParameters.m_HeWind, "m_HeWind" ); // @suppress("Invalid arguments")
  Herd::Exceptions::ThrowPreconditionErrorIfNegative( i_rParameters.m_BinaryWind, "m_BinaryWind" ); // @suppress("Invalid arguments")
  Herd::Exceptions::ThrowPreconditionErrorIfNegative( i_rParameters.m_RocheLobe, "m_RocheLobe" ); // @suppress("Invalid arguments")
  Herd::Exceptions::ThrowPreconditionErrorIfNegative( i_rParameters.m_SupernovaKickDispersion, "m_SupernovaKickDispersion" ); // @suppress("Invalid arguments")

  if( ranges::cpp20::any_of( i_rParameters.m_RelativeTimeStepSizes | ranges::cpp20::views::values,
      std::bind( std::less_equal< double >(), std::placeholders::_1, 0. ) ) )
  {
    [[unlikely]] Herd::Exceptions::ThrowPreconditionError( "m_TimeSteps", "all elements >0", "at least one <=0" );
  }

  Herd::Exceptions::ThrowPreconditionErrorIfNegative( i_rParameters.m_DefaultTimestep, "m_DefaultTimestep" ); // @suppress("Invalid arguments")
  Herd::Exceptions::ThrowPreconditionErrorIfNegative( i_rParameters.m_MinRemnantTimestep, "m_MinRemnantTimestep" ); // @suppress("Invalid arguments")
}

/**
 * @param i_Mass Mass
 * @param i_Z Metallicity
 * @param i_EvolveUntil Evolution cut-off
 */
void SingleStarEvolutuion::Validate( Herd::Generic::Mass i_Mass, Herd::Generic::Metallicity i_Z, Herd::Generic::Time i_EvolveUntil )
{
  SingleStarEvolutuionSpecs::s_MassRange.ThrowIfNotInRange( i_Mass, "i_Mass" );  // Mass is within the allowed range
  SingleStarEvolutuionSpecs::s_MetallicityRange.ThrowIfNotInRange( i_Z, "i_Z" );  // Metallicity is within the allowed range

  Herd::Exceptions::ThrowPreconditionErrorIfNegative( i_EvolveUntil, "i_EvolveUntil" ); // @suppress("Invalid arguments")
}

unsigned int SingleStarEvolutuion::EstimateTrajectoryLength( const Parameters& i_rParameters )
{
  // Accumulate is not included in C++20. C++23 has fold-left
  return ranges::accumulate(
      i_rParameters.m_RelativeTimeStepSizes | ranges::cpp20::views::values
          | ranges::cpp20::views::transform( std::bind( std::divides< double >(), 1., std::placeholders::_1 ) ), 0 );
}

/**
 * @param[in, out] io_rPhase PEvolution phase simulator
 * @param i_rState Evolution state
 * @param i_rParameters Parameters
 * @param i_rEvolveUntil Evolution cut-off
 * @return Timestep in Myr
 */
Herd::Generic::Time SingleStarEvolutuion::ComputeTimestep( Herd::SSE::IPhase& i_rPhase, const Herd::SSE::EvolutionState i_rState,
    const Parameters& i_rParameters,
    Herd::Generic::Time i_EvolveUntil )
{
  // Absolute timestep size from the relative size
  const auto& rTrackPoint = i_rState.m_TrackPoint;

  const auto itQuery = i_rParameters.m_RelativeTimeStepSizes.find( rTrackPoint.m_Stage );
  double deltaPercentage = ( itQuery == i_rParameters.m_RelativeTimeStepSizes.end() ) ? 0. : itQuery->second;

  Herd::Generic::Time deltaT;
  Herd::Generic::Time endOfPhase;
  switch( rTrackPoint.m_Stage )
  {
    case Herd::SSE::EvolutionStage::e_MSLM:
      endOfPhase = i_rState.m_TMS;
      deltaT.Set( deltaPercentage * i_rState.m_TMS );
      break;

    case Herd::SSE::EvolutionStage::e_MS:
      endOfPhase = i_rState.m_TMS;
      deltaT.Set( deltaPercentage * i_rState.m_TMS );
      break;

    default:
      break;
  }

  Herd::Generic::Time remainingTime = endOfPhase - i_rState.m_EffectiveAge;  // Remaining time in the current phase

  // Limit the radius change to 10%
  // Compute the state at the next time point and limit the jump
  // Even with the mass loss, this is done at the current mass
  // When there is no mass loss, this computation is repeated redundantly in the main loop
  unsigned int iterationCount = 0;
  while( true )
  {
    Herd::SSE::EvolutionState ClonedState = i_rState;  // We want to preserve the original state

    bool bEndOfPhase = remainingTime - deltaT < 1e-10;
    if( bEndOfPhase )
    {
      // This is only for testing the radius condition without triggering a phase change. If the radius does not grow beyond 10% at this point, deltaT is set to remainingTime
      ClonedState.m_DeltaT.Set(
          std::max( i_rState.m_EffectiveAge.Value(), i_rState.m_EffectiveAge * ( 1.0 - 1e-6 ) + remainingTime ) - i_rState.m_EffectiveAge );
    }
    else
    {
      ClonedState.m_DeltaT = deltaT;
    }
    ClonedState.m_TrackPoint.m_Age += ClonedState.m_DeltaT;

    i_rPhase.Evolve( ClonedState );

    Herd::Generic::Radius NewRadius = ClonedState.m_TrackPoint.m_Radius;
    Herd::Generic::Radius OldRadius = i_rState.m_TrackPoint.m_Radius;
    Herd::Generic::Radius AbsDeltaRadius( std::abs( NewRadius - OldRadius ) );
    if( AbsDeltaRadius / OldRadius > 0.1 )
    {
      if( bEndOfPhase )
      {
        deltaT.Set( remainingTime - i_rState.m_EffectiveAge * 1e-6 );
      }

      deltaT *= Herd::Generic::Time( 0.09 * std::max( NewRadius, OldRadius ) / AbsDeltaRadius );

      if( !bEndOfPhase && iterationCount >= 20 )
      {
        deltaT.Set( deltaT / 2 );
      }
    } else
    {
      if( bEndOfPhase )
      {
        deltaT.Set( remainingTime );
      }
      break;
    }

    ++iterationCount;
  }


  // No mass loss from the core mass
  Herd::Generic::Mass massLoss( i_rState.m_MassLossRate * 1.0e6 * deltaT );

  if( !Herd::SSE::IsRemnant( rTrackPoint.m_Stage ) )
  {
    Herd::Generic::Mass nonCoreMass = rTrackPoint.m_Mass - rTrackPoint.m_CoreMass;
    if( nonCoreMass < massLoss )
    {
      deltaT.Set( deltaT.Value() * ( nonCoreMass / massLoss ) );
      massLoss = nonCoreMass;
    }
  }

  // Limit the mass change to %1
  double relativeMassChange = massLoss / rTrackPoint.m_Mass; // Multiply with 1e6 to convert to Myr
  if( relativeMassChange > 0.01 )
  {
    deltaT.Set( deltaT.Value() * ( 0.01 / relativeMassChange ) );
  }

  Herd::Generic::Time minStepSize( 1e-7 * i_rState.m_EffectiveAge ); // Minimum timestep prevents tiny updates due to incremental changes to tMS due to mass loss
  deltaT.Set( std::max( minStepSize, deltaT ) );
  deltaT.Set( std::min( deltaT, i_EvolveUntil - i_rState.m_TrackPoint.m_Age ) );

  return deltaT;
}

}

