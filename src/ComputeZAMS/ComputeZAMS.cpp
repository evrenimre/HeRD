/**
 * @file ComputeZAMS.cpp
 * @author Evren Imre
 * @date 6 May 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <Generic/Quantities.h>
#include <SSE/EvolutionState.h>
#include <SSE/ZeroAgeMainSequence.h>

#include <iostream>

#include <boost/program_options.hpp>

/**
 * @brief Computes the ZAMS luminosity, radius and temperature
 * @param argc Argument count
 * @param argv Arguments
 * @return Status code
 */
int main( int argc, char* argv[] )
{
  boost::program_options::options_description description( "Arguments" );

  // @formatter:off
  description.add_options()( "help,", "Display this message" )
      ( "mass,", boost::program_options::value< double >(), "Mass in M_Sun" )
      ( "metallicity", boost::program_options::value< double >(), "Metallicity" );
    // @formatter:on

  boost::program_options::variables_map argument_map;
  boost::program_options::store( boost::program_options::command_line_parser( argc, argv ).options( description ).run(), argument_map );
  boost::program_options::notify( argument_map );

  if( argument_map.count( "help" ) == 1 )
  {
    std::cout << description;
  }

  if( argument_map.count( "mass" ) == 1 && argument_map.count( "metallicity" ) == 1 )
  {
    Herd::Generic::Mass mass( argument_map[ "mass" ].as< double >() );
    Herd::Generic::Metallicity metallicity( argument_map[ "metallicity" ].as< double >() );

    Herd::SSE::EvolutionState zamsState = Herd::SSE::ZeroAgeMainSequence::Compute( mass, metallicity );
    std::cout << " Luminosity " << zamsState.m_Luminosity.Value() << " L_Sun \n";
    std::cout << " Radius " << zamsState.m_Radius.Value() << " R_Sun \n";
    std::cout << " Temperature " << zamsState.m_Temperature.Value() << " K \n";
  }

  return 0;
}



