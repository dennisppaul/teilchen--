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

#include <vector>

#include "IForce.h"
#include "PVector.h"
#include "Physics.h"
#include "Particle.h"

class Gravity final : public IForce {

    bool       mActive;
    bool       mDead;
    PVector    mForce;
    const long mID;

public:
    Gravity() : Gravity(PVector(0, 9.81f, 0)) {}

    explicit Gravity(const PVector& pForce)
        : mActive(true), mDead(false), mForce(pForce), mID(Physics::getUniqueID()) {}

    Gravity(const float pForceX, const float pForceY, const float pForceZ)
        : Gravity(PVector(pForceX, pForceY, pForceZ)) {}

    PVector& force()  {
        return mForce;
    }

    void apply(float pDeltaTime, Physics& pParticleSystem) override {
        const auto& particles = pParticleSystem.particles();
        for (const auto& mParticle: particles) {
            if (!mParticle->fixed()) {
                mParticle->force().add(mForce);
            }
        }
    }

    bool dead() const override {
        return mDead;
    }

    void dead(const bool pDead) override {
        mDead = pDead;
    }

    bool active() const override {
        return mActive;
    }

    void active(const bool pActiveState) override {
        mActive = pActiveState;
    }

    long ID() const override {
        return mID;
    }

    static Gravity* make() {
        return new Gravity();
    }

    static Gravity* make(const PVector& pForce) {
        return new Gravity(pForce);
    }

    static Gravity* make(const float pForceX, const float pForceY, const float pForceZ) {
        return new Gravity(pForceX, pForceY, pForceZ);
    }
};

using GravityPtr = Gravity*;