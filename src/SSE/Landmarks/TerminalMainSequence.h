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

#include "ILandmark.h"

#include <Generic/Quantity.h>

#include <array>
#include <memory>
#include <optional>
#include <utility>

namespace Herd::SSE
{

class BaseOfGiantBranch;
class ZeroAgeMainSequence;

/**
 * @brief Computes the characteristic values at terminal main sequence
 * @remarks \f$ t_{hook} \f$ is not a characteristic value of TMS, but needed for \f$ t_{MS}\f$ calculations in Hertzsprung gap
 * @cite Hurley00
 */
class TerminalMainSequence : public Herd::SSE::ILandmark
{
public:
  
  TerminalMainSequence( Herd::Generic::Metallicity i_Z ); ///< Constructor
  ~TerminalMainSequence();

  Herd::Generic::Time Age( Herd::Generic::Mass i_Mass );  ///< Returns \f$ t_{TMS} \f$
  Herd::Generic::Luminosity Luminosity( Herd::Generic::Mass i_Mass );  ///< Returns \f$ L_{TMS} \f$
  Herd::Generic::Radius Radius( Herd::Generic::Mass i_Mass );  ///< Returns \f$ R_{TMS} \f$

  Herd::Generic::Time THook( Herd::Generic::Mass i_Mass );  ///< Returns \f$ t_{hook] \f$

private:

  void ComputeMetallicityDependents( Herd::Generic::Metallicity i_Z ); ///< Computes various metallicity-dependent quantities
  void ComputeMassDependents( Herd::Generic::Mass i_Mass ); ///< Computes various mass-dependent quantities

  Herd::Generic::Time ComputeAge( Herd::Generic::Mass i_Mass ) const; ///< Computes \f$ t_{MS} \f$
  Herd::Generic::Luminosity ComputeLuminosity( Herd::Generic::Mass i_Mass ) const;  ///< Computes \f$ L_{TMS} \f$
  Herd::Generic::Radius ComputeRadius( Herd::Generic::Mass i_Mass ) const;  ///< Computes \f$ R_{TMS} \f$
  Herd::Generic::Time ComputeTHook( Herd::Generic::Mass i_Mass ) const; ///< Computes \f$ t_{Hhok} \f$

  /**
   * @brief Various quantities and values that depend on metallicity only
   */
  struct MetallicityDependents
  {
    std::array< double, 13 > m_RTMS; ///< \f$ R_{TMS} \f$ calculations
    std::array< double, 6 > m_LTMS; ///< \f$ L_{TMS} \f$ calculations
    double m_X = 0;  ///< \f$ x \f$ in Eq. 6
    std::array< double, 5 > m_Thook;  ///< \f$ t_{hook} \f$ calculations

    std::unique_ptr< Herd::SSE::ZeroAgeMainSequence > m_pZAMSComputer; ///< ZAMS computations
    std::unique_ptr< Herd::SSE::BaseOfGiantBranch > m_pBGBComputer; ///< BGB computations
  };

  MetallicityDependents m_ZDependents;  ///< Metallicity-dependent quantities

  /**
   * @brief Various quantities and values that depend on mass
   * @remarks Note that these quantities also depend on metallicity via the equation coefficients
   */
  struct MassDependents
  {
    using TKey = std::optional< Herd::Generic::Mass >;

    std::pair< TKey, Herd::Generic::Time > m_Age; ///< \f$ t_{MS} \f$
    std::pair< TKey, Herd::Generic::Luminosity > m_Luminosity; ///< \f$ L_{TMS}\f$
    std::pair< TKey, Herd::Generic::Radius > m_Radius;  ///< \f$ R_{TMS} \f$

    std::pair< TKey, Herd::Generic::Time > m_THook;  ///< \f$ t_{hook} \f$
  };

  MassDependents m_MDependents; ///< Mass-dependent quantities
};
}



#endif /* H003D4153_CA63_4833_B8C8_CD0D78C0D419 */
