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

#include "TrackPoint.h"

#include <Generic/Quantity.h>
#include <Generic/QuantityRange.h>

#include <array>

namespace Herd::SSE
{

/**
 * @brief Computes the track point at ZAMS
 * @cite Tout96
 * @remarks Only computes the luminosity, radius and temperature
 */
class ZeroAgeMainSequence
{
public:

  ZeroAgeMainSequence( Herd::Generic::Metallicity i_Metallicity );  ///< Constructor
  Herd::SSE::TrackPoint Compute( Herd::Generic::Mass i_Mass ); ///< Computes the luminosity, radius and temperature at ZAMS

private:

  void ComputeMetallicityDependents( Herd::Generic::Metallicity i_Metallicity );  ///< Computes metallicity dependents

  void ComputeMassDependents( Herd::Generic::Mass i_Mass );  ///< Computes mass dependents
  Herd::Generic::Luminosity ComputeLuminosity( Herd::Generic::Mass i_Mass ) const;  ///< Computes \f$ L_{ZAMS} \f$
  Herd::Generic::Radius ComputeRadius( Herd::Generic::Mass i_Mass ) const;  ///< Computes \f$ R_{ZAMS} \f$

  /**
   * @brief Various quantities and values that depend on metallicity only
   */
  struct MetallicityDependents
  {
    Herd::Generic::Metallicity m_EvaluatedAt; ///< Dependents calculated at this value

    std::array< double, 7 > m_LZAMS;  ///< Coefficients for \f$ L_{ZAMS}\f$
    std::array< double, 9 > m_RZAMS;  ///< Coefficients for \f$ R_{ZAMS}\f$
  };

  MetallicityDependents m_ZDependents;  ///< Metallicity dependents

  /**
   * @brief Various quantities and values that depend on mass and metallicity
   */
  struct MassDependents
  {
    Herd::Generic::Mass m_EvaluatedAt;  ///< Dependents calculated at this value

    Herd::Generic::Luminosity m_LZAMS; ///< \f$ L_{ZAMS}\f$
    Herd::Generic::Radius m_RZAMS; ///< \f$ R_{ZAMS}\f$
    Herd::Generic::Temperature m_TZAMS;  ///< \f$ T_{ZAMS}\f$
  };

  MassDependents m_MDependents; ///< Mass dependents
};

/**
 * @brief Algorithm specs for ZeroAgeMainSequence
 * @remarks In the original paper, the mass range is \c [0.1, 100]. This is modified to \c [0.2, 100] to achieve a tighter tolerance
 */
struct ZeroAgeMainSequenceSpecs
{
  // Domain
  inline static const Herd::Generic::ClosedRange s_MassRange = Herd::Generic::ClosedRange( 0.2, 100. ); ///< Valid mass range
  inline static const Herd::Generic::ClosedRange s_ZRange = Herd::Generic::ClosedRange( 1e-4, 0.03 ); ///< Valid metallicity range

  // Error tolerance
  static constexpr double s_MaxLuminosityError = 0.075;  ///< Maximum percent luminosity error
  static constexpr double s_MaxRadiusError = 0.05;  ///< Maximum percent radius error
  static constexpr double s_MaxTemperatureError = 0.05; ///< Maximum percent temperature error. Estimated from the luminosity and radius tolerances
};
}
#endif /* HD2D9C3D9_9FCD_46C9_ABA8_24F21756CD03 */
