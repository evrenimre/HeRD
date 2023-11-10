/**
 * @file ILandmark.h
 * @author Evren Imre
 * @date 10 Nov 2023	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2023 Evren Imre
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef H9B462095_0DAF_47FC_BF8C_B6CFC2594BDC
#define H9B462095_0DAF_47FC_BF8C_B6CFC2594BDC

#include <Generic/Quantity.h>

namespace Herd::SSE
{
/**
 * @brief Interface class for landmarks
 */
class ILandmark
{
public:
  virtual Herd::Generic::Time Age( Herd::Generic::Mass i_Mass ) = 0;  ///< Returns the age at which the landmark occurs
  virtual Herd::Generic::Luminosity Luminosity( Herd::Generic::Mass i_Mass ) = 0;  ///< Returns the luminosity at the landmark
  virtual Herd::Generic::Radius Radius( Herd::Generic::Mass i_Mass ) = 0;  ///< Returns the radius at the landmark

  virtual ~ILandmark() = default;
};
}



#endif /* H9B462095_0DAF_47FC_BF8C_B6CFC2594BDC */
