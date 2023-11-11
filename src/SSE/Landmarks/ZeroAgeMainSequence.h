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

#include "ILandmark.h"

#include <Generic/Quantity.h>
#include <Generic/QuantityRange.h>

#include <array>
#include <optional>
#include <utility>

namespace Herd::SSE
{

/**
 * @brief Computes the track point at ZAMS
 * @cite Tout96
 */
class ZeroAgeMainSequence : public ILandmark
{
public:

  ZeroAgeMainSequence( Herd::Generic::Metallicity i_Z );  ///< Constructor

  Herd::Generic::Time Age( Herd::Generic::Mass i_Mass ) override;  ///< Returns \f$ t_{ZAMS} \f$
  Herd::Generic::Luminosity Luminosity( Herd::Generic::Mass i_Mass ) override;  ///< Returns \f$ L_{ZAMS} \f$
  Herd::Generic::Radius Radius( Herd::Generic::Mass i_Mass ) override;  ///< Returns \f$ R_{ZAMS} \f$

private:

  void ComputeMetallicityDependents( Herd::Generic::Metallicity i_Z );  ///< Computes the metallicity dependents

  Herd::Generic::Luminosity ComputeLuminosity( Herd::Generic::Mass i_Mass ) const;  ///< Computes \f$ L_{ZAMS} \f$
  Herd::Generic::Radius ComputeRadius( Herd::Generic::Mass i_Mass ) const;  ///< Computes \f$ R_{ZAMS} \f$

  /**
   * @brief Various quantities and values that depend on the metallicity only
   */
  struct MetallicityDependents
  {
    std::array< double, 7 > m_LZAMS;  ///< Coefficients for \f$ L_{ZAMS}\f$
    std::array< double, 9 > m_RZAMS;  ///< Coefficients for \f$ R_{ZAMS}\f$
  };

  MetallicityDependents m_ZDependents;  ///< Metallicity dependents

  /**
   * @brief Various quantities and values that depend on mass
   * @remarks Note that these quantities also depend on metallicity via the equation coefficients
   */
  struct MassDependents
  {
    using TKey = std::optional< Herd::Generic::Mass >;

    std::pair< TKey, Herd::Generic::Luminosity > m_Luminosity; ///< \f$ L_{ZAMS}\f$
    std::pair< TKey, Herd::Generic::Radius > m_Radius;  ///< \f$ R_{ZAMS} \f$
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
};
}
#endif /* HD2D9C3D9_9FCD_46C9_ABA8_24F21756CD03 */
