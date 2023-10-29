/**
 * @file GiantBranchRadius.h
 * @author Evren Imre
 * @date 1 Sep 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HF72D459C_7D4D_4F08_84B3_4DE9A7E08995
#define HF72D459C_7D4D_4F08_84B3_4DE9A7E08995

#include <Generic/Quantity.h>

#include <array>
#include <utility>

namespace Herd::SSE
{
/**
 * @brief Giant branch radius computations
 * @cite Hurley00
 */
class GiantBranchRadius
{
public:
  GiantBranchRadius( Herd::Generic::Metallicity i_Z ); ///< Constructor
  Herd::Generic::Radius Compute( Herd::Generic::Mass i_Mass, Herd::Generic::Luminosity i_Luminosity ); ///< Computes the radius

private:

  std::array< double, 7 > m_B;  ///< Coefficients depending on Z
  std::pair< Herd::Generic::Mass, double > m_A; ///< \f$ A \f$ in Eq. 46
};
}



#endif /* HF72D459C_7D4D_4F08_84B3_4DE9A7E08995 */
