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

#include <Concepts/GenericConcepts.h>

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
  std::mt19937& Rng(); ///< Returns a reference to RandomTestFixture::m_Rng

  /// @name Random value generators
  ///@{

  bool GenerateBool( double i_Probability = 0.5 );  ///< Generates a boolean

  template< Herd::Concepts::Number T > // @suppress("Type cannot be resolved")
  T GenerateNumber( T i_Min, T i_Max ); // @suppress("Type cannot be resolved")

  template< Herd::Concepts::Number T > // @suppress("Type cannot be resolved")
  T GenerateNumber(); // @suppress("Type cannot be resolved")
  ///@}
  
  /// @name Random physical quantity generators
  ///@{
  double GenerateMetallicity(); ///< Generates a metallicity value
  double GenerateMass(); ///< Generates a mass value
  double GenerateLuminosity();  ///< Generates a luminosity value
  double GenerateRadius(); ///< Generates a radius value
  /// @}


private:

  void TrySetSeedFromCommandLine(); ///< Attempts to read a seed value from the command line parameters
  void InitialiseLazy(); ///< Performs lazy initialisation of RandomTestFixture::m_Rng

  uint_fast32_t m_Seed = 0; ///< Seed used for initialising the random number engine
                            // mt19937 seed type is uint_fast32_t
  std::optional< std::mt19937 > m_Rng; ///< Random number generator

  inline static const std::string s_SeedParameterName = "--seed";  ///< Command line parameter name
};

// Extern template declarations
///@cond
// Suppress incorrect doxygen warnings
extern template double RandomTestFixture::GenerateNumber( double, double ); // @suppress("Member declaration not found")
extern template double RandomTestFixture::GenerateNumber(); // @suppress("Member declaration not found")
///@endcond

/**
 * @brief Generates a number
 * @tparam T Satisfies Herd::Concepts::Number
 * @param i_Min Minimum value
 * @param i_Max Maximum value
 * @return A random value
 * @pre <tt> i_Min < i_Max </tt>
 * @remarks For real distributions if <tt> i_Max - i_Min </tt> is bigger than the maximum for \c T the bounds are halved and the output is doubled
 * This allows sampling over the entire range of a floating point type
 */
template< Herd::Concepts::Number T > // @suppress("Type cannot be resolved")
T RandomTestFixture::GenerateNumber( T i_Min, T i_Max ) // @suppress("Type cannot be resolved")
{
  BOOST_TEST_REQUIRE( i_Min < i_Max ); // @suppress("Invalid arguments") // @suppress("Method cannot be resolved")

  InitialiseLazy();

  if constexpr ( std::is_floating_point< T >::value )
  {
    if( i_Max - i_Min == std::numeric_limits< T >::infinity() )
    {
      i_Min /= 2;
      i_Max /= 2;
      return 2 * std::uniform_real_distribution< T >( i_Min, std::nextafter( i_Max, std::numeric_limits< T >::max() ) )( *m_Rng ); // Closed range
    } else
    {
      return std::uniform_real_distribution< T >( i_Min, std::nextafter( i_Max, std::numeric_limits< T >::max() ) )( *m_Rng ); // Closed range
    }
  } else
  {
    return std::uniform_int_distribution< T >( i_Min, i_Max )( *m_Rng ); // Closed range
  }
}

/**
 * @brief Generates a number in the closed range
 * @tparam T Satisfies Herd::Concepts::Number
 * @return A random value
 */
template< Herd::Concepts::Number T > // @suppress("Type cannot be resolved")
T RandomTestFixture::GenerateNumber() // @suppress("Type cannot be resolved")
{
  if constexpr ( std::is_floating_point< T >::value )
  {
    return GenerateNumber( std::numeric_limits< T >::lowest(), std::nextafter( std::numeric_limits< T >::max(), std::numeric_limits< T >::lowest() ) ); // This ensures that the span of the range is still representable as an Arithmetic
  } else
  {
    return GenerateNumber( std::numeric_limits< T >::lowest(), std::numeric_limits< T >::max() ); // It seems like internally integers are promoted to floating point numbers if the span > the maximum value of Arithmetic                                                                                                                  // uniform_int_distrubtion works over a closed range, unlike uniform_real_distribution
  }
}
} // namespace Herd::UnitTestUtils

#endif /* H219F636B_7938_44D6_981D_D0D7FA5DAC8C */
