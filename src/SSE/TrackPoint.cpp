/**
 * @file TrackPoint.cpp
 * @author Evren Imre
 * @date 25 Jul 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "TrackPoint.h"

#include <Exceptions/ExceptionWrappers.h>

#include <iostream>

namespace Herd::SSE
{
/**
 * @param i_rTrackPoint Track point to be validated
 * @pre All numerical members are non-negative
 * @pre The evolution stage is not undefined
 * @pre Sum of the core and the envelope masses does not exceed the total mass
 * @throws PreconditionError If any preconditions are violated
 * @remarks The function does not test whether a track point belongs to a physically viable star
 */
void ValidateTrackPoint( const TrackPoint& i_rTrackPoint )
{

  Herd::Generic::ThrowIfNegative( i_rTrackPoint.m_Mass, "m_Mass" );
  Herd::Generic::ThrowIfNegative( i_rTrackPoint.m_InitialMetallicity, "m_Z" );
  Herd::Generic::ThrowIfNegative( i_rTrackPoint.m_Luminosity, "m_Luminosity" );
  Herd::Generic::ThrowIfNegative( i_rTrackPoint.m_Radius, "m_Radius" );
  Herd::Generic::ThrowIfNegative( i_rTrackPoint.m_Temperature, "m_Temperature" );
  Herd::Generic::ThrowIfNegative( i_rTrackPoint.m_Age, "m_Age" );
  Herd::Generic::ThrowIfNegative( i_rTrackPoint.m_CoreMass, "m_CoreMass" );
  Herd::Generic::ThrowIfNegative( i_rTrackPoint.m_EnvelopeMass, "m_EnvelopeMass" );
  Herd::Generic::ThrowIfNegative( i_rTrackPoint.m_AngularMomentum, "m_AngularMomentum" );

  // Core + Envelope mass can be smaller than the total mass. See the discussion in Sec 7.2
  if( i_rTrackPoint.m_CoreMass.Value() + i_rTrackPoint.m_EnvelopeMass.Value() > i_rTrackPoint.m_Mass.Value() )
  {

    std::cout << i_rTrackPoint.m_CoreMass.Value() << " " << i_rTrackPoint.m_EnvelopeMass.Value() << " " << i_rTrackPoint.m_Mass.Value() << "\n";
    
    [[unlikely]] Herd::Exceptions::ThrowPreconditionError( "m_CoreMass + m_EnvelopeMass", "<=m_Mass",
        i_rTrackPoint.m_CoreMass.Value() + i_rTrackPoint.m_EnvelopeMass.Value() );
  }

  if( i_rTrackPoint.m_Stage == Herd::SSE::EvolutionStage::e_Undefined )
  {
    [[unlikely]] Herd::Exceptions::ThrowPreconditionError( "m_Stage", "valid stage", "e_Undefined" );
  }

}

}

