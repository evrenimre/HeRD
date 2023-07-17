/**
 * @file PreconditionError.cpp
 * @author Evren Imre
 * @date 18 Jul 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "PreconditionError.h"

#include <boost/format.hpp>
#include <boost/stacktrace.hpp>

namespace Herd::Exceptions
{

// Explicit instantiations
///@cond
// Suppress doxygen warnings
template PreconditionError::PreconditionError( const char*, const char*, double ); // @suppress("Member declaration not found")
///@endcond
template void PreconditionError::ThrowIfNegative( double, const char* ); // @suppress("Member declaration not found")
template void PreconditionError::ThrowIfNotPositive( double, const char* ); // @suppress("Member declaration not found")

/**
 * @param i_rElement Element being tested
 * @param i_rExpected Precondition
 * @param i_rActual Actual value
 * @return Error message
 */
std::string PreconditionError::ComposeMessage( const std::string& i_rElement, const std::string& i_rExpected, const std::string& i_rActual )
{
  return boost::str( boost::format( "%s: Expected %s got %s\n" ) % i_rElement % i_rExpected % i_rActual )
      + boost::stacktrace::to_string( boost::stacktrace::stacktrace() );
}
}



