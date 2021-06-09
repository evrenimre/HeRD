/**
 * @file QuantityRange.h
 * @author Evren Imre
 * @date 12 Apr 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HAE817853_F0C3_44C9_92ED_19BA7C220E0E
#define HAE817853_F0C3_44C9_92ED_19BA7C220E0E

#include "Quantities.h"

#include <sstream>
#include <string>

#include <boost/icl/concept/interval.hpp>
#include <boost/icl/continuous_interval.hpp>

namespace Herd::Generic
{

namespace Detail
{
// Boundary policy types
struct OpenRange;
struct LeftOpenRange;
struct RightOpenRange;
struct ClosedRange;

// Implemented as free functions to reduce the amount of templated code
boost::icl::continuous_interval< double > MakeRange( double i_Min, double i_Max, Detail::OpenRange* );  ///< Makes an open interval
boost::icl::continuous_interval< double > MakeRange( double i_Min, double i_Max, Detail::LeftOpenRange* );  ///< Makes a left-open interval
boost::icl::continuous_interval< double > MakeRange( double i_Min, double i_Max, Detail::RightOpenRange* ); ///< Makes a right-open interval
boost::icl::continuous_interval< double > MakeRange( double i_Min, double i_Max, Detail::ClosedRange* );  ///< Makes a closed interval
}

/**
 * @brief A wrapper for boost::icl::interval for Quantity types
 * @tparam BoundaryPolicy An interval boundary type
 */
template< class BoundaryPolicy >
class QuantityRange
{
public:

  QuantityRange( double i_Min, double i_Max );  ///< Constructor

  template< class Tag >
  bool Contains( Herd::Generic::Quantity< Tag > i_Query ) const; ///< Checks whether a quantity is within range

  double Lower() const; ///< Returns the lower bound
  double Upper() const; ///< Returns the upper bound

  const boost::icl::continuous_interval< double >& Range(); ///< Returns a constant reference to the underlying interval

  std::string GetRangeString() const;  ///< Helper for printing range information

private:

  boost::icl::continuous_interval< double > m_Range;  ///< Interval
};

// Range types
// @formatter:off
using OpenRange = QuantityRange< Detail::OpenRange >; ///< Open interval
using LeftOpenRange = QuantityRange< Detail::LeftOpenRange >; ///< Left-open interval
using RightOpenRange = QuantityRange< Detail::RightOpenRange >; ///< Right-open interval
using ClosedRange = QuantityRange< Detail::ClosedRange >; ///< Closed interval
// @formatter:on

/**
 * @param i_Min Lower bound
 * @param i_Max Upper bound
 */
template< class BoundaryPolicy >
QuantityRange< BoundaryPolicy >::QuantityRange( double i_Min, double i_Max )
{
  BoundaryPolicy* pSelector = nullptr;
  m_Range = Detail::MakeRange( i_Min, i_Max, pSelector );
}

/**
 * @tparam Tag Quantity type indicator
 * @param i_Query Query value
 * @return \c true if the query is within the interval
 */
template< class BoundaryPolicy >
template< class Tag >
bool QuantityRange< BoundaryPolicy >::Contains( Herd::Generic::Quantity< Tag > i_Query ) const
{
  return boost::icl::contains( m_Range, i_Query.Value() );
}

/**
 * @return Lower bound of the interval
 */
template< class BoundaryPolicy >
double QuantityRange< BoundaryPolicy >::Lower() const
{
  return boost::icl::lower( m_Range );
}

/**
 * @return Upper bound of the interval
 */
template< class BoundaryPolicy >
double QuantityRange< BoundaryPolicy >::Upper() const
{
  return boost::icl::upper( m_Range );
}

template< class BoundaryPolicy >
const boost::icl::continuous_interval< double >& QuantityRange< BoundaryPolicy >::Range()
{
  return m_Range;
}

/**
 * @return String for printing the range boundaries
 */
template< class BoundaryPolicy >
std::string QuantityRange< BoundaryPolicy >::GetRangeString() const
{
  std::stringstream Buffer;
  Buffer << m_Range;
  return Buffer.str();
}

}



#endif /* HAE817853_F0C3_44C9_92ED_19BA7C220E0E */
