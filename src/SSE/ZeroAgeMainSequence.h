/**
 * @file ZeroAgeMainSequence.h
 * @author Evren Imre
 * @date 4 Nov 2020	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HD2D9C3D9_9FCD_46C9_ABA8_24F21756CD03
#define HD2D9C3D9_9FCD_46C9_ABA8_24F21756CD03

#include "StarState.h"

#include "Generic/Quantities.h"
#include "Generic/QuantityRange.h"

#include "Eigen/Core"

namespace Herd::SSE
{

namespace Detail::ZAMS
{
// Algorithm properties
// Original mass range [0.1, 100] and z range [1e-4, 0.03]
// Reported radius error 0.05 and luminosity error 0.075
// Values updated by running the algorithm over the ZAMS unit test dataset
inline constexpr double s_MinMass = 0.1;  ///< Lower bound of the mass range
inline constexpr double s_MaxMass = 40.0;  ///< Upper bound of the mass range
inline constexpr double s_MinZ = 1e-4; ///< Lower bound of the metallicity range
inline constexpr double s_MaxZ = 0.025; ///< Upper bound of the metallicity range
inline constexpr double s_MaxLuminosityError = 0.7;  ///< Maximum luminosity error in percentage
inline constexpr double s_MaxRadiusError = 0.125;  ///< Maximum radius error in percentage
inline constexpr double s_MaxTemperatureError = 0.08; ///< Maximum temperature error in percentage. Approximate computation from L and R errors
}

/**
 * @brief Computes the ZAMS state
 * @cite Tout96
 */
class ZeroAgeMainSequence
{
public:

  static Herd::SSE::StarState ComputeStarState( Herd::Generic::Mass i_Mass, Herd::Generic::Metallicity i_Z ); ///< Computes the star state at ZAMS

private:
  static void Validate( Herd::Generic::Mass i_Mass, Herd::Generic::Metallicity i_Z ); ///< Validates the input

  static Herd::Generic::Luminosity ComputeLuminosity( Herd::Generic::Mass i_Mass, const Eigen::Matrix< double, 5, 1 >& i_ZVector ); ///< Computes the ZAMS luminosity
  static Herd::Generic::Radius ComputeRadius( Herd::Generic::Mass i_Mass, const Eigen::Matrix< double, 5, 1 >& i_ZVector ); ///< Computes the ZAMS radius

  inline static const Herd::Generic::ClosedRange s_MassRange = Herd::Generic::ClosedRange( Detail::ZAMS::s_MinMass, Detail::ZAMS::s_MaxMass ); ///< Valid mass range
  inline static const Herd::Generic::ClosedRange s_ZRange = Herd::Generic::ClosedRange( Detail::ZAMS::s_MinZ, Detail::ZAMS::s_MaxZ ); ///< Valid metallicity range
};
}

#endif /* HD2D9C3D9_9FCD_46C9_ABA8_24F21756CD03 */
