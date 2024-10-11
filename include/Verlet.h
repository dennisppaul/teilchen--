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

#include "IIntegrator.h"
#include "PVector.h"
#include "IParticle.h"
#include "Physics.h"

class Verlet final : public IIntegrator {
    float   mDamping;
    PVector temp1;
    PVector temp2;

public:
    Verlet() : Verlet(1.0f) {}

    explicit Verlet(float pDamping) : mDamping(pDamping), temp1(), temp2() {}

    float damping() const {
        return mDamping;
    }

    void damping(float pDamping) {
        mDamping = pDamping;
    }

    void step(float pDeltaTime, Physics& pParticleSystem) override {
        pParticleSystem.applyForces(pDeltaTime);
        const auto& particles = pParticleSystem.particles();
        for (const auto& mParticle: particles) {
            if (!mParticle->fixed()) {
                integrate(pDeltaTime, *mParticle);
            }
        }
    }

private:
    void integrate(float pDeltaTime, IParticle& pParticle) {
        const PVector mOldPosition = pParticle.position();

        pParticle.velocity().set(PVector::sub(pParticle.position(), pParticle.old_position()));
        pParticle.velocity().mult(1.0f / pDeltaTime);

        temp1.set(pParticle.force());
        temp1.mult(1.0f / pParticle.mass());
        temp1.mult(pDeltaTime * pDeltaTime);

        temp2.set(PVector::sub(pParticle.position(), pParticle.old_position()));
        temp2.mult(mDamping);

        pParticle.position().add(temp1);
        pParticle.position().add(temp2);

        pParticle.old_position().set(mOldPosition);
    }
};
