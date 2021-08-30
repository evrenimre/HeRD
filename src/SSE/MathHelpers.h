/**
 * @file MathHelpers.h
 * @author Evren Imre
 * @date 3 Aug 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef H9D0D6C28_5A38_47E7_BCEC_9B3FE9E2381F
#define H9D0D6C28_5A38_47E7_BCEC_9B3FE9E2381F

#include <array>
#include <cstddef>
#include <utility>

#include "Eigen/Core"
#include <range/v3/algorithm.hpp>
#include <range/v3/view.hpp>

namespace Herd::SSE
{
template< std::size_t Size, std::size_t Row, std::size_t Column >
void MultiplyMatrixVector( std::array< double, Row >& o_rResult, const std::array< double, Size >& i_rMatrix, const std::array< double, Column >& i_rVector ); ///< Matrix-vector multiplication with fixed-size arrays

template< std::size_t N >
double ComputeInnerProduct( const std::array< double, N >& i_rLeft, const std::array< double, N >& i_rRight );  ///< Inner product with fixed-size arrays

template< std::size_t N >
void ComputePowers( std::array< double, N >& o_Result, double i_Base ); ///< Fills an array with the consecutive integer powers of a number starting from 0th power

double ComputeBlendWeight( double i_X, double i_A, double i_B );  ///< Computes \f$ \frac{ x-a }{b-a}\f$

/**
 * @tparam Size Matrix size
 * @tparam Row Number of rows
 * @tparam Column Number of columns
 * @param[out] o_rResult Result
 * @param i_rMatrix Matrix array
 * @param i_rVector Vector array
 * @pre \c Size=Matrix*Vector
 */
template< std::size_t Size, std::size_t Row, std::size_t Column >
void MultiplyMatrixVector( std::array< double, Row >& o_rResult, const std::array< double, Size >& i_rMatrix,
    const std::array< double, Column >& i_rVector )
{
  static_assert( Size == Row * Column );

  const Eigen::Map< Eigen::Matrix< double, Row, Column, Eigen::RowMajor > > matrix( const_cast< double* >( i_rMatrix.data() ) );
  const Eigen::Map< Eigen::Matrix< double, Column, 1 > > vector( const_cast< double* >( i_rVector.data() ) );
  Eigen::Map< Eigen::Matrix< double, Row, 1 > > result( o_rResult.data() );

  result = matrix * vector;
}

/**
 * @tparam N Vector size
 * @param i_rLeft First operand
 * @param i_rRight Second operand
 * @return Dot product
 */
template< std::size_t N >
double ComputeInnerProduct( const std::array< double, N >& i_rLeft, const std::array< double, N >& i_rRight )
{
  const Eigen::Map< Eigen::Matrix< double, 1, N > > left( const_cast< double* >( i_rLeft.data() ) );
  const Eigen::Map< Eigen::Matrix< double, 1, N > > right( const_cast< double* >( i_rRight.data() ) );

  return left.dot( right );
}

/**
 * @tparam N Size of the array
 * @param o_Result Powers of \c i_Base
 * @param i_Base Base
 * @pre \c N>1
 */
template< std::size_t N >
void ComputePowers( std::array< double, N >& o_Result, double i_Base )
{
  static_assert( N > 0 );

  o_Result[ 0 ] = 1;
  ranges::cpp20::for_each( ranges::cpp20::views::iota( static_cast< std::size_t >( 1 ), N ), [ & ]( auto i_Index ) // @suppress("Function cannot be resolved")
      { o_Result[ i_Index ] = o_Result[i_Index-1]*i_Base;} );

}

}

#endif /* H9D0D6C28_5A38_47E7_BCEC_9B3FE9E2381F */
