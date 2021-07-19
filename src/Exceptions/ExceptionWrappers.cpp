/**
 * @file ExceptionWrappers.cpp
 * @author Evren Imre
 * @date 19 Jul 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "ExceptionWrappers.h"

#include "PreconditionError.h"

namespace Herd::Exceptions
{

/**
 * @param i_rElement Entity under test
 * @param i_rExpected Precondition
 * @param i_rActual Actual value
 * @throws PreconditionError
 */
void ThrowPreconditionError( const std::string_view& i_rElement, const std::string_view& i_rExpected, const std::string_view& i_rActual )
{
  throw( Herd::Exceptions::PreconditionError( i_rElement, i_rExpected, i_rActual ) );
}

/**
 * @param i_rElement Entity under test
 * @param i_rExpected Precondition
 * @param i_Actual Actual value
 * @throws PreconditionError
 * @remarks Not a template because the point of the wrappers is keeping the included headers to a minimum. Template would require either string.h or PreconditionError.h. This wrapper covers the most common case.
 */
void ThrowPreconditionError( const std::string_view& i_rElement, const std::string_view& i_rExpected, double i_Actual )
{
  throw( Herd::Exceptions::PreconditionError( i_rElement, i_rExpected, i_Actual ) ); // @suppress("Symbol is not resolved")
}

/**
 * @brief Throws if value is negative
 * @param i_Value Value to be tested
 * @param i_rName Name of the variable
 * @throws PreconditionError If \c i_Value is negative
 */
void ThrowPreconditionErrorIfNegative( double i_Value, const std::string_view& i_rName )
{
  Herd::Exceptions::PreconditionError::ThrowIfNegative( i_Value, i_rName ); // @suppress("Invalid arguments")
}

void ThrowPreconditionErrorIfNotPositive( double i_Value, const std::string_view& i_rName )
{
  Herd::Exceptions::PreconditionError::ThrowIfNotPositive( i_Value, i_rName ); // @suppress("Invalid arguments")
}

}



