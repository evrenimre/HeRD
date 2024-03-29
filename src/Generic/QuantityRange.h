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

#include "Quantity.h"

#include <Exceptions/ExceptionWrappers.h>

#include <sstream>
#include <string>

#include <boost/icl/continuous_interval.hpp>

namespace Herd::Generic
{

namespace Detail
{
// Boundary policy types
struct OpenRangeTag;
struct LeftOpenRangeTag;
struct RightOpenRangeTag;
struct ClosedRangeTag;

// Implemented as free functions to reduce the amount of templated code
boost::icl::continuous_interval< double > MakeRange( double i_Min, double i_Max, Detail::OpenRangeTag* );  ///< Makes an open interval
boost::icl::continuous_interval< double > MakeRange( double i_Min, double i_Max, Detail::LeftOpenRangeTag* );  ///< Makes a left-open interval
boost::icl::continuous_interval< double > MakeRange( double i_Min, double i_Max, Detail::RightOpenRangeTag* ); ///< Makes a right-open interval
boost::icl::continuous_interval< double > MakeRange( double i_Min, double i_Max, Detail::ClosedRangeTag* );  ///< Makes a closed interval
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

  const boost::icl::continuous_interval< double >& Range() const; ///< Returns a constant reference to the underlying interval

  std::string GetRangeString() const;  ///< Helper for printing range information

  template< class Tag >
  void ThrowIfNotInRange( Herd::Generic::Quantity< Tag > i_Quantity, const char* i_pName ) const;  ///< Throws if a quantity is not contained

private:

  boost::icl::continuous_interval< double > m_Range;  ///< Interval
};

// Range types
// @formatter:off
using OpenRange = QuantityRange< Detail::OpenRangeTag >; ///< Open interval
using LeftOpenRange = QuantityRange< Detail::LeftOpenRangeTag >; ///< Left-open interval
using RightOpenRange = QuantityRange< Detail::RightOpenRangeTag >; ///< Right-open interval
using ClosedRange = QuantityRange< Detail::ClosedRangeTag >; ///< Closed interval
// @formatter:on

// Extern template declarations
extern template class QuantityRange< Detail::OpenRangeTag > ;
extern template class QuantityRange< Detail::LeftOpenRangeTag > ;
extern template class QuantityRange< Detail::RightOpenRangeTag > ;
extern template class QuantityRange< Detail::ClosedRangeTag > ;

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
const boost::icl::continuous_interval< double >& QuantityRange< BoundaryPolicy >::Range() const
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

/**
 * @tparam Tag Quantity type indicator
 * @param i_Quantity Value to be tested
 * @param i_rName Name of the value under test
 */
template< class BoundaryPolicy >
template< class Tag >
void QuantityRange< BoundaryPolicy >::ThrowIfNotInRange( Herd::Generic::Quantity< Tag > i_Quantity, const char* i_pName ) const
{
  if( !Contains( i_Quantity ) )
  {
    [[unlikely]] Herd::Exceptions::ThrowPreconditionError( i_pName, GetRangeString().c_str(), i_Quantity.Value() );
  }
}

}

#endif /* HAE817853_F0C3_44C9_92ED_19BA7C220E0E */
