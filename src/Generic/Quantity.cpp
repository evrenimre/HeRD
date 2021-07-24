/**
 * @file Quantity.cpp
 * @author Evren Imre
 * @date 12 Jul 2021	
 */
/* This file is a part of HeRD, a stellar evolution library
 * Copyright © 2021 Evren Imre 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "Quantity.h"

namespace Herd::Generic
{
// Explicit instantiations
template class Quantity< struct LuminosityTag > ;
template class Quantity< struct MassTag > ;
template class Quantity< struct MetallicityTag > ;
template class Quantity< struct RadiusTag > ;
template class Quantity< struct TemperatureTag > ;
template class Quantity< struct AgeTag > ;
template class Quantity< struct AngularMomentumTag > ;
template class Quantity< struct AngularVelocityTag > ;
}


