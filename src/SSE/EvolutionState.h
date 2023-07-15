/**
 * @file EvolutionState.h
 * @author Evren Imre
 * @date 5 Nov 2020	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef H633A09E8_2301_41B3_8E30_CFE3C37DFCF3
#define H633A09E8_2301_41B3_8E30_CFE3C37DFCF3

#include <Generic/Quantity.h>

#include "TrackPoint.h"

namespace Herd::SSE
{

/**
 * @brief State of evolution
 * @remarks This is used for internal communication between various components
 * @cite Hurley00
 */
struct EvolutionState
{
  Herd::SSE::TrackPoint m_TrackPoint; ///< Track point

  Herd::Generic::Age m_EffectiveAge;  ///< Effective age to represent the ageing effect of mass loss
  Herd::Generic::Radius m_CoreRadius; ///< Radius of the core in \f$ R_{\odot} \f$
  Herd::Generic::Radius m_EnvelopeRadius; ///< Radius of the convective envelope in \f$ R_{\odot} \f$

  double m_MassLossRate = 0;  ///< Mass loss rate in \f$ M_{\odot} {year}^{-1}\f$

  Herd::Generic::AngularMomentum m_AngularMomentum; ///< Angular momentum in \f$ M_{\odot}R_{\odot}^2 \; {year}^{-1}\f$
  double m_K2 = 0.15; ///< Constant used for computing the angular momentum due to the envelope

  Herd::Generic::Mass m_MZAMS; ///< Mass at ZAMS. Fixed
  Herd::Generic::Mass m_MZHe; ///< Mass at zero-age He MS. Fixed

  Herd::Generic::Radius m_RZAMS;  ///< \f$ R_{ZAMS}\f$. Radius at ZAMS. Updated as the mass changes

  Herd::Generic::Mass m_MFGB;  ///< \f$ M_{FGB} \f$, Eq. 3. Maximum mass for a start to have a GB phase
  Herd::Generic::Radius m_RTMS;  ///< \f$ R_{TMS} \f$, Eq. 9. Radius at terminal main sequence
  Herd::Generic::Luminosity m_LTMS;  ///< \f$ R_{TMS} \f$, Eq. 8. Luminosity at terminal main sequence
  Herd::Generic::Age m_TMS; ///< \f$ t_{TMS}\f$, Eq. 5. Main sequence duration

  Herd::Generic::Radius m_Rg; ///< \f$ R_g \f$ required for convective envelope calculations
  Herd::Generic::Luminosity m_LBGB; ///< \f$ L_{BGB} \f$, Eq. 10. Luminosity at the base of the giant branch

  Herd::Generic::Mass m_MCHeI; ///< \f$ M_{c, HeI}\f$. Core mass at He ignition
  Herd::Generic::Luminosity m_LHeI;  ///< \f$ L_{HeI}\f$, Eq. 49. Luminosity at He ignition

  Herd::Generic::Age m_DeltaT; ///<  Timestep

  Herd::Generic::Age m_THeMS; ///< Naked He main sequence duration

};

void ValidateEvolutionState( const EvolutionState& i_rState );  ///< Validates an evolution state
}

#endif /* H633A09E8_2301_41B3_8E30_CFE3C37DFCF3 */
