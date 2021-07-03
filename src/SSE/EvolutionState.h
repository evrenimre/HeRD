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

#include "EvolutionStages.h"
#include <Generic/Quantities.h>

#include <boost/container/flat_map.hpp>
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

  Herd::Generic::Mass m_InitialMass;  ///< Mass at the beginning of evolution in \f$ M_{\odot}\f$
};

}

#endif /* H633A09E8_2301_41B3_8E30_CFE3C37DFCF3 */