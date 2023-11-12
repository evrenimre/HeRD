/**
 * @file HeliumIgnition.h
 * @author Evren Imre
 * @date 28 Oct 2023	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2023 Evren Imre
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HC7481676_A169_40BF_94F2_FB5F9ED028D3
#define HC7481676_A169_40BF_94F2_FB5F9ED028D3

#include "ILandmark.h"

#include <Generic/Quantity.h>

#include <array>
#include <optional>
#include <utility>

namespace Herd::SSE
{

/**
 * @brief Computations for characteristic properties at HeI
 * @cite Hurley00
 */
class HeliumIgnition : public ILandmark
{
public:

  HeliumIgnition( Herd::Generic::Metallicity i_Z ); ///< Constructor

  Herd::Generic::Time Age( Herd::Generic::Mass i_Mass ) override;  ///< Returns \f$ t_{HeI} \f$
  Herd::Generic::Luminosity Luminosity( Herd::Generic::Mass i_Mass ) override;  ///< Returns \f$ L_{HeI} \f$
  Herd::Generic::Radius Radius( Herd::Generic::Mass i_Mass ) override;  ///< Returns \f$ R_{HeI} \f$

private:

  void ComputeMetallicityDependents( Herd::Generic::Metallicity i_Z ); ///< Computes various metallicity-dependent quantities

  Herd::Generic::Time ComputeAge( Herd::Generic::Mass i_Mass ) const;  ///< Computes \f$ t_{HeI}\f$
  Herd::Generic::Luminosity ComputeLuminosity( Herd::Generic::Mass i_Mass ) const;  ///< Computes \f$ L_{HeI} \f$
  Herd::Generic::Radius ComputeRadius( Herd::Generic::Mass i_Mass ) const;  ///< Computes \f$ R_{HeI} \f$

  /**
   * @brief Various quantities and values that depend on metallicity only
   */
  struct MetallicityDependents
  {
    std::array< double, 7 > m_THeI; ///< \f$ t_{HeI} \f$ calculations
    std::array< double, 7 > m_LHeI; ///< \f$ L_{HeI} \f$ calculations
    std::array< double, 7 > m_RHeI; ///< \f$ R_{HeI} \f$ calculations

    Herd::Generic::Mass m_MHeF;  ///< \f$ M_{HeF} \f$
  };

  MetallicityDependents m_ZDependents;  ///< Metallicity-dependent quantities

  /**
   * @brief Various quantities and values that depend on mass
   * @remarks Note that these quantities also depend on metallicity via the equation coefficients
   */
  struct MassDependents
  {
    using TKey = std::optional< Herd::Generic::Mass >;

    std::pair< TKey, Herd::Generic::Time > m_Age; ///< \f$ t_{HeI} \f$
    std::pair< TKey, Herd::Generic::Luminosity > m_Luminosity; ///< \f$ L_{HeI}\f$
    std::pair< TKey, Herd::Generic::Radius > m_Radius;  ///< \f$ R_{HeI} \f$
  };

  MassDependents m_MDependents; ///< Mass-dependent quantities

};
}



#endif /* HC7481676_A169_40BF_94F2_FB5F9ED028D3 */
