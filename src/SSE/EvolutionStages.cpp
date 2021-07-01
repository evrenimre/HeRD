/**
 * @file EvolutionStages.cpp
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
  return i_Stage == EvolutionStage::e_HeMS || i_Stage == EvolutionStage::e_HeHG || i_Stage == EvolutionStage::e_HeGB;
}

/**
 * @return A bimap for evolution stages and their corresponding strings
 */
boost::bimap< EvolutionStage, std::string > MakeEvolutionStageAndStringBimap()
{
  using Bimap = boost::bimap< EvolutionStage, std::string >;
  using Record = Bimap::value_type;

  Bimap dictionary;

  dictionary.insert( Record( EvolutionStage::e_MSLM, "MSLM" ) );
  dictionary.insert( Record( EvolutionStage::e_MS, "MS" ) );
  dictionary.insert( Record( EvolutionStage::e_HG, "HG" ) );
  dictionary.insert( Record( EvolutionStage::e_FGB, "FGB" ) );
  dictionary.insert( Record( EvolutionStage::e_CHeB, "CHeB" ) );
  dictionary.insert( Record( EvolutionStage::e_FAGB, "FAGB" ) );
  dictionary.insert( Record( EvolutionStage::e_SAGB, "SAGB" ) );
  dictionary.insert( Record( EvolutionStage::e_HeMS, "HeMS" ) );
  dictionary.insert( Record( EvolutionStage::e_HeHG, "HeHG" ) );
  dictionary.insert( Record( EvolutionStage::e_HeGB, "HeGB" ) );
  dictionary.insert( Record( EvolutionStage::e_HeWD, "HeWD" ) );
  dictionary.insert( Record( EvolutionStage::e_COWD, "COWD" ) );
  dictionary.insert( Record( EvolutionStage::e_ONWD, "ONWD" ) );
  dictionary.insert( Record( EvolutionStage::e_NS, "NS" ) );
  dictionary.insert( Record( EvolutionStage::e_BH, "BH" ) );
  dictionary.insert( Record( EvolutionStage::e_MSn, "MSn" ) );
  dictionary.insert( Record( EvolutionStage::e_Undefined, "Undefined" ) );

  return dictionary;
}

}

