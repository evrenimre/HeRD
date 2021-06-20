/**
 * @file Constants.h
 * @author Evren Imre
 * @date 28 Apr 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef H478BF0E6_1C4E_48C0_9531_57146385BE2B
#define H478BF0E6_1C4E_48C0_9531_57146385BE2B

namespace Herd::Physics::Constants
{
inline constexpr double s_StefanBoltzmannConstant = 5.670374419184429e-8;  ///< Stefan-Boltzmann constant

inline constexpr double s_SunSurfaceTemperatureSSE = 5797.885;  ///< Surface temperature of the sun in K in AMUSE.SSE (evolve.f, L266 1000*(1130)^0.25)
inline constexpr double s_SolarMetallicityTout96 = 0.02;  ///< Z value for the Sun in Tout96
}


#endif /* H478BF0E6_1C4E_48C0_9531_57146385BE2B */
