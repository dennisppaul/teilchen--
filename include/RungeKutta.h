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
#include "PVector.h" // Assuming you have a PVector class similar to the Java version
#include "Particle.h"
#include "Physics.h"
#include "IntegrationUtil.h"

class RungeKutta : public Integrator {
    std::vector<PVector> mK1Forces;
    std::vector<PVector> mK1Velocities;
    std::vector<PVector> mK2Forces;
    std::vector<PVector> mK2Velocities;
    std::vector<PVector> mK3Forces;
    std::vector<PVector> mK3Velocities;
    std::vector<PVector> mK4Forces;
    std::vector<PVector> mK4Velocities;
    std::vector<PVector> mOriginalPositions;
    std::vector<PVector> mOriginalVelocities;

public:
    RungeKutta() = default;

    void step(const float pDeltaTime, Physics& pParticleSystem) {
        const int mSize = static_cast<int>(pParticleSystem.particles().size());
        IntegrationUtil::checkContainerSize(mSize, mOriginalPositions);
        IntegrationUtil::checkContainerSize(mSize, mOriginalVelocities);
        IntegrationUtil::checkContainerSize(mSize, mK1Forces);
        IntegrationUtil::checkContainerSize(mSize, mK1Velocities);
        IntegrationUtil::checkContainerSize(mSize, mK2Forces);
        IntegrationUtil::checkContainerSize(mSize, mK2Velocities);
        IntegrationUtil::checkContainerSize(mSize, mK3Forces);
        IntegrationUtil::checkContainerSize(mSize, mK3Velocities);
        IntegrationUtil::checkContainerSize(mSize, mK4Forces);
        IntegrationUtil::checkContainerSize(mSize, mK4Velocities);

        // Save original positions and velocities
        for (int i = 0; i < pParticleSystem.particles().size(); ++i) {
            Particle* mParticle = pParticleSystem.particles()[i];
            if (!mParticle->fixed()) {
                mOriginalPositions[i].set(mParticle->position());
                mOriginalVelocities[i].set(mParticle->velocity());
            }
        }

        // Get all k1 values
        pParticleSystem.applyForces(pDeltaTime);

        // Save intermediate forces (k1)
        for (int i = 0; i < pParticleSystem.particles().size(); ++i) {
            Particle* mParticle = pParticleSystem.particles()[i];
            if (!mParticle->fixed()) {
                mK1Forces[i].set(mParticle->force());
                mK1Velocities[i].set(mParticle->velocity());
            }
        }

        // Get k2 values
        for (int i = 0; i < pParticleSystem.particles().size(); ++i) {
            Particle* mParticle = pParticleSystem.particles()[i];
            if (!mParticle->fixed()) {
                const PVector& originalPosition = mOriginalPositions[i];
                const PVector& k1Velocity       = mK1Velocities[i];

                mParticle->position().x = originalPosition.x + k1Velocity.x * 0.5f * pDeltaTime;
                mParticle->position().y = originalPosition.y + k1Velocity.y * 0.5f * pDeltaTime;
                mParticle->position().z = originalPosition.z + k1Velocity.z * 0.5f * pDeltaTime;

                const PVector& originalVelocity = mOriginalVelocities[i];
                const PVector& k1Force          = mK1Forces[i];

                mParticle->velocity().x = originalVelocity.x + k1Force.x * 0.5f * pDeltaTime / mParticle->mass();
                mParticle->velocity().y = originalVelocity.y + k1Force.y * 0.5f * pDeltaTime / mParticle->mass();
                mParticle->velocity().z = originalVelocity.z + k1Force.z * 0.5f * pDeltaTime / mParticle->mass();
            }
        }

        pParticleSystem.applyForces(pDeltaTime);

        // Save intermediate forces (k2)
        for (int i = 0; i < pParticleSystem.particles().size(); ++i) {
            Particle* mParticle = pParticleSystem.particles()[i];
            if (!mParticle->fixed()) {
                mK2Forces[i].set(mParticle->force());
                mK2Velocities[i].set(mParticle->velocity());
            }
        }

        // Get k3 values
        for (int i = 0; i < pParticleSystem.particles().size(); ++i) {
            Particle* mParticle = pParticleSystem.particles()[i];
            if (!mParticle->fixed()) {
                const PVector& originalPosition = mOriginalPositions[i];
                const PVector& k2Velocity       = mK2Velocities[i];

                mParticle->position().x = originalPosition.x + k2Velocity.x * 0.5f * pDeltaTime;
                mParticle->position().y = originalPosition.y + k2Velocity.y * 0.5f * pDeltaTime;
                mParticle->position().z = originalPosition.z + k2Velocity.z * 0.5f * pDeltaTime;

                const PVector& originalVelocity = mOriginalVelocities[i];
                const PVector& k2Force          = mK2Forces[i];

                mParticle->velocity().x = originalVelocity.x + k2Force.x * 0.5f * pDeltaTime / mParticle->mass();
                mParticle->velocity().y = originalVelocity.y + k2Force.y * 0.5f * pDeltaTime / mParticle->mass();
                mParticle->velocity().z = originalVelocity.z + k2Force.z * 0.5f * pDeltaTime / mParticle->mass();
            }
        }

        pParticleSystem.applyForces(pDeltaTime);

        // Save intermediate forces (k3)
        for (int i = 0; i < pParticleSystem.particles().size(); ++i) {
            Particle* mParticle = pParticleSystem.particles()[i];
            if (!mParticle->fixed()) {
                mK3Forces[i].set(mParticle->force());
                mK3Velocities[i].set(mParticle->velocity());
            }
        }

        // Get k4 values
        for (int i = 0; i < pParticleSystem.particles().size(); ++i) {
            Particle* mParticle = pParticleSystem.particles()[i];
            if (!mParticle->fixed()) {
                const PVector& originalPosition = mOriginalPositions[i];
                const PVector& k3Velocity       = mK3Velocities[i];

                mParticle->position().x = originalPosition.x + k3Velocity.x * pDeltaTime;
                mParticle->position().y = originalPosition.y + k3Velocity.y * pDeltaTime;
                mParticle->position().z = originalPosition.z + k3Velocity.z * pDeltaTime;

                const PVector& originalVelocity = mOriginalVelocities[i];
                const PVector& k3Force          = mK3Forces[i];

                mParticle->velocity().x = originalVelocity.x + k3Force.x * pDeltaTime / mParticle->mass();
                mParticle->velocity().y = originalVelocity.y + k3Force.y * pDeltaTime / mParticle->mass();
                mParticle->velocity().z = originalVelocity.z + k3Force.z * pDeltaTime / mParticle->mass();
            }
        }

        pParticleSystem.applyForces(pDeltaTime);

        // Save intermediate forces (k4)
        for (int i = 0; i < pParticleSystem.particles().size(); ++i) {
            Particle* mParticle = pParticleSystem.particles()[i];
            if (!mParticle->fixed()) {
                mK4Forces[i].set(mParticle->force());
                mK4Velocities[i].set(mParticle->velocity());
            }
        }

        // Final integration step
        for (int i = 0; i < pParticleSystem.particles().size(); ++i) {
            Particle* mParticle = pParticleSystem.particles()[i];
            if (!mParticle->fixed()) {
                // Update position
                const PVector& originalPosition = mOriginalPositions[i];
                const PVector& k1Velocity       = mK1Velocities[i];
                const PVector& k2Velocity       = mK2Velocities[i];
                const PVector& k3Velocity       = mK3Velocities[i];
                const PVector& k4Velocity       = mK4Velocities[i];

                mParticle->position().x = originalPosition.x + pDeltaTime / 6.0f * (k1Velocity.x + 2.0f * k2Velocity.x + 2.0f * k3Velocity.x + k4Velocity.x);
                mParticle->position().y = originalPosition.y + pDeltaTime / 6.0f * (k1Velocity.y + 2.0f * k2Velocity.y + 2.0f * k3Velocity.y + k4Velocity.y);
                mParticle->position().z = originalPosition.z + pDeltaTime / 6.0f * (k1Velocity.z + 2.0f * k2Velocity.z + 2.0f * k3Velocity.z + k4Velocity.z);

                // Update velocity
                const PVector& originalVelocity = mOriginalVelocities[i];
                const PVector& k1Force          = mK1Forces[i];
                const PVector& k2Force          = mK2Forces[i];
                const PVector& k3Force          = mK3Forces[i];
                const PVector& k4Force          = mK4Forces[i];

                mParticle->velocity().x = originalVelocity.x + pDeltaTime / (6.0f * mParticle->mass()) * (k1Force.x + 2.0f * k2Force.x + 2.0f * k3Force.x + k4Force.x);
                mParticle->velocity().y = originalVelocity.y + pDeltaTime / (6.0f * mParticle->mass()) * (k1Force.y + 2.0f * k2Force.y + 2.0f * k3Force.y + k4Force.y);
                mParticle->velocity().z = originalVelocity.z + pDeltaTime / (6.0f * mParticle->mass()) * (k1Force.z + 2.0f * k2Force.z + 2.0f * k3Force.z + k4Force.z);
            }
        }
    }
};
