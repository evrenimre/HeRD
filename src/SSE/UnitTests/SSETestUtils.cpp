/**
 * @file SSETestUtils.cpp
 * @author Evren Imre
 * @date 25 Jul 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "SSETestUtils.h"

#include <SSE/EvolutionStage.h>

namespace Herd::SSE::UnitTests
{
/**
 * @param io_rRng Random number generator
 * @return A random track point
 * @remarks The track point satisfies algebraic preconditions, but does not necessarily a physically viable star
 * @remarks https://en.wikipedia.org/wiki/List_of_star_extremes
 */
Herd::SSE::TrackPoint GenerateRandomTrackPoint( std::mt19937& io_rRng )
{
  Herd::SSE::TrackPoint generated;

  auto Generator = [ & ]( double i_Min, double i_Max )
  {
    std::uniform_real_distribution< double > distribution(i_Min, i_Max );
    return distribution(io_rRng);
  };

  generated.m_Mass.Set( Generator( 0.08, 216. ) );
  generated.m_InitialMetallicity.Set( Generator( 0., 0.1 ) );
  generated.m_Radius.Set( Generator( 0.084, 2150. ) );
  generated.m_Luminosity.Set( Generator( 0.000126, 6166000 ) );
  generated.m_Temperature.Set( Generator( 1800., 210000. ) );
  generated.m_Age.Set( Generator( 0., 13600. ) );
  generated.m_CoreMass.Set( Generator( 0., generated.m_Mass.Value() ) );
  generated.m_EnvelopeMass.Set( Generator( 0., generated.m_Mass.Value() - generated.m_CoreMass.Value() ) );
  generated.m_AngularMomentum.Set( Generator( 0., 715. ) );

  {
    static auto StageList = Herd::SSE::EnumerateEvolutionStages();
    std::uniform_int_distribution< std::size_t > distribution( 0, StageList.size() - 1 ); // Closed range
    while( generated.m_Stage == Herd::SSE::EvolutionStage::e_Undefined )
    {
      generated.m_Stage = StageList[ distribution( io_rRng ) ];
    }

  }

  return generated;
}

/**
 * @param io_rRng Random number generator
 * @return A random evolution state
 * @remarks The output satisfies algebraic preconditions, but does not necessarily a physically viable star
 */
Herd::SSE::EvolutionState GenerateRandomEvolutionState( std::mt19937& io_rRng )
{
  Herd::SSE::EvolutionState generated;

  generated.m_TrackPoint = GenerateRandomTrackPoint( io_rRng );

  auto Generator = [ & ]( double i_Min, double i_Max )
  {
    std::uniform_real_distribution< double > distribution(i_Min, i_Max );
    return distribution(io_rRng);
  };

  generated.m_InitialMass.Set( Generator( 0.08, 216. ) ); // A star in a binary system can gain mass
  generated.m_CoreRadius.Set( Generator( 0., generated.m_TrackPoint.m_Radius.Value() ) );
  generated.m_MassLossRate = Generator( 0., 1e-4 );
  generated.m_AngularMomentumLossRate = Generator( 0., 715. );

  return generated;
}

}

