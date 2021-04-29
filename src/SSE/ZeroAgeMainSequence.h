/**
 * @file ZeroAgeMainSequence.h
 * @author Evren Imre
 * @date 4 Nov 2020	
 */
/* This file is a part of HeRD, a stellar evolution library
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HD2D9C3D9_9FCD_46C9_ABA8_24F21756CD03
#define HD2D9C3D9_9FCD_46C9_ABA8_24F21756CD03

#include "StarState.h"

#include "Generic/Quantities.h"
#include "Generic/QuantityRange.h"

#include <optional>

#include "Eigen/Core"

namespace Herd::SSE
{
/**
 * @brief Computes the ZAMS state
 * @cite Tout96
 */
class ZeroAgeMainSequence
{
public:

  static std::optional< Herd::SSE::StarState > ComputeStarState( Herd::Generic::Mass i_Mass, Herd::Generic::Metallicity i_Z ); ///< Computes the star state at ZAMS

private:
  static bool Validate( Herd::Generic::Mass i_Mass, Herd::Generic::Metallicity i_Z ); ///< Validates the input

  static Herd::Generic::Luminosity ComputeLuminosity( Herd::Generic::Mass i_Mass, const Eigen::Matrix< double, 5, 1 >& i_ZVector ); ///< Computes the ZAMS luminosity
  static Herd::Generic::Radius ComputeRadius( Herd::Generic::Mass i_Mass, const Eigen::Matrix< double, 5, 1 >& i_ZVector ); ///< Computes the ZAMS radius

  inline static const Herd::Generic::ClosedRange s_MassRange = Herd::Generic::ClosedRange( 0.1, 100 );  ///< Valid mass range
  inline static const Herd::Generic::ClosedRange s_ZRange = Herd::Generic::ClosedRange( 1e-4, 0.03 ); ///< Valid metallicity range
};
}

#endif /* HD2D9C3D9_9FCD_46C9_ABA8_24F21756CD03 */
