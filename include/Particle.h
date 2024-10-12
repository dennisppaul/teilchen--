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

class Physics;

#include "Particle.h"
#include "PVector.h"

using namespace umgebung;

class Particle final : public IParticle {
    float      mAge;
    bool       mDead;
    bool       mFixed;
    PVector    mForce;
    const long mID;
    float      mMass;
    PVector    mOldPosition;
    PVector    mPosition;
    float      mRadius;
    bool       mStill;
    bool       mTagged;
    PVector    mVelocity;

public:
    Particle()
        : mAge(0),
          mDead(false),
          mFixed(false),
          mForce(0, 0, 0),
          mID(0),
          // mID(Physics::getUniqueID()),
          mMass(1.0f),
          mOldPosition(0, 0, 0),
          mPosition(0, 0, 0),
          mRadius(0.0f),
          mStill(false),
          mTagged(false),
          mVelocity(0, 0, 0) {}

    bool     fixed() const override { return mFixed; }
    void     fixed(const bool pFixed) override { mFixed = pFixed; }
    float    age() const override { return mAge; }
    void     age(const float pAge) override { mAge = pAge; }
    float    mass() const override { return mMass; }
    void     mass(const float pMass) override { mMass = pMass; }
    PVector& old_position() override { return mOldPosition; }
    PVector& position() override { return mPosition; }
    void     setPositionRef(const PVector& pPosition) override { mPosition = pPosition; }
    PVector& velocity() override { return mVelocity; }
    PVector& force() override { return mForce; }
    bool     dead() const override { return mDead; }
    void     dead(const bool pDead) override { mDead = pDead; }
    bool     tagged() const override { return mTagged; }
    void     tag(const bool pTag) override { mTagged = pTag; }
    float    radius() const override { return mRadius; }
    void     radius(const float pRadius) override { mRadius = pRadius; }
    bool     still() const override { return mStill; }
    void     still(const bool pStill) override { mStill = pStill; }
    long     ID() const override { return mID; }
    void     accumulateInnerForce(float pDeltaTime) override {}

    static Particle* make() {
        return new Particle();
    }
};
