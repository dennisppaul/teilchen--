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

#include "IConstraint.h"
#include "PVector.h"
#include "Physics.h"
#include <cmath>

class Teleporter final : public IConstraint {
protected:
    bool       mActive;
    bool       mDead;
    const long mID;
    PVector    mMax;
    PVector    mMin;

public:
    Teleporter() : Teleporter(PVector(), PVector()) {}

    Teleporter(const PVector& pMin, const PVector& pMax)
        : mActive(true),
          mDead(false),
          mID(Physics::getUniqueID()),
          mMax(PVector(pMax.x, pMax.y, pMax.z)),
          mMin(PVector(pMin.x, pMin.y, pMin.z)) {}

    PVector& max() {
        return mMax;
    }

    PVector& min() {
        return mMin;
    }

    void apply(Physics& pParticleSystem) override {
        if (!mActive) {
            return;
        }

        const auto& particles = pParticleSystem.particles();
        for (const auto& mParticle: particles) {
            if (mParticle->position().x > mMax.x) {
                mParticle->position().x -= std::abs(mMax.x - mMin.x);
            }
            if (mParticle->position().y > mMax.y) {
                mParticle->position().y -= std::abs(mMax.y - mMin.y);
            }
            if (mParticle->position().z > mMax.z) {
                mParticle->position().z -= std::abs(mMax.z - mMin.z);
            }
            if (mParticle->position().x < mMin.x) {
                mParticle->position().x += std::abs(mMax.x - mMin.x);
            }
            if (mParticle->position().y < mMin.y) {
                mParticle->position().y += std::abs(mMax.y - mMin.y);
            }
            if (mParticle->position().z < mMin.z) {
                mParticle->position().z += std::abs(mMax.z - mMin.z);
            }
        }
    }

    bool active() const override {
        return mActive;
    }

    void active(bool pActiveState) override {
        mActive = pActiveState;
    }

    bool dead() const override {
        return mDead;
    }

    void dead(bool pDead) override {
        mDead = pDead;
    }

    long ID() const override {
        return mID;
    }

    static Teleporter* make() {
        return new Teleporter();
    }
};