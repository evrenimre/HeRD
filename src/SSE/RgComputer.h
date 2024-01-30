/**
 * @file RgComputer.h
 * @author Evren Imre
 * @date 12 Dec 2023	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2023 Evren Imre
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HE4072FE9_41B4_4973_9343_922455B7EA1D
#define HE4072FE9_41B4_4973_9343_922455B7EA1D

#include "EvolutionState.h"

#include <Generic/Quantity.h>

#include <memory>

namespace Herd::SSE
{

// Forward declarations
class BaseOfGiantBranch;

/**
 * @brief Computes \f$ R_g \f$ for convective envelope calculations
 * @remarks \f$ R_g \f$ is the giant branch or the Hayashi radius, depending on the state
 */
class RgComputer
{
public:

  RgComputer( Herd::Generic::Metallicity i_Z );  ///< Constructor
  ~RgComputer(); ///< Destructor

  Herd::Generic::Radius ComputeRg( const Herd::SSE::EvolutionState& i_rState ); ///< Computes \f$ R_g \f$

private:

  /**
   * @brief Various quantities and values that depend on metallicity only
   */
  struct MetallicityDependents
  {
    std::unique_ptr< Herd::SSE::BaseOfGiantBranch > m_pBGBComputer; ///< Computes the characteristic values at BGB
  };

  MetallicityDependents m_ZDependents;  ///< Metallicity-dependent quantities evaluated at initial metallicity

};
}



#endif /* HE4072FE9_41B4_4973_9343_922455B7EA1D */
