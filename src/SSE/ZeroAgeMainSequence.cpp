/**
 * @file ZeroAgeMainSequence.cpp
 * @author Evren Imre
 * @date 4 Nov 2020	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "ZeroAgeMainSequence.h"

#include <Exceptions/PreconditionError.h>
#include <Generic/Quantities.h>
#include <Generic/QuantityRange.h>
#include <Physics/Constants.h>
#include <Physics/LuminosityRadiusTemperature.h>

#include <algorithm>
#include <cstddef>
#include <cmath>

#include <range/v3/algorithm.hpp>
#include <range/v3/view.hpp>

#include <boost/math/special_functions/pow.hpp>
#include <boost/math/special_functions/relative_difference.hpp>

namespace Herd::SSE
{

namespace Detail::Data
{
// @formatter:off
double eq3Coefficients[] { 3.970417e-01, -3.2913574e-01, 3.4776688e-01, 3.7470851e-01, 9.011915e-02,
    8.527626e+00, -2.441225973e+01, 5.643597107e+01, 3.706152575e+01, 5.4562406e+00,
    2.5546e-04, -1.23461e-03, -2.3246e-04, 4.5519e-04, 1.6176e-04,
    5.432889e+00, -8.62157806e+00, 1.344202049e+01, 1.451584135e+01, 3.39793084e+00,
    5.563579e+00, -1.032345224e+01, 1.944322980e+01, 1.897361347e+01, 4.16903097e+00,
    7.8866060e-01, -2.90870942e+00, 6.54713531e+00, 4.05606657e+00, 5.3287322e-01,
    5.86685e-03, -1.704237e-02, 3.872348e-02, 2.570041e-02, 3.83376e-03
};

double eq4Coefficients[] { 1.715359e+00, 6.2246212e-01, -9.2557761e-01, -1.16996966e+00, -3.0631491e-01,
    6.597788e+00, -4.2450044e-01, -1.213339427e+01, -1.073509484e+01, -2.51487077e+00,
    1.008855000e+01, -7.11727086e+00, -3.167119479e+01, -2.424848322e+01, -5.33608972e+00,
    1.012495e+00, 3.2699690e-01, -9.23418e-03, -3.876858e-02, -4.12750e-03,
    7.490166e-02, 2.410413e-02, 7.233664e-02, 3.040467e-02, 1.97741e-03,
    1.077422e-02, 0, 0, 0, 0,
    3.082234e+00, 9.447205e-01, -2.15200882e+00, -2.49219496e+00, -6.3848738e-01,
    1.784778e+01, -7.4534569e+00, -4.896066856e+01, -4.005386135e+01, -9.09331816e+00,
    2.2582e-04, -1.86899e-03, 3.88783e-03, 1.42402e-03, -7.671e-05
};
// @formatter:on
}

struct ZeroAgeMainSequenceAlgorithmSpecs;

/**
 * @param i_Mass Mass in \f$ M_{\odot} \f$
 * @param i_Z Metallicity
 * @return ZAMS state
 * @pre \c i_Mass is within \c s_MassRange
 * @pre \c i_Z is is within \c s_ZRange
 */
Herd::SSE::EvolutionState ZeroAgeMainSequence::Compute( Herd::Generic::Mass i_Mass, Herd::Generic::Metallicity i_Z )
{
  Validate( i_Mass, i_Z );  // Throws is the input is invalid
  
  Eigen::Matrix< double, 5, 1 > zVector;
  zVector[ 0 ] = 1;
  zVector[ 1 ] = log10( i_Z / Herd::Physics::Constants::s_SolarMetallicityTout96 );
  ranges::cpp20::for_each( ranges::cpp20::views::iota( 2, 5 ), [ & ]( auto i_Index ) // @suppress("Function cannot be resolved")
  { zVector[ i_Index ] = zVector[i_Index-1]*zVector[1];} );

  Herd::Generic::Luminosity luminosity = ComputeLuminosity( i_Mass, zVector );
  Herd::Generic::Radius radius = ComputeRadius( i_Mass, zVector );

  Herd::SSE::EvolutionState output;
  output.m_Age.Set( 0 );
  output.m_Luminosity = luminosity;
  output.m_Radius = radius;
  output.m_Mass = i_Mass;
  output.m_Z = i_Z;
  output.m_Temperature = Herd::Physics::LuminosityRadiusTemperature::ComputeTemperature( luminosity, radius );

  return output;
}

/**
 * @param i_Mass Mass in \f$ M_{\odot} \f$
 * @param i_Z Metallicity
 * @throws PreconditionError if preconditions are violated
 */
void ZeroAgeMainSequence::Validate( Herd::Generic::Mass i_Mass, Herd::Generic::Metallicity i_Z )
{
  // Mass is within the allowed range
  if( !ZeroAgeMainSequenceSpecs::s_MassRange.Contains( i_Mass ) )
  {
    throw( Herd::Exceptions::PreconditionError( "Mass", ZeroAgeMainSequenceSpecs::s_MassRange.GetRangeString(), i_Mass.Value() ) );
  }

  // Metallicity is within the allowed range
  if( !ZeroAgeMainSequenceSpecs::s_ZRange.Contains( i_Z ) )
  {
    throw( Herd::Exceptions::PreconditionError( "Metallicity", ZeroAgeMainSequenceSpecs::s_ZRange.GetRangeString(), i_Z.Value() ) );
  }
}

/**
 * @param i_Mass Mass in \f$ M_{\odot} \f$
 * @param i_ZVector Powers of log metallicity
 * @return Luminosity in \f$ L_{\odot} \f$
 */
Herd::Generic::Luminosity ZeroAgeMainSequence::ComputeLuminosity( Herd::Generic::Mass i_Mass, const Eigen::Matrix< double, 5, 1 >& i_ZVector )
{
  Eigen::Map< Eigen::Matrix< double, 7, 5, Eigen::RowMajor > > coefficientMatrix( &Detail::Data::eq3Coefficients[ 0 ] );
  Eigen::Matrix< double, 7, 1 > eq1Coeffs = coefficientMatrix * i_ZVector;  // Eq3

  // Powers of mass
  double m05 = std::sqrt( i_Mass );
  double m20 = boost::math::pow< 2, double >( i_Mass.Value() );
  double m30 = m20 * i_Mass;
  double m50 = m30 * m20;
  double m55 = m50 * m05;
  double m70 = m50 * m20;
  double m80 = m50 * m30;
  double m95 = m55 * m20 * m20;
  double m110 = m55 * m55;

  // Eq1
  double num = eq1Coeffs[ 0 ] * m55 + eq1Coeffs[ 1 ] * m110;
  double den = eq1Coeffs[ 2 ] + m30 + eq1Coeffs[ 3 ] * m50 + eq1Coeffs[ 4 ] * m70 + eq1Coeffs[ 5 ] * m80 + eq1Coeffs[ 6 ] * m95;

  return Herd::Generic::Luminosity( num / den );
}

/**
 * @param i_Mass Mass in \f$ M_{\odot} \f$
 * @param i_ZVector Powers of log metallicity
 * @return Radius in \f$ R_{\odot} \f$
 */
Herd::Generic::Radius ZeroAgeMainSequence::ComputeRadius( Herd::Generic::Mass i_Mass, const Eigen::Matrix< double, 5, 1 >& i_ZVector )
{
  Eigen::Map< Eigen::Matrix< double, 9, 5, Eigen::RowMajor > > coefficientMatrix( &Detail::Data::eq4Coefficients[ 0 ] );
  Eigen::Matrix< double, 9, 1 > eq2Coeffs = coefficientMatrix * i_ZVector;  // Eq4

  // Powers of mass
  double m05 = std::sqrt( i_Mass );
  double m20 = boost::math::pow< 2, double >( i_Mass );
  double m25 = m20 * m05;
  double m65 = boost::math::pow< 3 >( m20 ) * m05;
  double m85 = m65 * m20;
  double m110 = m85 * m25;
  double m185 = m110 * m65 * i_Mass;
  double m190 = m185 * m05;
  double m195 = m110 * m85;

  // Eq2
  double num = eq2Coeffs[ 0 ] * m25 + eq2Coeffs[ 1 ] * m65 + eq2Coeffs[ 2 ] * m110 + eq2Coeffs[ 3 ] * m190 + eq2Coeffs[ 4 ] * m195;
  double den = eq2Coeffs[ 5 ] + eq2Coeffs[ 6 ] * m20 + eq2Coeffs[ 7 ] * m85 + m185 + eq2Coeffs[ 8 ] * m195;

  return Herd::Generic::Radius( num / den );
}


} // namespace Herd::SSE

