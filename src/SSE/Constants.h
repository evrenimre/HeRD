/**
 * @file Constants.h
 * @author Evren Imre
 * @date 20 Jun 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef H4BB1EA9B_933C_43CA_A1BA_11FDF79810E0
#define H4BB1EA9B_933C_43CA_A1BA_11FDF79810E0

namespace Herd::SSE::Constants
{
inline constexpr double s_SunSurfaceTemperatureSSE = 5797.885;  ///< Surface temperature of the sun in K in AMUSE.SSE (evolve.f, L266 1000*(1130)^0.25)
inline constexpr double s_SolarMetallicityTout96 = 0.02;  ///< Z value for the Sun in Tout96
}



#endif /* H4BB1EA9B_933C_43CA_A1BA_11FDF79810E0 */
