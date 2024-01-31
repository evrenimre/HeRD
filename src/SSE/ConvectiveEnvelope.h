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

#include "RgComputer.h"

#include <Generic/Quantity.h>

#include <memory>
#include <utility>

namespace Herd::SSE
{
// Forward declarations
struct EvolutionState;
class BaseOfGiantBranch;
class HeliumIgnition;
class TerminalMainSequence;
class ZeroAgeMainSequence;

class RgComputer;

/**
 * @brief Computation of convective envelope properties
 * @remarks Implementation of mrenv in AMUSE.SSE
 * @remarks No mention of the original paper for this one. And it is not discussed in Hurley00 either.
 * @cite AMUSE
 * @cite Hurley02
 */
class ConvectiveEnvelope
{
public:

  ConvectiveEnvelope( Herd::Generic::Metallicity i_Z );  ///< Constructor
  ~ConvectiveEnvelope();  ///< Destructor

  /**
   * @brief Envelope properties
   */
  struct Envelope
  {
    Herd::Generic::Mass m_Mass; ///< Envelope mass
    Herd::Generic::Radius m_Radius; ///< Envelope radius
    double m_K2; ///< Constant for computing the contribution of the envelope to the angular momentum. Hurley02 defines it as the radius of gyration, and referred to it that way in the code. It is actually the radius of gyration normalised by radius
  };

  Envelope Compute( const Herd::SSE::EvolutionState& i_rState );

private:
  /**
   * @brief Components depending on the metallicity
   */
  struct MetallicityDependents
  {
    std::unique_ptr< Herd::SSE::ZeroAgeMainSequence > m_pZAMSComputer;  ///< ZAMS computations
    std::unique_ptr< Herd::SSE::TerminalMainSequence > m_pTMSComputer;  ///< TMS computations
    std::unique_ptr< Herd::SSE::BaseOfGiantBranch > m_pBGBComputer; ///< BGB computations
    std::unique_ptr< Herd::SSE::HeliumIgnition > m_pHeIComputer; ///< HeI computations

    std::unique_ptr< Herd::SSE::RgComputer > m_pRgComputer; ///< \fS R_g \fS computations
  };

  MetallicityDependents m_ZDependents;  ///< Metallicity dependentss

  /**
   * @brief Quantities depending on initial mass
   */
  struct InitialMassDependents
  {
    Herd::Generic::Mass m_EvaluatedAt;

    double m_A = 0; ///< A
    double m_C = 0; ///< C
    double m_LogM0 = 0;  ///< \f$ log_{10}M_0 \f$
    Herd::Generic::Mass m_MCEZAMS; ///< Mass of the convective envelope at ZAMS, in terms of the total envelope mass
    Herd::Generic::Radius m_RCEZAMS;  ///< Radius of the convective envelope at ZAMS, in terms of the total envelope radius
    double m_Y = 0;  ///< Used for computing envelope properties in non-Hayashi stars

    double m_K2ZAMS = 0;  ///< Normalised radius of gyration at ZAMS
    double m_K2BGB = 0; ///< Normalised radius of gyration at BGB
  };

  InitialMassDependents m_M0Dependents;

  Herd::Generic::Radius m_Rg; ///< \f$ R_g \f$

  void ComputeInitialMassDependents( Herd::Generic::Mass i_Mass );  ///< Computes the initial mass dependent quantities
  Herd::Generic::Mass DetermineInitialMass( const Herd::SSE::EvolutionState& i_rState );  ///< Determines the effective initial mass for the computations

  double ComputeProximityToHayashi( const Herd::SSE::EvolutionState& i_rState ); ///< Computes a measure of proximity to the Hayashi track in terms of temperature

  std::pair< double, double > ComputeMassAndRadius( const Herd::SSE::EvolutionState& i_rState, double i_TauEnv ); ///< Computes the mass and the radius of the convective envelope in terms of the entire envelope
  double ComputeK2( const Herd::SSE::EvolutionState& i_rState, double i_TauEnv );  ///< Computes the K2 coefficient, the normalised radius of gyration
};
}

#endif /* HAFD0B7BF_5E2F_47CD_A165_00F64B49B33F */
