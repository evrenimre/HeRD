/**
 * @file StellarWindMassLoss.h
 * @author Evren Imre
 * @date 19 Jun 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef H5884ABA0_83BD_4120_AFD9_CDC95056180F
#define H5884ABA0_83BD_4120_AFD9_CDC95056180F

#include "Star.h"

namespace Herd::SSE
{

/**
 * @brief Computes the mass loss rate due to stellar wind
 * @cite Hurley00
 * @cite AMUSE
 * @remarks Reimplementation of mlwind in AMUSE.SSE
 */
class StellarWindMassLoss
{
public:

  static double Compute( const Herd::SSE::Star& i_rStar, double i_Neta, double i_HeWind, double i_BinaryWind, double i_RocheLobe );
  static double Compute( const Herd::SSE::Star& i_rStar, double i_Neta );

private:

  static void Validate( const Herd::SSE::Star& i_rStar, double i_Neta, double i_HeWind, double i_BinaryWind, double i_RocheLobe ); ///< Validates the inputs

  static double ComputeReimersLoss( const Herd::SSE::Star& i_rStar, double i_Neta, double i_BinaryWind, double i_RocheLobe ); ///< Computes the Reimers loss
  static double ComputePulsationLoss( const Herd::SSE::Star& i_rStar );  ///< Computes the loss in AGB due to high pulsation periods
  static double ComputeMassiveStarLoss( const Herd::SSE::Star& i_rStar ); ///< Computes the loss for massive stars
  static double ComputeWRLikeLoss( const Herd::SSE::Star& i_rStar, double i_Mu ); ///< Computes the Wolf-Rayet-like loss for stars with a small He envelope
  static double ComputeLBVLikeLoss( const Herd::SSE::Star& i_rStar ); ///< Computes the luminous blue variable-like loss for stars beyond the Humphreys-Davidson limit
};
}

#endif /* H5884ABA0_83BD_4120_AFD9_CDC95056180F */
