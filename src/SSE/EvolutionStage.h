/**
 * @file EvolutionStages.h
 * @author Evren Imre
 * @date 14 Jun 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef H808BBCA9_C3C4_4237_A17C_8721989DF8BE
#define H808BBCA9_C3C4_4237_A17C_8721989DF8BE

#include <array>
#include <string>

namespace Herd::SSE
{
/**
 * @brief Star evolution stages
 * @cite Hurley00
 */
enum class EvolutionStage
{
  e_MSLM,  // Deeply or fully convective low mass MS star
  e_MS,  // Main sequence star
  e_HG,  // Hertzsprung gap
  e_FGB, // First giant branch
  e_CHeB, // Core helium burning
  e_FAGB,  // First asymptotic giant branch
  e_SAGB,  // Second asymptotic giant branch
  e_HeMS, // Main sequence naked helium star
  e_HeHG,  // Hertzsprung gap naked helium star
  e_HeGB, // Giant branch naked helium star
  e_HeWD, // Helium white dwarf
  e_COWD,  // Carbon/oxygen white dwarf
  e_ONWD, // Oxygen/neon white dwarf
  e_NS,  // Neutron star
  e_BH,  // Black hole
  e_MSn,  // Massless supernova
  e_Undefined // Undefined
};

// Helpers
bool IsRemnant( EvolutionStage i_Stage ); ///< Is a remnant stage?
bool IsMS( EvolutionStage i_Stage );  ///< Is a MS stage?
bool IsAGB( EvolutionStage i_Stage );  ///< Is an AGB stage?
bool IsHeStar( EvolutionStage i_Stage );  ///< Is a He Star?

// Enum <-> String conversions
EvolutionStage StringToEvolutionStage( const std::string& i_rString ); ///< Converts a string to an evolution stage
std::string EvolutionStageToString( EvolutionStage i_Stage );  ///< Converts an evolution stage to a string

std::array< EvolutionStage, 17 > EnumerateEvolutionStages();  ///< Returns a list of evolution stages


}

#endif /* H808BBCA9_C3C4_4237_A17C_8721989DF8BE */
