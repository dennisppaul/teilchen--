/*
 * Teilchen++
 *
 * This file is part of the *teilchen* library (https://github.com/dennisppaul/teilchen).
 * Copyright (c) 2024 Dennis P Paul.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * {@link http://www.gnu.org/licenses/lgpl.html}
 *
 */

#pragma once

#include "SpatialEntity.h"
#include "PVector.h"

using namespace umgebung;

class Particle : public SpatialEntity {
public:
    virtual bool     fixed() const                            = 0;
    virtual void     fixed(bool pFixed)                       = 0;
    virtual float    age() const                              = 0;
    virtual void     age(float pAge)                          = 0;
    virtual float    mass() const                             = 0;
    virtual void     mass(float pMass)                        = 0;
    virtual PVector& old_position()                           = 0;
    virtual void     setPositionRef(const PVector& pPosition) = 0;
    virtual PVector& velocity()                               = 0;
    virtual PVector& force()                                  = 0;
    virtual bool     dead() const                             = 0;
    virtual void     dead(bool pDead)                         = 0;
    virtual bool     tagged() const                           = 0;
    virtual void     tag(bool pTag)                           = 0;
    virtual void     accumulateInnerForce(float pDeltaTime)   = 0;
    float            radius() const override                  = 0;
    virtual void     radius(float pRadius)                    = 0;
    PVector&         position() override                      = 0;
    virtual bool     still() const                            = 0;
    virtual void     still(bool pStill)                       = 0;
    virtual long     ID() const                               = 0;
};
