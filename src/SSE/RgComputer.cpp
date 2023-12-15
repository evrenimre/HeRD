/**
 * @file RgComputer.cpp
 * @author Evren Imre
 * @date 12 Dec 2023	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2023 Evren Imre
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "RgComputer.h"

#include <SSE/Landmarks/BaseOfGiantBranch.h>

namespace Herd::SSE
{

/**
 * @param i_Z Metallicity
 */
RgComputer::RgComputer( Herd::Generic::Metallicity i_Z )
{
  m_ZDependents.m_pBGBComputer = std::make_unique< Herd::SSE::BaseOfGiantBranch >( i_Z );
}

/**
 * @param i_rState State
 * @return \f$ R_g \f$
 * @pre \c i_rState is valid
 */
Herd::Generic::Radius RgComputer::ComputeRg( const Herd::SSE::EvolutionState& i_rState )
{
  Herd::SSE::ValidateEvolutionState( i_rState );

  const auto& rTrackPoint = i_rState.m_TrackPoint;
  if( rTrackPoint.m_Stage == Herd::SSE::EvolutionStage::e_MS || rTrackPoint.m_Stage == Herd::SSE::EvolutionStage::e_MSLM )
  {
    return m_ZDependents.m_pBGBComputer->Radius( rTrackPoint.m_Mass );
  }
  else
  {
    return Herd::Generic::Radius( 0.1 );  // Placeholder. 0.1, as unit tests can fail at 0
  }
}
}



