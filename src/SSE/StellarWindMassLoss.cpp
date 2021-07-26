/**
 * @file StellarWindMassLoss.cpp
 * @author Evren Imre
 * @date 19 Jun 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "Constants.h"
#include "EvolutionStage.h"
#include "StellarWindMassLoss.h"
#include "TrackPoint.h"

#include <Exceptions/ExceptionWrappers.h>

#include <algorithm>
#include <cmath>

#include <boost/math/special_functions/pow.hpp>

namespace Herd::SSE
{

/**
 * @param i_rTrackPoint Track point. Luminosity, radius and core mass >=0, mass > 0, stage not undefined
 * @param i_Neta Reimers mass loss efficiency.  >=0
 * @param i_HeWind Helium star mass loss factor. >=0
 * @param i_BinaryWind Mass loss factor for binary stars. >=0
 * @param i_RocheLobe Roche lobe factor for binary stars. >=0
 * @return Mass loss rate per year in \f$ M_{\odot}\f$. >=0
 * @remarks Binary star case
 */
double StellarWindMassLoss::Compute( const Herd::SSE::TrackPoint& i_rTrackPoint, double i_Neta, double i_HeWind, double i_BinaryWind, double i_RocheLobe )
{
  Validate( i_rTrackPoint, i_Neta, i_HeWind, i_BinaryWind, i_RocheLobe );

  // MS and remnant loss
  if( Herd::SSE::IsRemnant( i_rTrackPoint.m_Stage ) || Herd::SSE::IsMS( i_rTrackPoint.m_Stage ) )
  {
    return ComputeMassiveStarLoss( i_rTrackPoint );
  }

  // From HG to remnant
  double dMR = ComputeReimersLoss( i_rTrackPoint, i_Neta, i_BinaryWind, i_RocheLobe );
  double dMWR = ComputeWRLikeLoss( i_rTrackPoint );

  // Naked He star loss
  if( Herd::SSE::IsHeStar( i_rTrackPoint.m_Stage ) )
  {
    return std::max( dMR, dMWR * i_HeWind );
  }

  // From HG to He star
  double dMNJ = ComputeMassiveStarLoss( i_rTrackPoint );
  double dMLBV = ComputeLBVLikeLoss( i_rTrackPoint );
  double dMVW = Herd::SSE::IsAGB( i_rTrackPoint.m_Stage ) ? ComputePulsationLoss( i_rTrackPoint ) : 0.;

  return std::max( { dMR, dMVW, dMNJ, dMWR } ) + dMLBV;
}

/**
 * @param i_rTrackPoint Track point
 * @param i_Neta Reimers mass loss efficiency
 * @param i_HeWind Helium star mass loss factor
 * @param i_BinaryWind Mass loss factor for binary stars
 * @param i_RocheLobe Roche lobe factor for binary stars
 * @throws PreconditionError if preconditions violated
 */
void StellarWindMassLoss::Validate( const Herd::SSE::TrackPoint& i_rTrackPoint, double i_Neta, double i_HeWind, double i_BinaryWind, double i_RocheLobe )
{
  Herd::SSE::ValidateTrackPoint( i_rTrackPoint );

  Herd::Exceptions::ThrowPreconditionErrorIfNegative( i_Neta, "i_Neta" ); // @suppress("Invalid arguments")
  Herd::Exceptions::ThrowPreconditionErrorIfNegative( i_BinaryWind, "i_BinaryWind" ); // @suppress("Invalid arguments")
  Herd::Exceptions::ThrowPreconditionErrorIfNegative( i_HeWind, "i_HeWind" ); // @suppress("Invalid arguments")
  Herd::Exceptions::ThrowPreconditionErrorIfNegative( i_RocheLobe, "i_RocheLobe" ); // @suppress("Invalid arguments")
}

/**
 * @param i_rTrackPoint Track point
 * @param i_Neta Reimers mass loss efficiency
 * @param i_BinaryWind Mass loss factor for binary stars
 * @param i_RocheLobe Roche lobe factor for binary stars
 * @return Loss rate
 */
double StellarWindMassLoss::ComputeReimersLoss( const Herd::SSE::TrackPoint& i_rTrackPoint, double i_Neta, double i_BinaryWind, double i_RocheLobe )
{
  double lossRate = 4e-13 * i_Neta * i_rTrackPoint.m_Radius * i_rTrackPoint.m_Luminosity / i_rTrackPoint.m_Mass; // Eq. 106

  if( i_RocheLobe > 0 )
  {
    lossRate *= ( 1. + i_BinaryWind * ( boost::math::pow< 6 >( std::min( 0.5, i_rTrackPoint.m_Radius / i_RocheLobe ) ) ) );
  }

  return lossRate;
}

/**
 * @param i_rTrackPoint Track point
 * @return Loss rate
 */
double StellarWindMassLoss::ComputePulsationLoss( const Herd::SSE::TrackPoint& i_rTrackPoint )
{
  // Mira pulsation period
  double logP0 = -2.07 - 0.9 * std::log10( i_rTrackPoint.m_Mass ) + 1.94 * std::log10( i_rTrackPoint.m_Radius );
  double p0 = std::min( 2000., std::pow( 10., logP0 ) );

  double logLossRate = -11.4 + 0.0125 * ( p0 - 100. * std::max( i_rTrackPoint.m_Mass - 2.5, 0. ) );
  return std::min( 1.36e-9 * i_rTrackPoint.m_Luminosity, std::pow( 10., logLossRate ) );
}

/**
 * @param i_rTrackPoint Track point
 * @return Loss rate
 */
double StellarWindMassLoss::ComputeMassiveStarLoss( const Herd::SSE::TrackPoint& i_rTrackPoint )
{
  if( i_rTrackPoint.m_Luminosity <= 4000. )
  {
    return 0.;
  }

  double x = std::min( 1., ( i_rTrackPoint.m_Luminosity - 4000. ) / 500. );

  double r081 = std::pow( i_rTrackPoint.m_Radius, 0.81 );
  double l124 = std::pow( i_rTrackPoint.m_Luminosity, 1.24 );
  double m016 = std::pow( i_rTrackPoint.m_Mass, 0.16 );
  double zos05 = std::sqrt( i_rTrackPoint.m_InitialMetallicity / Herd::SSE::Constants::s_SolarMetallicityTout96 );

  return 9.6e-15 * x * r081 * l124 * m016 * zos05;
}

/**
 * @param i_rTrackPoint Track point
 * @return Loss rate
 */
double StellarWindMassLoss::ComputeWRLikeLoss( const Herd::SSE::TrackPoint& i_rTrackPoint )
{
  double mu = 0.;

  if( !Herd::SSE::IsHeStar( i_rTrackPoint.m_Stage ) )
  {
    double envelopeRatio = ( i_rTrackPoint.m_Mass.Value() - i_rTrackPoint.m_CoreMass.Value() ) / i_rTrackPoint.m_Mass.Value();
    mu = envelopeRatio * std::min( 5., std::max( 1.2, std::sqrt( 7.0e4 / i_rTrackPoint.m_Luminosity ) ) );  // Eq. 97
  }

  return mu > 1. ? 0. : 1e-13 * std::pow( i_rTrackPoint.m_Luminosity, 1.5 ) * ( 1. - mu );
}

/**
 * @param i_rTrackPoint Track point
 * @return Loss rate
 */
double StellarWindMassLoss::ComputeLBVLikeLoss( const Herd::SSE::TrackPoint& i_rTrackPoint )
{
  if( i_rTrackPoint.m_Luminosity <= 6.0e5 )
  {
    return 0.;
  }

  double x = 1e-5 * i_rTrackPoint.m_Radius * std::sqrt( i_rTrackPoint.m_Luminosity.Value() );
  if( x <= 1. )
  {
    return 0.;
  }

  return 0.1 * boost::math::pow< 3 >( x - 1. ) * ( i_rTrackPoint.m_Luminosity / 6.0e5 - 1. );
}

}

