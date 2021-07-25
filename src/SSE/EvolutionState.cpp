/**
 * @file EvolutionState.cpp
 * @author Evren Imre
 * @date 25 Jul 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "EvolutionState.h"

#include <Exceptions/ExceptionWrappers.h>

namespace Herd::SSE
{

/**
 * @param i_rState
 * @pre EvolutionState::m_TrackPoint is valid
 * @pre All numerical values are non-negative
 * @throws PreconditionError If any preconditions are violated
 */
void ValidateEvolutionState( const EvolutionState& i_rState )
{
  Herd::SSE::ValidateTrackPoint( i_rState.m_TrackPoint );
  Herd::Generic::ThrowIfNegative( i_rState.m_InitialMass, "m_InitialMass" );  // Current mass can be higher than the initial mass due to mass gain in a binary
  Herd::Generic::ThrowIfNegative( i_rState.m_CoreRadius, "m_CoreRadius" );
  Herd::Generic::ThrowIfNegative( i_rState.m_MassLossRate, "m_MassLossRate" );
  Herd::Generic::ThrowIfNegative( i_rState.m_AngularMomentumLossRate, "m_AngularMomentumLossRate" );

  if( i_rState.m_CoreRadius.Value() > i_rState.m_TrackPoint.m_Radius.Value() )
  {
    Herd::Exceptions::ThrowPreconditionError( "m_CoreRadus", "<=m_Radius", i_rState.m_CoreRadius.Value() );
  }
}

}

