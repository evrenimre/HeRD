/**
 * @file SingleStarEvolution.h
 * @author Evren Imre
 * @date 15 Jun 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef H5CFC91CB_E335_485A_9D7D_189B85CF3E28
#define H5CFC91CB_E335_485A_9D7D_189B85CF3E28

#include "EvolutionState.h"

#include <Generic/Quantity.h>
#include <Generic/QuantityRange.h>

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>

#include "EvolutionStage.h"
#include "TrackPoint.h"

namespace Herd::SSE
{

// Forward declarations
class IPhase;

/**
 * @brief Implements the single star evolution
 * @cite Hurley00
 * @cite Belczynski02
 * @cite Han95
 */
class SingleStarEvolutuion
{
public:

  /**
   * @brief Parameters
   * @remarks The actual timesteps are such that the mass loss between two consecutive time points do not exceed %1. There is also a time point at the beginning of each stage
   */
  struct Parameters
  {
    double m_Eta = 0.5; ///< Reimers mass loss efficiency. >=0
    double m_HeWind = 1.;  ///< Helium star mass loss factor. >=0
    double m_BinaryWind = 0;  ///< Mass loss parameter in binary stars. >=0
    double m_RocheLobe = 0;  ///< Roche lobe factor for binary stars. >=0

    double m_SupernovaKickDispersion = 190.; ///< Dispersion in the Maxwellian for supernova kick speed, km/s. >=0
    uint_fast64_t m_Seed = 0;  ///< Random number seed for supernova kick

    bool m_UseHanIFMR = false;  ///< If \c true uses Han95 for the initial-final mass relation for white dwarves
    bool m_UseModifiedMestel = true;  ///< If \c true uses modified Mestel cooling for white dwarves
    bool m_AllowVelocityKickForBlackHoles = false;  ///< If \c true, velocity kick for black holes
    bool m_UseBelczynskiMass = true;  ///< Compute neutron star and black hole masses by Belczynski02
    double m_MaxNSMass = 3.;  ///< Maximum neutron star mass. If Parameters::m_UseBelczynskiMass is \c false, 1.8

    //@formatter:off
      std::unordered_map< Herd::SSE::EvolutionStage, double > m_RelativeTimeStepSizes {
        { Herd::SSE::EvolutionStage::e_MSLM, 0.05 },
        { Herd::SSE::EvolutionStage::e_MS, 0.05 },
        { Herd::SSE::EvolutionStage::e_HG, 0.02 },
        { Herd::SSE::EvolutionStage::e_FGB, 0.01 },
        { Herd::SSE::EvolutionStage::e_CHeB, 0.01 },
        { Herd::SSE::EvolutionStage::e_FAGB, 0.01 },
        { Herd::SSE::EvolutionStage::e_SAGB, 0.01 },
        { Herd::SSE::EvolutionStage::e_HeMS, 0.02 },
        { Herd::SSE::EvolutionStage::e_HeGB, 0.01 }
      }; ///< Preferred time step size at each evolutionary stage, as a percentage of the duration of the phase. >0
                        //@formatter:on

    double m_DefaultTimestep = 0.01;  ///< Default timestep size as a percentage of the duration of a phase. >0
    double m_MinRemnantTimestep = 0.1; ///< Minimum timestep for evolution of a remnant, in Myr. >0
  };

  void Evolve( Herd::Generic::Mass i_Mass, Herd::Generic::Metallicity i_Z, Herd::Generic::Age i_EvolveUntil, const Parameters& i_rParameters ); ///< Evolves a star

  const std::vector< Herd::SSE::TrackPoint >& Trajectory() const;  ///< Accessor for SingleStarEvolutuion::m_Trajectory

private:

  static void Validate( const Parameters& i_rParameters ); ///< Validates parameters
  static void Validate( Herd::Generic::Mass i_Mass, Herd::Generic::Metallicity i_Z, Herd::Generic::Age i_EvolveUntil );  ///< Validates the input arguments

  unsigned int EstimateTrajectoryLength( const Parameters& i_rParameters ); ///< Estimates the total number of timesteps
  static Herd::Generic::Age ComputeTimestep( Herd::SSE::IPhase& io_rPhase, const Herd::SSE::EvolutionState i_rState,
      const Parameters& i_rParameters, Herd::Generic::Age i_EvolveUntil ); ///< Computes the size of the timestep

  std::vector< Herd::SSE::TrackPoint > m_Trajectory; ///< Evolution trajectory
};

/**
 * @brief Algorithm specs for SingleStarEvolutuion
 * @cite AMUSE
 */
struct SingleStarEvolutuionSpecs
{
  // Domain
  inline static const Herd::Generic::ClosedRange s_MassRange = Herd::Generic::ClosedRange( 0.1, 100. ); ///< Valid mass range
  inline static const Herd::Generic::ClosedRange s_ZRange = Herd::Generic::ClosedRange( 1e-4, 0.03 ); ///< Valid metallicity range
};
}

#endif /* H5CFC91CB_E335_485A_9D7D_189B85CF3E28 */
