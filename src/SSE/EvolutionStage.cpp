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

#include "EvolutionStage.h"

#include <Exceptions/ExceptionWrappers.h>

#include <string>
#include <unordered_map>

#include <range/v3/algorithm.hpp>
#include <range/v3/view.hpp>

namespace
{
//@formatter:off
const std::unordered_map< Herd::SSE::EvolutionStage, std::string > s_StageToString{ { Herd::SSE::EvolutionStage::e_MSLM, "MSLM"},
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
//@formatter:on

/**
 * @brief Makes a map from strings to evolution stages
 * @return A map from strings to evolution stages
 */
std::unordered_map< std::string_view, Herd::SSE::EvolutionStage > MakeStringToStage()
{
  std::unordered_map< std::string_view, Herd::SSE::EvolutionStage > output;

  //@formatter:off
  ranges::cpp20::for_each( s_StageToString, [&]( const auto& i_rEntry ){ output.emplace( i_rEntry.second, i_rEntry.first);});
    //@formatter:on

  return output;
}
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
EvolutionStage StringToEvolutionStage( const std::string_view& i_rString )
{
  // This makes the converter map from s_StageToString. If a new type is added, it is automatically picked up
  static const std::unordered_map< std::string_view, Herd::SSE::EvolutionStage > s_StringToStage = MakeStringToStage();

  auto iQuery = s_StringToStage.find( i_rString );
  if( iQuery == s_StringToStage.end() )
  {
    [[unlikely]] Herd::Exceptions::ThrowPreconditionError( "s_StringToStage", "a valid evolution stage", i_rString );
  }

  return iQuery->second;
}

/**
 * @param i_Stage Evolution stage
 * @return String corresponding to the evolution stage
 */
std::string_view EvolutionStageToString( EvolutionStage i_Stage )
{
  return s_StageToString.find( i_Stage )->second;
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

