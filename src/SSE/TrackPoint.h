/**
 * @file TrackPoint.h
 * @author Evren Imre
 * @date 14 Jun 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef H8AB9F35C_A33A_4D5D_8F9A_CB81639D74B5
#define H8AB9F35C_A33A_4D5D_8F9A_CB81639D74B5

#include "EvolutionStage.h"

#include <Generic/Quantity.h>

namespace Herd::SSE
{
/**
 * @brief Represents a track point in an evolutionary track
 */
struct TrackPoint
{
  Herd::Generic::Mass m_Mass;  ///< Mass in \f$ M_{\odot}\f$
  Herd::Generic::Metallicity m_InitialMetallicity; ///< Initial metallicity
  Herd::Generic::Radius m_Radius; ///< Radius in \f$ R_{\odot}\f$
  Herd::Generic::Luminosity m_Luminosity;  ///< Luminosity in \f$ L_{\odot}\f$
  Herd::Generic::Temperature m_Temperature; ///< Effective surface temperature in K
  Herd::Generic::Age m_Age; ///< Age from ZAMS, in million years
  Herd::SSE::EvolutionStage m_Stage = Herd::SSE::EvolutionStage::e_Undefined;

  Herd::Generic::Mass m_CoreMass; ///< Core mass in \f$ M_{\odot}\f$
  Herd::Generic::Mass m_EnvelopeMass; ///< Convective envelope mass in \f$ M_{\odot}\f$
  Herd::Generic::AngularMomentum m_AngularMomentum; ///< Angular momentum in \f$ M_{\odot}R_{\odot}^2 \; {year}^{-1}\f$
};

void ValidateTrackPoint( const TrackPoint& i_rTrackPoint ); ///< Validates a track point
}

#endif /* H8AB9F35C_A33A_4D5D_8F9A_CB81639D74B5 */
