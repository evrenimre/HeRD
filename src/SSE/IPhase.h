/**
 * @file IPhase.h
 * @author Evren Imre
 * @date 30 Jul 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef H99A0DCEA_0F0C_4875_BF2A_E2F9CA5E4BAC
#define H99A0DCEA_0F0C_4875_BF2A_E2F9CA5E4BAC

#include <Generic/Quantity.h>

namespace Herd::SSE
{
struct EvolutionState;

/**
 * @brief Interface class for updating the star state in an evolution phase
 */
class IPhase
{
public:
  virtual bool Evolve( EvolutionState& io_rState ) = 0;
  virtual ~IPhase() = default;
};
}



#endif /* H99A0DCEA_0F0C_4875_BF2A_E2F9CA5E4BAC */
