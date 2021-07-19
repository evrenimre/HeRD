/**
 * @file ExceptionWrappers.h
 * @author Evren Imre
 * @date 19 Jul 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef H762D5200_8072_4BCE_ACD1_9CD77A317A3B
#define H762D5200_8072_4BCE_ACD1_9CD77A317A3B

#include <string_view>

namespace Herd::Exceptions
{
// Exception wrappers help reduce dependencies: more expensive headers are included only in the source file
// This unfortunately prevents the use of templates, so where necessary, only wrappers for the most common use cases are provided

[[noreturn]] void ThrowPreconditionError( const std::string_view& i_rElement, const std::string_view& i_rExpected, const std::string_view& i_rActual ); ///< Wrapper for PreconditionError
[[noreturn]] void ThrowPreconditionError( const std::string_view& i_rElement, const std::string_view& i_rExpected, double i_Actual ); ///< Wrapper for PreconditionError
void ThrowPreconditionErrorIfNegative( double i_Value, const std::string_view& i_rName );  ///< Wrapper for Precondition error with condition check
void ThrowPreconditionErrorIfNotPositive( double i_Value, const std::string_view& i_rName ); ///< Wrapper for Precondition error with condition check

}

#endif /* H762D5200_8072_4BCE_ACD1_9CD77A317A3B */
