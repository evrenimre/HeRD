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

namespace Herd::SSE
{
/**
 * @brief Star evolution stages
 * @cite Hurley00
 */
enum class EvolutionStage
{
  e_MSLO,  // Low mass main sequence
  e_MS,  // Main sequence
  e_HG,  // Hertzsprung gap
  e_FGB, // First giant branch
  e_CHB, // Core helium burning
  e_FAGB,  // First ascent giant
  e_SAGB,  // Second ascent giant
  e_HGNH,  // Hertzsprung gap naked helium
  e_HWD, // Helium white dwarf
  e_COWD,  // Carbon/oxygen white dwarf
  e_NS,  // Neutron star
  e_BH,  // Black hole
  e_MSn,  // Massless supernova
  e_Undefined/**< e_Undefined */
};

}



#endif /* H808BBCA9_C3C4_4237_A17C_8721989DF8BE */
