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

#include <vector>
#include <iostream>
#include "Midpoint.h"
#include "Physics.h"
#include "IntegrationUtil.h"

auto Midpoint::step(const float pDeltaTime, Physics& pParticleSystem) -> void {
    try {
        // Lock particles and mK1 if necessary for multithreading (use std::mutex in real-world cases)
        const auto& particles = pParticleSystem.particles();

        // Ensure the size of mK1 matches the particle system
        IntegrationUtil::checkContainerSize(static_cast<int>(particles.size()), mK1);
        // First integration step
        pParticleSystem.applyForces(pDeltaTime);
        IntegrationUtil::calculateDerivatives(particles, mK1);

        for (size_t i = 0; i < particles.size(); ++i) {
            auto& mParticle = particles[i];
            if (!mParticle->fixed() && i < mK1.size()) {
                // Update particle positions with the first set of derivatives (mK1)
                mParticle->position().x += mK1[i].px * pDeltaTime / 2.0f;
                mParticle->position().y += mK1[i].py * pDeltaTime / 2.0f;
                mParticle->position().z += mK1[i].pz * pDeltaTime / 2.0f;
                mParticle->velocity().x += mK1[i].vx * pDeltaTime / 2.0f;
                mParticle->velocity().y += mK1[i].vy * pDeltaTime / 2.0f;
                mParticle->velocity().z += mK1[i].vz * pDeltaTime / 2.0f;
            }
        }

        // Second integration step
        pParticleSystem.applyForces(pDeltaTime);
        IntegrationUtil::calculateDerivatives(particles, mK1);

        for (size_t i = 0; i < particles.size(); ++i) {
            auto& mParticle = particles[i];
            if (!mParticle->fixed() && i < mK1.size()) {
                // Update particle positions and velocities with the second set of derivatives (mK1)
                mParticle->position().x += mK1[i].px * pDeltaTime;
                mParticle->position().y += mK1[i].py * pDeltaTime;
                mParticle->position().z += mK1[i].pz * pDeltaTime;
                mParticle->velocity().x += mK1[i].vx * pDeltaTime;
                mParticle->velocity().y += mK1[i].vy * pDeltaTime;
                mParticle->velocity().z += mK1[i].vz * pDeltaTime;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception during midpoint integration: " << e.what() << std::endl;
    }
}