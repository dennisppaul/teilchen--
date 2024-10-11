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
#include "IParticle.h"

class Attractor final : public IForce {
protected:
    PVector    mPosition;
    float      mRadius;
    float      mStrength;
    bool       mActive;
    bool       mDead;
    const long mID;

public:
    Attractor()
        : mPosition(0, 0, 0),
          mRadius(100.0f),
          mStrength(1.0f),
          mActive(true),
          mDead(false),
          mID(Physics::getUniqueID()) {}

    static float fastInverseSqrt(float v) {
        const float half = 0.5f * v;
        int         i    = *reinterpret_cast<int*>(&v);
        i                = 0x5f375a86 - (i >> 1);
        v                = *reinterpret_cast<float*>(&i);
        return v * (1.5f - half * v * v);
    }

    PVector& position() {
        return mPosition;
    }

    void setPositionRef(const PVector& pPosition) {
        mPosition = pPosition;
    }

    float strength() const {
        return mStrength;
    }

    void strength(const float pStrength) {
        mStrength = pStrength;
    }

    float radius() const {
        return mRadius;
    }

    void radius(const float pRadius) {
        mRadius = pRadius;
    }

    void apply(float pDeltaTime, Physics& pParticleSystem) override {
        if (mStrength != 0) {
            const auto& particles = pParticleSystem.particles();
            for (const auto& mParticle: particles) {
                if (!mParticle->fixed()) {
                    PVector     mTemp     = PVector::sub(mPosition, mParticle->position());
                    const float mDistance = fastInverseSqrt(1.0f / mTemp.magSq());
                    if (mDistance < mRadius) {
                        const float mFallOff = 1.0f - mDistance / mRadius;
                        const float mForce   = mFallOff * mFallOff * mStrength;
                        mTemp.mult(mForce / mDistance);
                        mParticle->force().add(mTemp);
                    }
                }
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

    static std::shared_ptr<Attractor> make() {
        return std::make_shared<Attractor>();
    }
};

using AttractorPtr = std::shared_ptr<Attractor>;
