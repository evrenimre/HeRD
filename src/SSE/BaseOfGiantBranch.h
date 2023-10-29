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

namespace Herd::SSE
{
/**
 * @brief Computations for characteristic properties at the base of the giant branch
 * @cite Hurley00
 */
class BaseOfGiantBranch
{
public:

  BaseOfGiantBranch( Herd::Generic::Metallicity i_Z ); ///< Constructor
  void Compute( Herd::Generic::Mass i_Mass ); ///< Computes the characteristic properties

  Herd::Generic::Time TBGB() const;  ///< Returns the age at BGB
  Herd::Generic::Luminosity LBGB() const;  ///< Returns the luminosity at BGB

private:

  void ComputeMetallicityDependents( Herd::Generic::Metallicity i_Z ); ///< Computes various metallicity-dependent quantities
  void ComputeMassDependents( Herd::Generic::Mass i_Mass ); ///< Computes various mass-dependent quantities

  Herd::Generic::Luminosity ComputeLBGB( Herd::Generic::Mass i_Mass ) const;  ///< Computes the luminosity at the base of the giant branch
  Herd::Generic::Time ComputeTBGB( Herd::Generic::Mass i_Mass ) const;  ///< Computes the age at the base of the giant branch

  /**
   * @brief Various quantities and values that depend on metallicity only
   */
  struct MetallicityDependents
  {
    std::array< double, 5 > m_TBGB; ///< \f$ T_{BGB} \f$ calculations
    std::array< double, 8 > m_LBGB;  ///< \f$ L_{BGB} \f$ calculations
  };

  MetallicityDependents m_ZDependents;  ///< Metallicity-dependent quantities

  /**
   * @brief Various quantities and values that depend on mass
   * @remarks Note that these quantities also depend on metallicity via the equation coefficients
   */
  struct MassDependents
  {
    Herd::Generic::Mass m_EvaluatedAt; ///< Dependents calculated at this value

    Herd::Generic::Time m_TBGB; ///< Age at BGB
    Herd::Generic::Luminosity m_LBGB; ///< Luminosity at BGB
  };

  MassDependents m_MDependents; ///< Mass-dependent quantities
};
}




#endif /* H0A6D735C_7542_4D2B_9796_8E32EEB127F1 */
