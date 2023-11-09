/**
 * @file BaseOfGiantBranch.h
 * @author Evren Imre
 * @date 28 Oct 2023	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2023 Evren Imre
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef H0A6D735C_7542_4D2B_9796_8E32EEB127F1
#define H0A6D735C_7542_4D2B_9796_8E32EEB127F1

#include <Generic/Quantity.h>

#include <array>
#include <memory>
#include <optional>
#include <utility>

namespace Herd::SSE
{

class GiantBranchRadius;

/**
 * @brief Computations for characteristic properties at BGB
 * @cite Hurley00
 */
class BaseOfGiantBranch
{
public:

  BaseOfGiantBranch( Herd::Generic::Metallicity i_Z ); ///< Constructor
  ~BaseOfGiantBranch(); ///< Destructor

  Herd::Generic::Time Age( Herd::Generic::Mass i_Mass );  ///< Returns \f$ t_{BGB} \f$
  Herd::Generic::Luminosity Luminosity( Herd::Generic::Mass i_Mass );  ///< Returns \f$ L_{BGB} \f$
  Herd::Generic::Radius Radius( Herd::Generic::Mass i_Mass );  ///< Returns \f$ R_{BGB} \f$

private:

  void ComputeMetallicityDependents( Herd::Generic::Metallicity i_Z ); ///< Computes various metallicity-dependent quantities

  Herd::Generic::Time ComputeAge( Herd::Generic::Mass i_Mass ) const;  ///< Computes \f$ T_{BGB}\f$
  Herd::Generic::Luminosity ComputeLuminosity( Herd::Generic::Mass i_Mass ) const;  ///< Computes \f$ L_{BGB} \f$
  Herd::Generic::Radius ComputeRadius( Herd::Generic::Mass i_Mass ) const;  ///< Computes \f$ R_{BGB} \f$

  /**
   * @brief Various quantities and values that depend on metallicity only
   */
  struct MetallicityDependents
  {
    std::array< double, 5 > m_TBGB; ///< \f$ T_{BGB} \f$ calculations
    std::array< double, 8 > m_LBGB;  ///< \f$ L_{BGB} \f$ calculations

    std::unique_ptr< Herd::SSE::GiantBranchRadius > m_pRGBComputer; ///< Computes the giant branch radius
  };

  MetallicityDependents m_ZDependents;  ///< Metallicity-dependent quantities

  /**
   * @brief Various quantities and values that depend on mass
   * @remarks Note that these quantities also depend on metallicity via the equation coefficients
   */
  struct MassDependents
  {
    using TKey = std::optional< Herd::Generic::Mass >;

    std::pair< TKey, Herd::Generic::Time > m_Age; ///< \f$ t_{BGB} \f$
    std::pair< TKey, Herd::Generic::Luminosity > m_Luminosity; ///< \f$ L_{BGB}\f$
    std::pair< TKey, Herd::Generic::Radius > m_Radius;  ///< \f$ R_{BGB} \f$
  };

  MassDependents m_MDependents; ///< Mass-dependent quantities
};
}




#endif /* H0A6D735C_7542_4D2B_9796_8E32EEB127F1 */
