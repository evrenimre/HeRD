/**
 * @file TerminalMainSequence.h
 * @author Evren Imre
 * @date 30 Oct 2023	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2023 Evren Imre
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef H003D4153_CA63_4833_B8C8_CD0D78C0D419
#define H003D4153_CA63_4833_B8C8_CD0D78C0D419

#include <Generic/Quantity.h>

#include <array>
#include <utility>

namespace Herd::SSE
{

/**
 * @brief Computes the characteristic values at terminal main sequence
 * @cite Hurley00
 */
class TerminalMainSequence
{
public:
  
  TerminalMainSequence( Herd::Generic::Metallicity i_Z ); ///< Constructor

  void Compute( Herd::Generic::Mass i_Mass, Herd::Generic::Radius i_RZAMS, Herd::Generic::Time i_TBGB ); ///< Computes the characteristic properties

  // Accessors
  Herd::Generic::Time Age() const;  ///< Returns \f$ t_{TMS} \f$
  Herd::Generic::Luminosity Luminosity() const;  ///< Returns \f$ L_{TMS} \f$
  Herd::Generic::Radius Radius() const;  ///< Returns \f$ R_{TMS} \f$
  Herd::Generic::Time THook() const;  ///< Returns \f$ t_{hook] \f$

private:

  void ComputeMetallicityDependents( Herd::Generic::Metallicity i_Z ); ///< Computes various metallicity-dependent quantities
  void ComputeMassDependents( Herd::Generic::Mass i_Mass, Herd::Generic::Radius i_RZAMS, Herd::Generic::Time i_TBGB ); ///< Computes various mass-dependent quantities

  Herd::Generic::Time ComputeTMS( Herd::Generic::Time i_TBGB, Herd::Generic::Time i_THook ) const; ///< Computes the age at terminal main sequence
  Herd::Generic::Luminosity ComputeLTMS( Herd::Generic::Mass i_Mass ) const;  ///< Computes the terminal main sequence luminosity
  Herd::Generic::Radius ComputeRTMS( Herd::Generic::Mass i_Mass, Herd::Generic::Radius i_RZAMS ) const;  ///< Computes the terminal main sequence radius
  Herd::Generic::Time ComputeTHook( Herd::Generic::Mass i_Mass, Herd::Generic::Time i_TBGB ); ///< Age at the appearance of the hook

  /**
   * @brief Various quantities and values that depend on metallicity only
   */
  struct MetallicityDependents
  {
    std::array< double, 13 > m_RTMS; ///< \f$ R_{TMS} \f$ calculations
    std::array< double, 6 > m_LTMS; ///< \f$ L_{TMS} \f$ calculations
    double m_X = 0;  ///< \f$ x \f$ in Eq. 6
    std::array< double, 5 > m_Thook;  ///< \f$ T_{hook} \f$ calculations
  };

  MetallicityDependents m_ZDependents;  ///< Metallicity-dependent quantities

  /**
   * @brief Various quantities and values that depend on mass
   * @remarks Note that these quantities also depend on metallicity via the equation coefficients
   */
  struct MassDependents
  {
    std::pair< Herd::Generic::Mass, Herd::Generic::Radius > m_EvaluatedAt; ///< Dependents calculated at this value

    Herd::Generic::Time m_TMS; ///< Age at TMS
    Herd::Generic::Luminosity m_LTMS; ///< Luminosity at TMS
    Herd::Generic::Radius m_RTMS;  ///< Radius at TMS
    Herd::Generic::Time m_THook; ///< Age at the appearance of the hook
  };

  MassDependents m_MDependents; ///< Mass-dependent quantities
};
}



#endif /* H003D4153_CA63_4833_B8C8_CD0D78C0D419 */
