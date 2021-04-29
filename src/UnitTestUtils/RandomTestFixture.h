/**
 * @file RandomTestFixture.h
 * @author Evren Imre
 * @date 18 Apr 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef H219F636B_7938_44D6_981D_D0D7FA5DAC8C
#define H219F636B_7938_44D6_981D_D0D7FA5DAC8C

#include <cstdint>
#include <limits>
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
  template< class Real >
  Real GenerateReal( Real i_Min, Real i_Max );  ///< Generates a real value

  template< class Real >
  Real GenerateReal();  ///< Generates a real value

  template< class Int >
  Int GenerateInt( Int i_Min, Int i_Max );  ///< Generates an integer

  template< class Int >
  Int GenerateInt();  ///< Generates an integer

  bool GenerateBool();  ///< Generates a boolean
  ///@}

private:

  void TrySetSeedFromCommandLine(); ///< Attempts to read a seed value from the command line parameters

  uint_fast32_t m_Seed = 0; ///< Seed used for initialising the random number engine
                            // mt19937 seed type is uint_fast32_t
  std::mt19937 m_Rng; ///< Random number generator

  inline static const std::string s_SeedParameterName = "--random_seed";  ///< Command line parameter name
};
} // namespace Herd::UnitTestUtils

/**
 * @tparam Real Value type
 * @param i_Min Minimum value
 * @param i_Max Maximum value. Right-open range
 * @return A random real value within the specified range
 * @pre \c Real is a floating point type
 */
template< class Real >
Real Herd::UnitTestUtils::RandomTestFixture::GenerateReal( Real i_Min, Real i_Max )
{
  static_assert( std::is_floating_point< Real >::value );
  return std::uniform_real_distribution< Real >( i_Min, i_Max )( m_Rng );
}

/**
 * @tparam Real Value type
 * @return A random real value
 * @pre \c Real is a floating point type
 */
template< class Real >
Real Herd::UnitTestUtils::RandomTestFixture::GenerateReal()
{
  static_assert( std::is_floating_point< Real >::value );
  return GenerateReal( std::numeric_limits< Real >::lowest(), std::numeric_limits< Real >::max() );
}

/**
 * @tparam Int Value type
 * @param i_Min Minimum value
 * @param i_Max Maximum value. Closed range
 * @return A random integer within the specified range
 * @pre \c Int is an integer type
 */
template< class Int >
Int Herd::UnitTestUtils::RandomTestFixture::GenerateInt( Int i_Min, Int i_Max )
{
  static_assert( std::is_integral< Int >::value );
  return std::uniform_int_distribution< Int >( i_Min, i_Max )( m_Rng );
}

/**
 * @tparam Int Value type
 * @return A random integer
 * @pre \c Int is an integer type
 */
template< class Int >
Int Herd::UnitTestUtils::RandomTestFixture::GenerateInt()
{
  static_assert( std::is_integral< Int >::value );
  return GenerateInt( std::numeric_limits< Int >::lowest(), std::numeric_limits< Int >::max() );
}

#endif /* H219F636B_7938_44D6_981D_D0D7FA5DAC8C */
