/**
 * @file StellarRotation.cpp
 * @author Evren Imre
 * @date 24 Jul 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "StellarRotation.h"

#include "EvolutionStage.h"
#include "EvolutionState.h"
#include "TrackPoint.h"

#include <Exceptions/ExceptionWrappers.h>

#include <cmath>

#include <boost/math/special_functions/pow.hpp>

namespace Herd::SSE
{

/**
 * @param io_rState State
 * @pre \c io_rState.m_TrackPoint is at ZAMS
 * @throws PreconditionError if not at ZAMS
 * @post TrackPoint::m_AngularMomentum is initialised
 */
void StellarRotation::InitialiseAtZAMS( Herd::SSE::EvolutionState& io_rState )
{
  if( io_rState.m_TrackPoint.m_Age > 0. )
  {
    Herd::Exceptions::ThrowPreconditionError( "m_Age", "0", std::to_string( io_rState.m_TrackPoint.m_Age.Value() ) );
  }

  Herd::SSE::ValidateEvolutionState( io_rState );

  Herd::Generic::AngularVelocity angularVelocity = ComputeInitialAngularVelocity( io_rState.m_TrackPoint );
  io_rState.m_TrackPoint.m_AngularVelocity = angularVelocity;

  double momentOfInertia = ComputeMomentOfInertia( io_rState );
  io_rState.m_AngularMomentum.Set( momentOfInertia * angularVelocity ); // Eq. 109
}

/**
 * @param io_rState State
 * @pre \c io_rState.m_TrackPoint is at NS or BH
 * @throws PreconditionError if not a neutron star or a black hole
 * @post TrackPoint::m_AngularMomentum is initialised
 */
void StellarRotation::InitialiseAtNSOrBH( Herd::SSE::EvolutionState& io_rState )
{
  Herd::SSE::ValidateEvolutionState( io_rState );
  if( ( io_rState.m_TrackPoint.m_Stage != Herd::SSE::EvolutionStage::e_BH ) && ( io_rState.m_TrackPoint.m_Stage != Herd::SSE::EvolutionStage::e_NS ) )
  {
    Herd::Exceptions::ThrowPreconditionError( "m_Stage", "e_BH or e_NS", Herd::SSE::EvolutionStageToString( io_rState.m_TrackPoint.m_Stage ) );
  }

  Herd::Generic::AngularVelocity angularVelocity( 2e8 );
  io_rState.m_TrackPoint.m_AngularVelocity = angularVelocity;

  double momentOfInertia = ComputeMomentOfInertia( io_rState );
  io_rState.m_AngularMomentum.Set( momentOfInertia * angularVelocity );
}

/**
 * @param i_rState State
 * @return Angular momentum loss per year, in \f$ M_{\odot}R_{\odot}^2 \; {year}^{-2}\f$
 */
double StellarRotation::ComputeAngularMomentumLossRate( const Herd::SSE::EvolutionState& i_rState )
{
  Herd::SSE::ValidateEvolutionState( i_rState );

  double dJwind = ComputeStellarWindLoss( i_rState );
  double dJmb = ComputeMagneticBrakingLoss( i_rState.m_TrackPoint );

  return dJwind + dJmb;
}

/***
 * @param i_rTrackPoint Track point
 * @return Initial angular velocity
 */
Herd::Generic::AngularVelocity StellarRotation::ComputeInitialAngularVelocity( const Herd::SSE::TrackPoint& i_rTrackPoint )
{
  double m330 = std::pow( i_rTrackPoint.m_Mass, 3.3 );
  double m345 = std::pow( i_rTrackPoint.m_Mass, 3.45 );
  double tangentialVelocity = 330 * m330 / ( 15 + m345 );  // Eq. 107
  double angularVelocity = 45.35 * tangentialVelocity / i_rTrackPoint.m_Radius; // Eq. 108

  return Herd::Generic::AngularVelocity( angularVelocity );
}

/**
 * @param i_rState State
 * @return Angular velocity
 */
Herd::Generic::AngularVelocity StellarRotation::ComputeAngularVelocity( const Herd::SSE::EvolutionState& i_rState )
{
  Herd::SSE::ValidateEvolutionState( i_rState );

  double momentOfIntertia = ComputeMomentOfInertia( i_rState );
  double angularVelocity = i_rState.m_AngularMomentum / momentOfIntertia;
  return Herd::Generic::AngularVelocity( angularVelocity );
}

/**
 * @param i_rState State
 * @return Moment of inertia
 */
double StellarRotation::ComputeMomentOfInertia( const Herd::SSE::EvolutionState& i_rState )
{
  auto& rTrackPoint = i_rState.m_TrackPoint;

  double r2 = boost::math::pow< 2, double >( rTrackPoint.m_Radius );
  double rc2 = boost::math::pow< 2, double >( i_rState.m_CoreRadius );

  return i_rState.m_K2 * r2 * ( rTrackPoint.m_Mass - rTrackPoint.m_CoreMass ) + 0.21 * rc2 * ( rTrackPoint.m_CoreMass ); // Eq. 109. k2 is 0.1 in the paper, but 0.15 in AMUSE.SSE
}

/**
 * @param i_rState State
 * @return Momentum loss rate due to mass loss
 */
double StellarRotation::ComputeStellarWindLoss( const Herd::SSE::EvolutionState& i_rState )
{
  auto& rTrackPoint = i_rState.m_TrackPoint;

  double r2 = boost::math::pow< 2, double >( rTrackPoint.m_Radius );
  return ( 2. / 3. ) * i_rState.m_MassLossRate * r2 * rTrackPoint.m_AngularVelocity;  // Eq. 110
}

/**
 * @param i_rTrackPoint Track point
 * @return Momentum loss rate due to magnetic braking
 */
double StellarRotation::ComputeMagneticBrakingLoss( const Herd::SSE::TrackPoint& i_rTrackPoint )
{

  if( Herd::SSE::IsRemnant( i_rTrackPoint.m_Stage ) || i_rTrackPoint.m_Mass <= 0.35 )
  {
    [[unlikely]] return 0;
  }

  double rw3 = boost::math::pow< 3 >( i_rTrackPoint.m_Radius.Value() * i_rTrackPoint.m_AngularVelocity.Value() );
  return 5.83e-16 * ( i_rTrackPoint.m_EnvelopeMass / i_rTrackPoint.m_Mass ) * rw3;  // Eq. 111
}

}

