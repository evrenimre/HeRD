/**
 * @file SSETestUtils.h
 * @author Evren Imre
 * @date 25 Jul 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef H5654923D_ECC2_4E88_8819_51F79E788A57
#define H5654923D_ECC2_4E88_8819_51F79E788A57

#include <random>

#include <SSE/EvolutionState.h>
#include <SSE/TrackPoint.h>

namespace Herd::SSE::UnitTests
{
Herd::SSE::TrackPoint GenerateRandomTrackPoint( std::mt19937& io_rRng ); ///< Generates a random track point
Herd::SSE::EvolutionState GenerateRandomEvolutionState( std::mt19937& io_rRng );  ///< Generates a random evolution state
}



#endif /* H5654923D_ECC2_4E88_8819_51F79E788A57 */
