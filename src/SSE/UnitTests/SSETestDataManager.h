/**
 * @file SSEUnitTestHelper.h
 * @author Evren Imre
 * @date 26 Jun 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef H4AB1AC39_1519_4C63_9784_D33A5B105D68
#define H4AB1AC39_1519_4C63_9784_D33A5B105D68

#include <Generic/Quantities.h>
#include <SSE/EvolutionStages.h>
#include <SSE/Star.h>

#include <cstddef>
#include <optional>
#include <string>
#include <vector>

#include <boost/bimap.hpp>
#include <boost/property_tree/ptree.hpp>

namespace Herd::SSE::UnitTests
{
/**
 * @brief Default versioning information for the unit test data
 * @warning If the unit test data is regenerated, \c s_UUID must be updated
 * @remarks Version number changes if the structure of the XML files change. In this case, \c s_Version must be updated
 */
namespace DefaultVersionInfo
{
inline static const std::string s_Version = "1.0";
inline static const std::string s_UUID = "20fde9dc-0601-41cb-b5de-b3320e2ac0aa";
}

/**
 * @brief Data manager for SSE unit tests
 * @remarks Unit test data is presented as a property tree, with the children of its top-level node as track points
 * @remarks In order to get the initial values in a track, get the track point at ZAMS. Typically the first element in a track.
 */
class SSETestDataManager
{
public:

  void SetData( const boost::property_tree::ptree& i_rParentToTrackPoints );  ///< Sets the test data

  void CheckVersionInfo( const std::string& i_rUUIDExpected, const std::string& i_rVersionExpected ) const; ///< Checks the dataset version
  void CheckVersionInfo() const; ///< Checks the dataset version against the default values

  void PopulateTrackPoints( const std::optional< Herd::Generic::Metallicity >& i_rZo = std::nullopt ); ///< Populates SSETestDataManager::m_TrackPoints

  const std::vector< Herd::SSE::Star >& TrackPoints() const;  ///< Accessor for SSETestDataManager::m_TrackPoints
  std::size_t TrackPointCount() const; ///< Accessor for SSETestDataManager::m_TrackPointCount

  Herd::SSE::Star MakeTrackPoint( std::size_t i_Index, const std::optional< Herd::Generic::Metallicity >& i_rZo = std::nullopt ) const; ///< Track point corresponding to the node at index

private:

  Herd::SSE::Star MakeTrackPoint( const boost::property_tree::ptree& i_rNode, const std::optional< Herd::Generic::Metallicity >& i_rZo = std::nullopt ) const; ///< Makes a track point from a node

  inline static const std::string s_TrackPointTag = "TrackPoint"; ///< XML tag for track points

  boost::property_tree::ptree m_Parent; ///< Parent of the track points
  std::size_t m_TrackPointCount = 0; ///< Number of track points for SSETestDataManager::m_Parent
  std::vector< Herd::SSE::Star > m_TrackPoints; ///< Track points in the tree

  boost::bimap< Herd::SSE::EvolutionStage, std::string > m_StringAndStage;  ///< Evolution stages and their corresponding strings
};

}



#endif /* H4AB1AC39_1519_4C63_9784_D33A5B105D68 */
