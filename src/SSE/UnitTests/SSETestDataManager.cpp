/**
 * @file SSEUnitTestHelper.cpp
 * @author Evren Imre
 * @date 26 Jun 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <cmath>
#include <iterator>

#include <boost/test/unit_test.hpp>
#include <range/v3/algorithm.hpp>
#include <range/v3/view.hpp>

#include "SSETestDataManager.h"

namespace Herd::SSE::UnitTests
{



/**
 * @param i_rParentToTrackPoints A node with track point nodes as its children
 */
void SSETestDataManager::SetData( const boost::property_tree::ptree& i_rParentToTrackPoints )
{
  m_Parent = i_rParentToTrackPoints;
  m_TrackPoints.clear();

  m_TrackPointCount = ranges::cpp20::count_if( m_Parent, [ & ]( const auto& i_rNode )
  { return i_rNode.first == s_TrackPointTag;} );
  m_StringAndStage = Herd::SSE::MakeEvolutionStageAndStringBimap();
}

/**
 * @param i_rUUIDExpected Expected UUID
 * @param i_rVersionExpected Expected version
 */
void SSETestDataManager::CheckVersionInfo( const std::string& i_rUUIDExpected, const std::string& i_rVersionExpected ) const
{
  std::string i_rUUIDActual;
  std::string i_rVersionActual;
  try
  {
    const boost::property_tree::ptree& rAttributes = m_Parent.get_child( "<xmlattr>" );
    i_rUUIDActual = rAttributes.get< std::string >( "UUID" );
    i_rVersionActual = rAttributes.get< std::string >( "Version" );
  } catch( ... )
  {
    BOOST_TEST_REQUIRE( false, "Invalid or missing versioning attribute" );
  }

  BOOST_TEST_REQUIRE( i_rUUIDActual == i_rUUIDExpected );
  BOOST_TEST_REQUIRE( i_rVersionActual == i_rVersionExpected );
}

void SSETestDataManager::CheckVersionInfo() const
{
  CheckVersionInfo( Herd::SSE::UnitTests::DefaultVersionInfo::s_UUID, Herd::SSE::UnitTests::DefaultVersionInfo::s_Version );
}

/**
 * @return Returns a constant reference to SSETestDataManager::m_TrackPoints
 */
const std::vector< Herd::SSE::Star >& SSETestDataManager::TrackPoints() const
{
  return m_TrackPoints;
}

Herd::SSE::Star SSETestDataManager::MakeTrackPoint( std::size_t i_Index, const std::optional< Herd::Generic::Metallicity >& i_rZo ) const
{
  BOOST_TEST_REQUIRE( i_Index < m_TrackPointCount );

  auto trackPointsView = m_Parent | ranges::view::remove_if( [ & ]( const auto& i_rNode )
  { return i_rNode.first != s_TrackPointTag;} );
  const boost::property_tree::ptree& rNode = std::next( ranges::cpp20::begin( trackPointsView ), i_Index )->second; // @suppress("Invalid arguments") // @suppress("Field cannot be resolved")
  return MakeTrackPoint( rNode, i_rZo );
}


/**
 * @param i_rZo Initial value of the metallicity. If unset, read from the Z attribute
 */
void SSETestDataManager::PopulateTrackPoints( const std::optional< Herd::Generic::Metallicity >& i_rZo )
{
  // If there are any non-track point nodes, MakeTrackPoint will fail
  m_TrackPoints.reserve( m_TrackPointCount );

  auto trackPointsView = m_Parent | ranges::view::remove_if( [ & ]( const auto& i_rNode )
  { return i_rNode.first != s_TrackPointTag;} );

  ranges::cpp20::transform( trackPointsView, std::back_inserter( m_TrackPoints ), [ & ]( const auto& i_rNode )
  { return MakeTrackPoint(i_rNode.second, i_rZo);} );
}

/**
 * @return Returns SSETestDataManager::m_ChildrenCount
 */
std::size_t SSETestDataManager::TrackPointCount() const
{
  return m_TrackPointCount;
}

/**
 * @param i_rNode Node for the track point
 * @param i_rZo Initial metallicity. If unset, read from the node
 * @return Track point for the node
 */
Herd::SSE::Star SSETestDataManager::MakeTrackPoint( const boost::property_tree::ptree& i_rNode,
    const std::optional< Herd::Generic::Metallicity >& i_rZo ) const
{
  Herd::SSE::Star trackPoint;

  boost::property_tree::ptree rAttributes = i_rNode.get_child( "<xmlattr>" );

  try
  {
    trackPoint.m_Age.Set( rAttributes.get< double >( "Age" ) );
    trackPoint.m_Mass.Set( rAttributes.get< double >( "M" ) );
    trackPoint.m_CoreMass.Set( rAttributes.get< double >( "Mc" ) );
    trackPoint.m_EnvelopeMass.Set( rAttributes.get< double >( "Menv" ) );
    trackPoint.m_Luminosity.Set( std::pow( 10., rAttributes.get< double >( "logL" ) ) );
    trackPoint.m_Radius.Set( std::pow( 10., rAttributes.get< double >( "logR" ) ) );
    trackPoint.m_Temperature.Set( std::pow( 10., rAttributes.get< double >( "logT" ) ) );
    trackPoint.m_AngularMomentum.Set( rAttributes.get< double >( "Spin" ) );

    if( !i_rZo )
    {
      trackPoint.m_InitialMetallicity.Set( rAttributes.get< double >( "Z" ) );
    } else
    {
      // Z ignored as metallicity evolution is not computed
      trackPoint.m_InitialMetallicity.Set( *i_rZo );
    }

    trackPoint.m_Stage = m_StringAndStage.right.at( rAttributes.get< std::string >( "Stage" ) );

  } catch( ... )
  {
    BOOST_TEST_REQUIRE( false, "Invalid or missing track point attribute" );
  }

  return trackPoint;
}

}


