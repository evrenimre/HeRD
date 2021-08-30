/**
 * @file LuminosityRadiusTemperature.h
 * @author Evren Imre
 * @date 28 Apr 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HD40F097B_D96C_4F59_9F6D_2FF6630B422A
#define HD40F097B_D96C_4F59_9F6D_2FF6630B422A

#include <Generic/Quantity.h>

namespace Herd::Physics
{
/**
 * @brief Relation between the electromagnetic radiation emitted by a blackbody, its temperature and radius
 * @remarks Expressions
 *  - The equation is \f$ L = 4\pi\sigma_{SB}R^2T^4 \f$ where \f$ \sigma_{SB} \f$ is the Stefan-Boltzmann constant
 *  - Expressed in terms of solar units, it becomes \f$ \frac{L}{L_{\odot}} =  \left( \frac{R}{R_{\odot}} \right)^2 \left( \frac{T}{T_{\odot}} \right)^4 \f$
 */
class LuminosityRadiusTemperature
{
public:

  static Herd::Generic::Luminosity ComputeLuminosty( Herd::Generic::Radius i_Radius, Herd::Generic::Temperature i_Temperature ); ///< Computes luminosity from radius and temperature in \f$ L_{\odot}\f$
  static Herd::Generic::Radius ComputeRadius( Herd::Generic::Luminosity i_Luminosity, Herd::Generic::Temperature i_Temperature ); ///< Computes radius from luminosity and temperature in \f$ R_{\odot}\f$
  static Herd::Generic::Temperature ComputeTemperature( Herd::Generic::Luminosity i_Luminosity, Herd::Generic::Radius i_Radius ); ///< Computes temperature from luminosity and radius, in \f$ T_{\odot}\f$
};

Herd::Generic::Temperature ComputeAbsoluteTemperature( Herd::Generic::Luminosity i_Luminosity, Herd::Generic::Radius i_Radius ); ///< Computes temperature from luminosity and radius, in K
}

#endif /* HD40F097B_D96C_4F59_9F6D_2FF6630B422A */
