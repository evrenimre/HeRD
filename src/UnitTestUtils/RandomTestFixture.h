/**
 * @file RandomTestFixture.h
 * @author Evren Imre
 * @date 18 Apr 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef H219F636B_7938_44D6_981D_D0D7FA5DAC8C
#define H219F636B_7938_44D6_981D_D0D7FA5DAC8C

#include <cerrno>
#include <cmath>
#include <cstdint>
#include <limits>
#include <optional>
#include <random>
#include <string>
#include <type_traits>

#include <boost/test/unit_test.hpp>


namespace Herd::UnitTestUtils
{

/**
 * @brief A test fixture for unit tests requiring random values
 * @remarks Seeded with current time
 * @warning Do not use outside of Boost.Test. Some preconditions are enforced by BOOST_TEST_REQUIRED
 */
class RandomTestFixture
{
public:

  RandomTestFixture();  ///< Constructor
  void SetSeed( unsigned int i_Seed ); ///< Reinitialises the random number generator with seed
  uint_fast32_t Seed(); ///< Returns the current seed

  /// @name Random value generators
  ///@{

  bool GenerateBool( double i_Probability = 0.5 );  ///< Generates a boolean

  template< class Arithmetic >
  Arithmetic GenerateNumber( Arithmetic i_Min, Arithmetic i_Max );  ///< Generates a number in the closed range

  template< class Arithmetic >
  Arithmetic GenerateNumber(); ///< Generates a number
  ///@}
  

private:

  void TrySetSeedFromCommandLine(); ///< Attempts to read a seed value from the command line parameters

  uint_fast32_t m_Seed = 0; ///< Seed used for initialising the random number engine
                            // mt19937 seed type is uint_fast32_t
  std::optional< std::mt19937 > m_Rng; ///< Random number generator

  inline static const std::string s_SeedParameterName = "--seed";  ///< Command line parameter name
};

/**
 * @tparam Arithmetic An arithmetic type
 * @param i_Min Minimum value
 * @param i_Max Maximum value
 * @return A random value
 * @pre <tt> i_Min < i_Max </tt>
 * @pre <tt> i_Max - i_Min </tt> can be represented as \c Arithmetic
 */
template< class Arithmetic >
Arithmetic RandomTestFixture::GenerateNumber( Arithmetic i_Min, Arithmetic i_Max )
{
  static_assert( std::is_arithmetic< Arithmetic >::value);

  BOOST_TEST_REQUIRE( i_Min < i_Max );

  // Lazy initialisation. Avoids redundant construction if user sets a different seed before first call to the generator
  // mt19937 constructor is expensive.
  if( !m_Rng )
  {
    m_Rng = std::mt19937( m_Seed );
  }

  if constexpr ( std::is_floating_point< Arithmetic >::value )
  {
    errno = 0;
    Arithmetic span = i_Max - i_Min;
    BOOST_TEST_REQUIRE( errno != ERANGE, std::strerror( errno ) );

    return std::uniform_real_distribution< Arithmetic >( i_Min, std::nextafter( i_Max, std::numeric_limits< Arithmetic >::max() ) )( *m_Rng );  // Closed range
  } else
  {
    return std::uniform_int_distribution< Arithmetic >( i_Min, i_Max )( *m_Rng ); // Closed range
  }
}

/**
 * @tparam Arithmetic An arithmetic type
 * @return A random value
 */
template< class Arithmetic >
Arithmetic RandomTestFixture::GenerateNumber()
{
  if constexpr ( std::is_floating_point< Arithmetic >::value )
  {
    return GenerateNumber( std::numeric_limits< Arithmetic >::lowest() / 2,
        std::nextafter( std::numeric_limits< Arithmetic >::max() / 2, std::numeric_limits< Arithmetic >::lowest() ) ); // This ensures that the span of the range is still representable as an Arithmetic
  } else
  {
    return GenerateNumber( std::numeric_limits< Arithmetic >::lowest(), std::numeric_limits< Arithmetic >::max() ); // It seems like internally integers are promoted to floating point numbers if the span > the maximum value of Arithmetic
  }
}
} // namespace Herd::UnitTestUtils

#endif /* H219F636B_7938_44D6_981D_D0D7FA5DAC8C */
