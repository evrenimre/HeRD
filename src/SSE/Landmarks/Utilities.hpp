/**
 * @file Utilities.hpp
 * @author Evren Imre
 * @date 11 Nov 2023	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2023 Evren Imre
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef H05618146_02BA_4D36_B70C_268F92A425F0
#define H05618146_02BA_4D36_B70C_268F92A425F0

#include <type_traits>

namespace Herd::SSE
{
/**
 * @tparam TValue Type of the quantity being computed
 * @tparam TCallable Computing function
 * @param[in, out] i_rCache Current cached value. Updated
 * @param i_Mass Mass
 * @param i_Computer Computing function. Passed by reference as it carries captured data
 * @return Computed value
 * @pre \c TCallable can be called with an argument of type \c Mass and return a value
 */
template< class TValue, class TCallable >
auto UpdateCache( std::pair< std::optional< Herd::Generic::Mass >, TValue >& io_rCache, Herd::Generic::Mass i_Mass, const TCallable& i_Computer )
{
  static_assert( std::is_invocable_v< TCallable, Herd::Generic::Mass >);
  static_assert( !std::is_same_v< std::invoke_result<TCallable, Herd::Generic::Mass>, void> );

  auto& [ rKey, rValue ] = io_rCache;

  if( !rKey || *rKey != i_Mass )
  {
    rKey = i_Mass;
    rValue = i_Computer( i_Mass );
  }

  return rValue;
}
}



#endif /* H05618146_02BA_4D36_B70C_268F92A425F0 */
