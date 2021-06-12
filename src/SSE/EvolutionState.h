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

#include "Generic/Quantities.h"

namespace Herd::SSE
{
/**
 * @brief State of a star
 */
struct EvolutionState
{
  Herd::Generic::Mass m_Mass;	///< Mass in \f$ M_{\odot}\f$
  Herd::Generic::Metallicity m_Z; ///< Metallicity
  Herd::Generic::Radius m_Radius;	///< Radius in \f$ R_{\odot}\f$
  Herd::Generic::Luminosity m_Luminosity;	///< Luminosity in \f$ L_{\odot}\f$
  Herd::Generic::Temperature m_Temperature;	///< Temperature in K
  Herd::Generic::Age m_Age;	///< Age in million years
};

}

#endif /* H633A09E8_2301_41B3_8E30_CFE3C37DFCF3 */
