/**
 * @file StellarRotation.h
 * @author Evren Imre
 * @date 24 Jul 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef H81F297AE_5FFA_4056_826B_6C032B3D3051
#define H81F297AE_5FFA_4056_826B_6C032B3D3051

#include <Generic/Quantity.h>

namespace Herd::SSE
{
// Forward declarations
class EvolutionState;
class TrackPoint;

/**
 * @brief Stellar rotation computations
 * @cite Hurley00
 * @cite AMUSE
 */
class StellarRotation
{
public:

  static void InitialiseAtZAMS( Herd::SSE::EvolutionState& io_rState ); ///< Computes the angular momentum at ZAMS
  static void InitialiseAtNSOrBH( Herd::SSE::EvolutionState& io_rState );  ///< Computes the angular momentum for a new neutron star or black hole
  static double ComputeAngularMomentumLossRate( const Herd::SSE::EvolutionState& i_rState );  ///< Computes the angular momentum loss per year

private:

  static Herd::Generic::AngularVelocity ComputeInitialAngularVelocity( const Herd::SSE::TrackPoint& i_rTrackPoint ); ///< Computes the initial angular velocity
  static Herd::Generic::AngularVelocity ComputeAngularVelocity( const Herd::SSE::EvolutionState& i_rState ); ///< Computes the angular velocity
  static double ComputeMomentOfInertia( const Herd::SSE::EvolutionState& i_rState ); ///< Computes the moment of inertia
  static double ComputeStellarWindLoss( const Herd::SSE::EvolutionState& i_rState, Herd::Generic::AngularVelocity i_AngularVelocity ); ///< Computes the momentum loss due to mass loss
  static double ComputeMagneticBrakingLoss( const Herd::SSE::TrackPoint& i_rTrackPoint, Herd::Generic::AngularVelocity i_AngularVelocity ); ///< Computes the momentum loss due to magnetic braking
};
}

#endif /* H81F297AE_5FFA_4056_826B_6C032B3D3051 */
