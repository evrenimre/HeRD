/**
 * @file Utilities.h
 * @author Evren Imre
 * @date 11 Nov 2023	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2023 Evren Imre
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HCF240618_F624_4732_A909_FD02EDD0311C
#define HCF240618_F624_4732_A909_FD02EDD0311C

#include <Generic/Quantity.h>

#include <optional>

namespace Herd::SSE
{
template< class TValue, class TCallable >
auto UpdateCache( std::pair< std::optional< Herd::Generic::Mass >, TValue >& io_rCache, Herd::Generic::Mass i_Mass, const TCallable& i_Computer ); ///< Updates a cache entry
}


#include "Utilities.hpp"

#endif /* HCF240618_F624_4732_A909_FD02EDD0311C */
