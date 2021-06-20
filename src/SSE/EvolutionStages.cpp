/**
 * @file EvolutionStage.cpp
 * @author Evren Imre
 * @date 19 Jun 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "EvolutionStages.h"

namespace Herd::SSE
{
/**
 * @param i_Stage Evolution stage
 * @return \c true if one of the remnant stages
 */
bool IsRemnant( EvolutionStage i_Stage )
{
  // @formatter:off
  return i_Stage == EvolutionStage::e_HeWD
      || i_Stage == EvolutionStage::e_COWD
      || i_Stage == EvolutionStage::e_ONWD
      || i_Stage == EvolutionStage::e_NS
      || i_Stage == EvolutionStage::e_BH
      || i_Stage == EvolutionStage::e_MSn;
            // @formatter:on
} /**
   * @param i_Stage Evolution stage
   * @return \c true if one of the MS stages
   */
bool IsMS( EvolutionStage i_Stage )
{
  return i_Stage == EvolutionStage::e_MSLM || i_Stage == EvolutionStage::e_MS;
}

/**
 * @param i_Stage Evolution stage
 * @return \c true if one of the AGB stages
 */
bool IsAGB( EvolutionStage i_Stage )
{
  return i_Stage == EvolutionStage::e_FAGB || i_Stage == EvolutionStage::e_SAGB;
}

bool IsHeStar( EvolutionStage i_Stage )
{
  return i_Stage == EvolutionStage::e_CHeB || i_Stage == EvolutionStage::e_HeHG || i_Stage == EvolutionStage::e_HeGB;
}
}

