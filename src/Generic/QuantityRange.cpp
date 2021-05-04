/**
 * @file QuantityRange.cpp
 * @author Evren Imre
 * @date 13 Apr 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "QuantityRange.h"

namespace Herd::Generic::Detail
{
/**
 * @param i_Min Lower bound
 * @param i_Max Upper bound
 * @return An open interval
 */
boost::icl::continuous_interval< double > MakeRange( double i_Min, double i_Max, Detail::OpenRange* )
{
  return boost::icl::continuous_interval< double >::open( i_Min, i_Max );
}

/**
 * @param i_Min Lower bound
 * @param i_Max Upper bound
 * @return A left-open interval
 */
boost::icl::continuous_interval< double > MakeRange( double i_Min, double i_Max, Detail::LeftOpenRange* )
{
  return boost::icl::continuous_interval< double >::left_open( i_Min, i_Max );
}

/**
 * @param i_Min Lower bound
 * @param i_Max Upper bound
 * @return A right-open interval
 */
boost::icl::continuous_interval< double > MakeRange( double i_Min, double i_Max, Detail::RightOpenRange* )
{
  return boost::icl::continuous_interval< double >::right_open( i_Min, i_Max );
}

/**
 * @param i_Min Lower bound
 * @param i_Max Upper bound
 * @return A closed interval
 */
boost::icl::continuous_interval< double > MakeRange( double i_Min, double i_Max, Detail::ClosedRange* )
{
  return boost::icl::continuous_interval< double >::closed( i_Min, i_Max );
}
} // namespace Herd::Generic::Detail



