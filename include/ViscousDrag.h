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

#include "IForce.h"
#include "Physics.h"
#include "IParticle.h"
#include "Verlet.h"

class ViscousDrag : public IForce {
public:
    float      coefficient;
    bool       mActive;
    bool       mDead;
    const long mID;

    explicit ViscousDrag(float pCoefficient)
        : coefficient(pCoefficient), mActive(true), mDead(false), mID(Physics::getUniqueID()) {}

    ViscousDrag() : ViscousDrag(1.0f) {}

    void apply(float pDeltaTime, Physics& pParticleSystem) override {
        if (dynamic_cast<Verlet*>(pParticleSystem.getIntegrator()) != nullptr) {
            return;
        }

        if (coefficient != 0) {
            const auto& particles = pParticleSystem.particles();
            for (const auto& mParticle: particles) {
                if (!mParticle->fixed()) {
                    mParticle->force().add(
                        mParticle->velocity().x * -coefficient,
                        mParticle->velocity().y * -coefficient,
                        mParticle->velocity().z * -coefficient);
                }
            }
        }
    }

    bool dead() const override {
        return mDead;
    }

    void dead(bool pDead) override {
        mDead = pDead;
    }

    bool active() const override {
        return mActive;
    }

    void active(bool pActiveState) override {
        mActive = pActiveState;
    }

    long ID() const override {
        return mID;
    }

    static ViscousDrag* make() {
        return new ViscousDrag();
    }

    static ViscousDrag* make(float pCoefficient) {
        return new ViscousDrag(pCoefficient);
    }
};
