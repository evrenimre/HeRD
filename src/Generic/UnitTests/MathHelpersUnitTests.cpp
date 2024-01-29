/**
 * @file MathHelpersUnitTests.cpp
 * @author Evren Imre
 * @date 14 Dec 2023	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2023 Evren Imre
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <boost/test/unit_test.hpp>

#include <Generic/MathHelpers.h>

#include <UnitTestUtils/RandomTestFixture.h>
#include <UnitTestUtils/UnitTestUtilityFunctions.h>

#include <array>
#include <cmath>
#include <limits>

#include <range/v3/algorithm.hpp>
#include <range/v3/numeric.hpp>
#include <range/v3/view.hpp>

BOOST_FIXTURE_TEST_SUITE( MathHelpersTests, Herd::UnitTestUtils::RandomTestFixture, *boost::unit_test::tolerance(1e-8) )

// When there is no alternative implementation, we are testing the consistency

BOOST_AUTO_TEST_CASE( ScalarOperationsTest, *Herd::UnitTestUtils::Labels::s_Compile)
{
  double a = GenerateNumber( -1.0, 1.0 ); // @suppress("Invalid arguments")
  double x = GenerateNumber( std::numeric_limits< double >::min(), 1.0 ); // std::pow raises a number of errors for non-positive base  // @suppress("Invalid arguments")
  double b = GenerateNumber( -1.0, 1.0 ); // @suppress("Invalid arguments")
  double c = GenerateNumber( -1.0, 1.0 ); // @suppress("Invalid arguments")

  double bxhc = Herd::Generic::BXhC( x, b, c );
  BOOST_TEST( bxhc * Herd::Generic::BXhC( x, 1./b, -c ) == 1. ); // @suppress("Method cannot be resolved") // @suppress("Invalid arguments")
  BOOST_TEST( bxhc * Herd::Generic::BXhC( 1./x, 1./b, c ) == 1. ); // @suppress("Method cannot be resolved") // @suppress("Invalid arguments")
  BOOST_TEST( bxhc + Herd::Generic::BXhC( x, -b, c) == 0. );  // @suppress("Method cannot be resolved") // @suppress("Invalid arguments")

  double apbxhc = Herd::Generic::ApBXhC( x, a, b, c );
  BOOST_TEST( apbxhc + Herd::Generic::ApBXhC( x, -a, -b, c) == 0. );  // @suppress("Method cannot be resolved") // @suppress("Invalid arguments")
  BOOST_TEST( apbxhc - bxhc == a ); // @suppress("Method cannot be resolved") // @suppress("Invalid arguments")
}

BOOST_AUTO_TEST_CASE( ComputeBlendWeightTest, *Herd::UnitTestUtils::Labels::s_Compile)
{
  double a = 0.;
  double b = 0.;

  while( a == b )
  {
    a = GenerateNumber( -1.0, 1.0 ); // @suppress("Invalid arguments")
    b = GenerateNumber( -1.0, 1.0 ); // @suppress("Invalid arguments")
  }

  BOOST_TEST( Herd::Generic::ComputeBlendWeight( a, a, b ) == 0. ); // @suppress("Method cannot be resolved") // @suppress("Invalid arguments")
  BOOST_TEST( Herd::Generic::ComputeBlendWeight( b, a, b ) == 1. ); // @suppress("Method cannot be resolved") // @suppress("Invalid arguments")
  BOOST_TEST( Herd::Generic::ComputeBlendWeight( 0.5*(a+b), a, b ) == 0.5 );  // @suppress("Method cannot be resolved") // @suppress("Invalid arguments")

  BOOST_TEST( std::isnan( Herd::Generic::ComputeBlendWeight( a, a, a ) ) );
  BOOST_TEST( std::isinf( Herd::Generic::ComputeBlendWeight( b, a, a ) ) );

  double left = GenerateNumber( -2., -1.1 );   // @suppress("Invalid arguments")
  double right = GenerateNumber( 1.1, 2. );  // @suppress("Invalid arguments")

  if( a < b )
  {
    BOOST_TEST( Herd::Generic::ComputeBlendWeight( left, a, b ) < 0. ); // @suppress("Method cannot be resolved") // @suppress("Invalid arguments")
    BOOST_TEST( Herd::Generic::ComputeBlendWeight( right, a, b ) > 1. ); // @suppress("Method cannot be resolved") // @suppress("Invalid arguments")
  } else
  {
    BOOST_TEST( Herd::Generic::ComputeBlendWeight( left, a, b ) > 1. ); // @suppress("Method cannot be resolved") // @suppress("Invalid arguments")
    BOOST_TEST( Herd::Generic::ComputeBlendWeight( right, a, b ) < 0. ); // @suppress("Method cannot be resolved") // @suppress("Invalid arguments")
  }
}

BOOST_AUTO_TEST_CASE( ComputePowersTest, *Herd::UnitTestUtils::Labels::s_Compile)
{
  double base = GenerateNumber( std::numeric_limits< double >::min(), 1.0 ); // std::pow raises a number of errors for non-positive base  // @suppress("Invalid arguments")
  constexpr std::size_t size = 5;
  BOOST_REQUIRE( size > 1 );

  std::array< double, size > powers;
  Herd::Generic::ComputePowers( powers, base );

  BOOST_TEST( powers[0] == 1. );  // @suppress("Method cannot be resolved") // @suppress("Invalid arguments")
  BOOST_TEST( powers[1] == base );  // @suppress("Method cannot be resolved") // @suppress("Invalid arguments")

  std::size_t idx = GenerateNumber( 1ul, size - 1 );  // @suppress("Invalid arguments")
  BOOST_TEST( powers[idx]/powers[idx-1] == base );  // @suppress("Method cannot be resolved") // @suppress("Invalid arguments")
}

BOOST_AUTO_TEST_CASE( MatrixOperationsTest, *Herd::UnitTestUtils::Labels::s_Compile)
{
  constexpr std::size_t rowCount = 2;
  BOOST_REQUIRE( rowCount > 0 );

  constexpr std::size_t colCount = 3;
  BOOST_REQUIRE( colCount > 0 );

  std::array< double, colCount > vector1;
  ranges::cpp20::generate( vector1, [ & ]()
      { return GenerateNumber(-1.0, 1.0);} ); // @suppress("Invalid arguments")

  std::array< double, colCount > vector2;
  ranges::cpp20::generate( vector2, [ & ]()
      { return GenerateNumber(-1.0, 1.0);} ); // @suppress("Invalid arguments")

  {
    double expected = ranges::inner_product( vector1, vector2, 0.0 ); // @suppress("Function cannot be resolved")
    double actual = Herd::Generic::ComputeInnerProduct( vector1, vector2 );
    BOOST_TEST( expected == actual ); // @suppress("Method cannot be resolved") // @suppress("Invalid arguments")
  }

  std::array< double, rowCount * colCount > matrix;
  ranges::cpp20::generate( matrix, [ & ]()
      { return GenerateNumber(-1.0, 1.0);} ); // @suppress("Invalid arguments")

  {
    std::array< double, rowCount > actual;
    Herd::Generic::MultiplyMatrixVector( actual, matrix, vector1 );

    auto const rowsView = matrix | ranges::views::chunk( colCount ); // @suppress("Function cannot be resolved")
    std::array< double, rowCount > expected;
    ranges::cpp20::transform( rowsView, expected.begin(), [ & ]( const auto& i_rRow )
        { return ranges::inner_product( i_rRow, vector1, 0.0 );} ); // @suppress("Function cannot be resolved")

    BOOST_TEST( actual == expected );
  }

}


BOOST_AUTO_TEST_SUITE_END( )


