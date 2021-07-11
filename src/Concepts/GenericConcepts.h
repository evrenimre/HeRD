/**
 * @file Concepts.h
 * @author Evren Imre
 * @date 11 Jul 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HEC8CAA41_DDD7_4F5D_B01E_3838B9848220
#define HEC8CAA41_DDD7_4F5D_B01E_3838B9848220

#include <concepts>
#include <type_traits>

namespace Herd::Concepts
{

template< typename T >
concept Arithmetic = std::is_arithmetic< T >::value; // @suppress("Type cannot be resolved")

template< typename T >
concept Number = std::integral< T > || std::floating_point< T >; // @suppress("Type cannot be resolved")
}

#endif /* HEC8CAA41_DDD7_4F5D_B01E_3838B9848220 */
