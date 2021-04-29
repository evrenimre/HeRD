/**
 * @file LuminosityRadiusTemperature.h
 * @author Evren Imre
 * @date 28 Apr 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HD40F097B_D96C_4F59_9F6D_2FF6630B422A
#define HD40F097B_D96C_4F59_9F6D_2FF6630B422A

#include <Generic/Quantities.h>

#include <optional>

namespace Herd::Physics
{
/**
 * @brief Luminosity, radius and surface temperature equation
 * @remarks Expressions
 *  - The equation is \f$ L = 4\pi\sigma_{SB}R^2T^4 \f$ where \f$ \sigma_{SB} \f$ is the Stefan-Boltzmann constant
 *  - Expressed in terms of solar properties, it becomes \f$ \frac{L}{L_{\odot}} =  \left( \frac{R}{R_{\odot}} \right)^2 \left( \frac{T}{T_{\odot}} \right)^4 \f$
 */
class LuminosityRadiusTemperature
{
public:

  static std::optional< Herd::Generic::Luminosity > ComputeLuminosty( Herd::Generic::Radius i_Radius, Herd::Generic::Temperature i_Temperature );
  static std::optional< Herd::Generic::Radius > ComputeRadius( Herd::Generic::Luminosity i_Luminosity, Herd::Generic::Temperature i_Temperature );
  static std::optional< Herd::Generic::Temperature > ComputeTemperature( Herd::Generic::Luminosity i_Luminosity, Herd::Generic::Radius i_Radius );
};
}




#endif /* HD40F097B_D96C_4F59_9F6D_2FF6630B422A */
