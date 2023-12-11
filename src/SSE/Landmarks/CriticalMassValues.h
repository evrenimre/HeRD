/**
 * @file CriticalMassValues
 * @author Evren Imre
 * @date 8 Dec 2023	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2023 Evren Imre
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef H04D2712F_9CAE_47AF_A497_C92A22F30F34
#define H04D2712F_9CAE_47AF_A497_C92A22F30F34

#include <Generic/Quantity.h>

namespace Herd::SSE
{
Herd::Generic::Mass ComputeMhook( Herd::Generic::Metallicity i_Z );  ///< Computes \f$ M_{hook}\f$
Herd::Generic::Mass ComputeMHeF( Herd::Generic::Metallicity i_Z ); ///< Computes \f$ M_{HeF}\f$
Herd::Generic::Mass ComputeMFGB( Herd::Generic::Metallicity i_Z ); ///< Computes \f$ M_{FGB}\f$
}



#endif /* H04D2712F_9CAE_47AF_A497_C92A22F30F34 */
