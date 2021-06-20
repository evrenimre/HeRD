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

/**
 * @brief Physical constants
 * @cite IAU16
 */
namespace Herd::Physics::Constants
{
inline constexpr double s_StefanBoltzmannConstant = 5.670374419184429e-8;  ///< Stefan-Boltzmann constant

inline constexpr double s_SunSurfaceTemperature = 5772.0; ///< Sun surface temperature in K (IAU recommended value)
inline constexpr double s_SunRadius = 6.957e8; ///< Radius of the Sun in m
inline constexpr double s_SunLuminosity = 3.828e26; ///< Solar luminosity in W
}


#endif /* H478BF0E6_1C4E_48C0_9531_57146385BE2B */
