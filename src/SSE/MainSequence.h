/**
 * @file MainSequence.h
 * @author Evren Imre
 * @date 30 Jul 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef H9242DC68_449E_4FAF_81AA_13E3C3C156E3
#define H9242DC68_449E_4FAF_81AA_13E3C3C156E3

#include "ConvectiveEnvelope.h"
#include "GiantBranchRadius.h"
#include "IPhase.h"
#include "TrackPoint.h"
#include "ZeroAgeMainSequence.h"

#include <Generic/Quantity.h>

#include <array>
#include <memory>
#include <utility>

namespace Herd::SSE
{
struct EvolutionState;

/**
 * @brief Main sequence evolution
 * @cite Hurley00
 */
class MainSequence : public Herd::SSE::IPhase
{
public:
  MainSequence( Herd::Generic::Metallicity i_Z ); ///< Constructor
  bool Evolve( Herd::SSE::EvolutionState& io_rState ) override; ///< Evolves the state

private:

  void ComputeMetallicityDependents( Herd::Generic::Metallicity i_Z ); ///< Computes various metallicity-dependent quantities

  std::pair< Herd::Generic::Age, Herd::Generic::Age > ComputeTimescales( Herd::Generic::Mass i_Mass ) const; ///< Computes the relevant timescales

  void ComputeMassDependents( Herd::Generic::Mass i_Mass ); ///< Computes various mass-dependent quantities

  Herd::Generic::Luminosity ComputeLTMS( Herd::Generic::Mass i_Mass ) const;  ///< Computes the terminal main sequence luminosity
  double ComputeAlphaL( Herd::Generic::Mass i_Mass ) const; ///< Computes \f$ \alpha_L\f$
  double ComputeBetaL( Herd::Generic::Mass i_Mass ) const; ///< Computes \f$ \beta_L\f$
  double ComputeLHook( Herd::Generic::Mass i_Mass ) const; ///< Computes \f$ \Delta_L\f$

  Herd::Generic::Radius ComputeRTMS( Herd::Generic::Mass i_Mass ) const;  ///< Computes the terminal main sequence radius
  double ComputeAlphaR( Herd::Generic::Mass i_Mass ) const; ///< Computes \f$ \alpha_R\f$
  double ComputeBetaR( Herd::Generic::Mass i_Mass ) const; ///< Computes \f$ \beta_R\f$
  double ComputeGammaR( Herd::Generic::Mass i_Mass ) const; ///< Computes \f$ \beta_R\f$
  double ComputeRHook( Herd::Generic::Mass i_Mass ) const; ///< Computes \f$ \Delta_R\f$

  Herd::Generic::Luminosity ComputeLBGB( Herd::Generic::Mass i_Mass ) const;  ///< Computes the luminosity at the base of the giant branch
  Herd::Generic::Luminosity ComputeLHeI( Herd::Generic::Mass i_Mass ) const;  ///< Computes the luminosity at He ignition

  /**
   * @brief Various quantities and values that depend on metallicity only
   */
  struct MetallicityDependents
  {
    Herd::Generic::Metallicity m_EvaluatedAt; ///< Dependents calculated at this value

    Herd::Generic::Mass m_Mhook;  ///< Minimum initial mass for a hook
    Herd::Generic::Mass m_MHeF; ///< Maximum initial mass for a He flash
    Herd::Generic::Mass m_MFGB; ///< Maximum initial mass for He to ignite on the first giant branch

    double m_X = 0;  ///< \f$ x \f$ in Eq. 6
    double m_MaxEta = 0.;  ///< \f$ \eta \f$ in Eq. 18

    // Equation coefficients
    std::array< double, 5 > m_TBGB; ///< \f$ T_{BGB} \f$ calculations
    std::array< double, 5 > m_Thook;  ///< \f$ T_{hook} \f$ calculations
    std::array< double, 6 > m_LTMS; ///< \f$ L_{TMS} \f$ calculations
    std::array< double, 10 > m_AlphaL;  ///< \f$ \alpha_L \f$ calculations
    std::array< double, 4 > m_BetaL;  ///< \f$ \beta_L \f$ calculations
    std::array< double, 5 > m_Lhook;  ///< \f$ L_{hook} \f$ calculations
    std::array< double, 13 > m_RTMS; ///< \f$ R_{TMS} \f$ calculations
    std::array< double, 12 > m_AlphaR;  ///< \f$ \alpha_R \f$ calculations
    std::array< double, 6 > m_BetaR;  ///< \f$ \beta_R \f$ calculations
    std::array< double, 7 > m_GammaR;  ///< \f$ \gamma_R \f$ calculations
    std::array< double, 7 > m_Rhook;  ///< \f$ \R_{hook} \f$ calculations

    std::array< double, 8 > m_LBGB;  ///< \f$ L_{BGB} \f$ calculations
    std::array< double, 7 > m_LHeI; ///< \f$ L_{HeI} \f$ calculations

    // No default constructor, so needs to be a pointer
    std::unique_ptr< Herd::SSE::ZeroAgeMainSequence > m_pZAMSComputer; ///< Computes the ZAMS parameters
    std::unique_ptr< Herd::SSE::GiantBranchRadius > m_pRGBComputer; ///< Computes the giant branch radius
  };

  MetallicityDependents m_ZDependents;  ///< Metallicity-dependent quantities evaluated at initial  metallicity

  /**
   * @brief Various quantities and values that depend on mass
   * @remarks Note that these quantities also depend on metallicity via the equation coefficients
   */
  struct MassDependents
  {
    Herd::Generic::Mass m_EvaluatedAt; ///< Dependents calculated at this value

    Herd::Generic::Age m_TMS; ///< MS duration
    Herd::Generic::Age m_Thook;  ///< Start of hook formation

    Herd::Generic::Luminosity m_LZAMS;  ///< Luminosity at ZAMS
    Herd::Generic::Radius m_RZAMS; ///< Radius at ZAMS

    Herd::Generic::Luminosity m_LTMS;  ///< Luminosity at TMS
    double m_AlphaL = 0;  ///< \f$ \alpha_L \f$
    double m_BetaL = 0;  ///< \f$ \beta_L \f$
    double m_DeltaL = 0;  ///< \f$ \Delta_L \f$

    double m_Eta = 0;  ///< \f$ \eta \f$

    Herd::Generic::Radius m_RTMS; ///< Radius at TMS
    double m_AlphaR = 0;  ///< \f$ \alpha_R \f$
    double m_BetaR = 0;  ///< \f$ \beta_R \f$
    double m_GammaR = 0;  ///< \f$ \gamma_R \f$
    double m_DeltaR = 0;  ///< \f$ \Delta_R \f$

    Herd::Generic::Luminosity m_LBGB; ///< Luminosity at BGB
    Herd::Generic::Radius m_RBGB;  ///< Radius at BGB

    Herd::Generic::Luminosity m_LHeI; ///< Luminosity at He ignition

    Herd::Generic::Radius m_Rg; ///< \f$ R_g \f$ required for convective envelope calculations
  };

  MassDependents m_MDependents; ///< Mass-dependent quantities evaluated at a certain value

  Herd::Generic::Metallicity m_InitialMetallicity;  ///< Initial value of the metallicity. Used to initialise the track points

  Herd::SSE::ConvectiveEnvelope m_ConvectiveEnvelopeComputer; ///< Computes the properties of the convective envelope
};
}

#endif /* H9242DC68_449E_4FAF_81AA_13E3C3C156E3 */
