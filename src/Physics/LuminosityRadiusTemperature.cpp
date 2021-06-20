/**
 * @file LuminosityRadiusTemperature.cpp
 * @author Evren Imre
 * @date 28 Apr 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "LuminosityRadiusTemperature.h"

#include <Exceptions/PreconditionError.h>

#include <cmath>

#include <boost/math/special_functions/pow.hpp>

namespace Herd::Physics
{

/**
 * @param i_Radius Radius in \f$ R_{\odot} \f$
 * @param i_Temperature Temperature in \f$ T_{\odot} \f$
 * @return Luminosity in \f$ L_{\odot} \f$
 * @pre \c i_Radius>=0
 * @pre \c i_Temperature>=0
 * @throws PreconditionError if preconditions are violated
 */
Herd::Generic::Luminosity LuminosityRadiusTemperature::ComputeLuminosty( Herd::Generic::Radius i_Radius,
    Herd::Generic::Temperature i_Temperature )
{
  Herd::Generic::ThrowIfNegative( i_Radius, "i_Radius" );
  Herd::Generic::ThrowIfNegative( i_Temperature, "i_Temperature" );

  double r2 = boost::math::pow< 2, double >( i_Radius );
  double t4 = boost::math::pow< 4, double >( i_Temperature );

  return Herd::Generic::Luminosity( r2 * t4 );
}

/**
 * @param i_Luminosity Luminosity in \f$ L_{\odot}\f$
 * @param i_Temperature Temperature in \f$ T_{\odot} \f$
 * @return Radius in \f$ R_{\odot} \f$
 * @pre \c i_Luminosity>=0
 * @pre \c i_Temperature>0
 * @throws PreconditionError if preconditions are violated
 */
Herd::Generic::Radius LuminosityRadiusTemperature::ComputeRadius( Herd::Generic::Luminosity i_Luminosity,
    Herd::Generic::Temperature i_Temperature )
{
  Herd::Generic::ThrowIfNegative( i_Luminosity, "i_Luminosity" );
  Herd::Generic::ThrowIfNotPositive( i_Temperature, "i_Temperature" );

  double t2 = boost::math::pow< 2, double >( i_Temperature );
  return Herd::Generic::Radius( std::sqrt( i_Luminosity ) / t2 );
}

/**
 * @param i_Luminosity Luminosity in \f$ L_{\odot} \f$
 * @param i_Radius Radius in \f$ R_{\odot} \f$
 * @return Temperature in \f$ T_{\odot} \f$
 * @pre \c i_Luminosity>=0
 * @pre \c i_Radius>0
 * @throws PreconditionError if preconditions are violated
 */
Herd::Generic::Temperature LuminosityRadiusTemperature::ComputeTemperature( Herd::Generic::Luminosity i_Luminosity,
    Herd::Generic::Radius i_Radius )
{
  Herd::Generic::ThrowIfNegative( i_Luminosity, "i_Luminosity" );
  Herd::Generic::ThrowIfNotPositive( i_Radius, "i_Radius" );

  double r2 = boost::math::pow< 2, double >( i_Radius );
  return Herd::Generic::Temperature( std::pow( i_Luminosity / r2, 0.25 ) );
}

}



