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

#include "Physics.h"
#include "Midpoint.h"
#include "Util.h"

Physics::Physics()
    : mIntegrator(new Midpoint()) {
}

bool Physics::VERBOSE                  = false;
bool Physics::HINT_UPDATE_OLD_POSITION = true;
long Physics::oID                      = -1;

void Physics::handleForces() {
    if (HINT_REMOVE_DEAD) {
        for (auto it = mForces.begin(); it != mForces.end(); ) {
            if ((*it)->dead()) {
                it = mForces.erase(it); // Remove dead forces
            } else {
                ++it; // Move to the next force
            }
        }
    }
}

void Physics::handleParticles(const float pDeltaTime) {
    try {
        for (auto it = mParticles.begin(); it != mParticles.end();) {
            const auto& mParticle = *it;

            // Clear force
            mParticle->force().set(0, 0, 0);

            // Age particle
            mParticle->age(mParticle->age() + pDeltaTime);

            // Remove dead particles
            if (HINT_REMOVE_DEAD && mParticle->dead()) {
                it = mParticles.erase(it);
                continue; // Skip the rest of the loop since we removed this particle
            }

            // Recover NaN values
            if (HINT_RECOVER_NAN) {
                if (Util::isNaN(mParticle->position())) {
                    if (Util::isNaN(mParticle->old_position())) {
                        mParticle->position().set(0, 0, 0);
                    } else {
                        mParticle->position().set(mParticle->old_position());
                    }
                }

                if (Util::isNaN(mParticle->velocity())) {
                    mParticle->velocity().set(0, 0, 0);
                }
            }

            // Optimize still particles
            if (HINT_OPTIMIZE_STILL) {
                const float mSpeed = mParticle->velocity().magSq();
                mParticle->still(mSpeed > -EPSILON && mSpeed < EPSILON);
            }

            ++it; // Move to the next particle
        }
    } catch (const std::exception& ex) {
        if (VERBOSE) {
            // Print exception (use proper logging mechanism as needed)
            std::cerr << ex.what() << std::endl;
        }
    }
}

void Physics::handleConstraints() {
    for (auto it = mConstraints.begin(); it != mConstraints.end();) {
        const auto& mConstraint = *it;
        mConstraint->apply(*this); // Apply the constraint

        // Check if the constraint should be removed if it's dead
        if (HINT_REMOVE_DEAD && mConstraint->dead()) {
            it = mConstraints.erase(it); // Remove dead constraint and move iterator forward
        } else {
            ++it; // Move to the next constraint
        }
    }
}

void Physics::postHandleParticles(float pDeltaTime) const {
    if (HINT_SET_VELOCITY_FROM_PREVIOUS_POSITION || HINT_UPDATE_OLD_POSITION) {
        try {
            for (auto& mParticle: mParticles) {
                if (HINT_SET_VELOCITY_FROM_PREVIOUS_POSITION) {
                    if (mParticle->fixed()) {
                        PVector velocity = PVector::sub(mParticle->position(), mParticle->old_position());
                        mParticle->velocity().set(velocity);
                    }
                }

                if (HINT_UPDATE_OLD_POSITION) {
                    mParticle->old_position().set(mParticle->position());
                }
            }
        } catch (const std::exception& ex) {
            if (VERBOSE) {
                std::cerr << ex.what() << std::endl;
            }
        }
    }
}