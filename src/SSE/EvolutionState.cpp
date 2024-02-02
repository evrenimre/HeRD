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
  Herd::Generic::ThrowIfNegative( i_rState.m_EffectiveAge, "m_EffectiveAge" );
  Herd::Exceptions::ThrowPreconditionErrorIfNegative( i_rState.m_MassLossRate, "m_MassLossRate" );

  Herd::Generic::ThrowIfNegative( i_rState.m_AngularMomentum, "m_AngularMomentum" );
  Herd::Exceptions::ThrowPreconditionErrorIfNegative( i_rState.m_K2, "m_K2" );

  Herd::Generic::ThrowIfNegative( i_rState.m_CoreRadius, "m_CoreRadius" );
  if( i_rState.m_CoreRadius > i_rState.m_TrackPoint.m_Radius )
  {
    Herd::Exceptions::ThrowPreconditionError( "m_CoreRadus", "<=m_Radius", i_rState.m_CoreRadius.Value() );
  }

  Herd::Generic::ThrowIfNotPositive( i_rState.m_MZAMS, "m_MZAMS" );

  Herd::Generic::ThrowIfNotPositive( i_rState.m_MFGB, "m_MFGB" );

  Herd::Generic::ThrowIfNegative( i_rState.m_DeltaT, "m_DeltaT" );

  if( i_rState.m_TrackPoint.m_Stage >= Herd::SSE::EvolutionStage::e_HG )
  {
    Herd::Generic::ThrowIfNotPositive( i_rState.m_MCHeI, "m_MCHeI" );
  }

  if( i_rState.m_TrackPoint.m_Stage >= Herd::SSE::EvolutionStage::e_HeMS )
  {
    Herd::Generic::ThrowIfNotPositive( i_rState.m_MZHe, "m_MZHe" );
  }

  if( Herd::SSE::IsHeStar( i_rState.m_TrackPoint.m_Stage ) )
  {
    Herd::Generic::ThrowIfNotPositive( i_rState.m_THeMS, "m_THeMS" );

  }
}

}

