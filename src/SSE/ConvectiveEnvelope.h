/**
 * @file ConvectiveEnvelope.h
 * @author Evren Imre
 * @date 29 Aug 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HAFD0B7BF_5E2F_47CD_A165_00F64B49B33F
#define HAFD0B7BF_5E2F_47CD_A165_00F64B49B33F

#include <utility>

#include <Generic/Quantity.h>

namespace Herd::SSE
{
// Forward declarations
struct EvolutionState;

/**
 * @brief Computation of convective envelope properties
 * @remarks Implementation of mrenv in AMUSE.SSE
 * @cite AMUSE
 * @warning Radius of gyration is used only in binary star evolution. Until it is implemented, it will not be possible to write useful unit tests
 */
class ConvectiveEnvelope
{
public:
  /**
   * @brief Envelope properties
   */
  struct Envelope
  {
    Herd::Generic::Mass m_Mass; ///< Envelope mass
    Herd::Generic::Radius m_Radius; ///< Envelope radius
    Herd::Generic::Radius m_RadiusOfGyration;  ///< Radius of gyration for the envelope, in \f$ R_{\odot}\f$
  };

  Envelope Compute( const Herd::SSE::EvolutionState& i_rState );

private:

  /**
   * @brief Quantities depending on initial mass
   */
  struct InitialMassDependents
  {
    Herd::Generic::Mass m_EvaluatedAt;

    double m_A = 0; ///< A
    double m_C = 0; ///< C
    double m_LogM0 = 0;  ///< \f$ log_{10}M_0 \f$
    double m_MCEZAMS = 0; ///< Mass of the convective envelope at ZAMS, in terms of the total envelope mass
    double m_RCEZAMS = 0;  ///< Radius of the convective envelope at ZAMS, in terms of the total envelope radius
    double m_Y = 0;  ///< Used for computing envelope properties in non-Hayashi stars

    double m_RGZAMS = 0;  ///< Radius of gyration at ZAMS
    double m_RGBGB = 0;  ///< Radius of gyration at base of the giant branch
  };

  InitialMassDependents m_M0Dependents;

  void ComputeInitialMassDependents( Herd::Generic::Mass i_Mass );  ///< Computes the initial mass dependent quantities
  Herd::Generic::Mass DetermineInitialMass( const Herd::SSE::EvolutionState& i_rState );  ///< Determines the effective initial mass for the computations

  double ComputeProximityToHayashi( const Herd::SSE::EvolutionState& i_rState ); ///< Computes a measure of proximity to the Hayashi track in terms of temperature

  Herd::Generic::Radius ComputeRadiusOfGyration( const Herd::SSE::EvolutionState& i_rState, double i_TauEnv ); ///< Computes the radius of gyration
  std::pair< double, double > ComputeMassAndRadius( const Herd::SSE::EvolutionState& i_rState, double i_TauEnv ); ///< Computes the mass and the radius of the convective envelope in terms of the entire envelope
};
}

#endif /* HAFD0B7BF_5E2F_47CD_A165_00F64B49B33F */
