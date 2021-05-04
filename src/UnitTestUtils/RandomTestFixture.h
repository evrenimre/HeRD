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

#include <cstdint>
#include <limits>
#include <optional>
#include <random>
#include <string>
#include <type_traits>

namespace Herd::UnitTestUtils
{

/**
 * @brief A test fixture for unit tests requiring random values
 * @remarks Seeded with current time
 */
class RandomTestFixture
{
public:

  RandomTestFixture();  ///< Constructor
  void SetSeed( unsigned int i_Seed ); ///< Reinitialises the random number generator with seed

  /// @name Random value generators
  ///@{

  bool GenerateBool( double i_Probability = 0.5 );  ///< Generates a boolean

  template< class Arithmetic >
  Arithmetic GenerateNumber( Arithmetic i_Min, Arithmetic i_Max );  ///< Generates a number

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
 */
template< class Arithmetic >
Arithmetic RandomTestFixture::GenerateNumber( Arithmetic i_Min, Arithmetic i_Max )
{
  static_assert( std::is_arithmetic< Arithmetic >::value);

  // Lazy initialisation. Avoids redundant construction if user sets a different seed before first call to the generator
  // mt19937 constructor is expensive.
  if( !m_Rng )
  {
    m_Rng = std::mt19937( m_Seed );
  }

  if constexpr ( std::is_floating_point< Arithmetic >::value )
  {
    return std::uniform_real_distribution< Arithmetic >( i_Min, i_Max )( *m_Rng );
  } else
  {
    return std::uniform_int_distribution< Arithmetic >( i_Min, i_Max )( *m_Rng );
  }
}

/**
 * @tparam Arithmetic An arithmetic type
 * @return A random value
 */
template< class Arithmetic >
Arithmetic RandomTestFixture::GenerateNumber()
{
  return GenerateNumber( std::numeric_limits< Arithmetic >::lowest(), std::numeric_limits< Arithmetic >::max() );
}
} // namespace Herd::UnitTestUtils

#endif /* H219F636B_7938_44D6_981D_D0D7FA5DAC8C */
