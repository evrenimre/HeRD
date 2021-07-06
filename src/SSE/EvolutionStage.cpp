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

#include "EvolutionStage.h"

#include <Exceptions/PreconditionError.h>

#include <boost/container/flat_map.hpp>
#include <range/v3/algorithm.hpp>
#include <range/v3/view.hpp>

namespace
{
//@formatter:off
static inline boost::container::flat_map< Herd::SSE::EvolutionStage, std::string > s_StageToString{ { Herd::SSE::EvolutionStage::e_MSLM, "MSLM"},
  { Herd::SSE::EvolutionStage::e_MS, "MS"},
  { Herd::SSE::EvolutionStage::e_HG, "HG"},
  { Herd::SSE::EvolutionStage::e_FGB, "FGB"},
  { Herd::SSE::EvolutionStage::e_CHeB, "CHeB"},
  { Herd::SSE::EvolutionStage::e_FAGB, "FAGB"},
  { Herd::SSE::EvolutionStage::e_SAGB, "SAGB"},
  { Herd::SSE::EvolutionStage::e_HeMS, "HeMS"},
  { Herd::SSE::EvolutionStage::e_HeHG, "HeHG"},
  { Herd::SSE::EvolutionStage::e_HeGB, "HeGB"},
  { Herd::SSE::EvolutionStage::e_HeWD, "HeWD"},
  { Herd::SSE::EvolutionStage::e_COWD, "COWD"},
  { Herd::SSE::EvolutionStage::e_ONWD, "ONWD"},
  { Herd::SSE::EvolutionStage::e_NS, "NS"},
  { Herd::SSE::EvolutionStage::e_BH, "BH"},
  { Herd::SSE::EvolutionStage::e_MSn, "MSn"},
  { Herd::SSE::EvolutionStage::e_Undefined, "Undefined"}};

static inline boost::container::flat_map< std::string, Herd::SSE::EvolutionStage > s_StringToStage{ { "MSLM", Herd::SSE::EvolutionStage::e_MSLM },
  { "MS",Herd::SSE::EvolutionStage::e_MS},
  { "HG", Herd::SSE::EvolutionStage::e_HG},
  { "FGB", Herd::SSE::EvolutionStage::e_FGB },
  { "CHeB", Herd::SSE::EvolutionStage::e_CHeB },
  { "FAGB", Herd::SSE::EvolutionStage::e_FAGB },
  { "SAGB", Herd::SSE::EvolutionStage::e_SAGB },
  { "HeMS", Herd::SSE::EvolutionStage::e_HeMS },
  { "HeHG", Herd::SSE::EvolutionStage::e_HeHG },
  { "HeGB", Herd::SSE::EvolutionStage::e_HeGB },
  { "HeWD", Herd::SSE::EvolutionStage::e_HeWD },
  { "COWD", Herd::SSE::EvolutionStage::e_COWD },
  { "ONWD", Herd::SSE::EvolutionStage::e_ONWD },
  { "NS", Herd::SSE::EvolutionStage::e_NS },
  { "BH", Herd::SSE::EvolutionStage::e_BH },
  { "MSn", Herd::SSE::EvolutionStage::e_MSn },
  { "Undefined", Herd::SSE::EvolutionStage::e_Undefined }};
//@formatter:on
}

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
 * @param i_rString String
 * @return Corresponding enum
 * @pre \c i_rString has a corresponding enum
 * @throws PreconditionError If \c i_rString does not have a corresponding enum
 */
EvolutionStage ConvertStringToEvolutionStage( const std::string& i_rString )
{
  auto iQuery = s_StringToStage.find( i_rString );
  //@formatter:off
  if( iQuery == s_StringToStage.end() ) [[unlikely]]
                                           //@formatter:on
  {
    throw( Herd::Exceptions::PreconditionError( "s_StringToStage", "a valid evolution stage", i_rString ));
  }

  return iQuery->second;
}

/**
 * @param i_Stage Evolution stage
 * @return String corresponding to the evolution stage
 */
std::string ConvertEvolutionStageToString( EvolutionStage i_Stage )
{
  return s_StageToString[ i_Stage ];
}

/**
 * @return A list of evolution stages in the same order as EvolutionStage
 */
std::array< EvolutionStage, 17 > EnumerateEvolutionStages()
{
  std::array< EvolutionStage, 17 > output;
  ranges::cpp20::copy( s_StageToString | ranges::views::keys, output.begin() );

  return output;
}

}

