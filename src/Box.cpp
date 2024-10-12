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

#include "Box.h"
#include "PVector.h"
#include "Physics.h"
#include "Verlet.h"
#include "Util.h"

using namespace umgebung;

std::vector<PVector> Box::NORMALS = {
    PVector(-1, 0, 0),
    PVector(0, -1, 0),
    PVector(0, 0, -1),
    PVector(1, 0, 0),
    PVector(0, 1, 0),
    PVector(0, 0, 1)};

Box::Box(const PVector& pMin, const PVector& pMax)
    : mCoefficientOfRestitution(1.0f),
      mMax(pMax),
      mMin(pMin),
      mReflectFlag(true),
      mTeleport(false) {
    mID = Physics::getUniqueID();
}

void Box::apply(Physics& pParticleSystem) {
    if (!mActive) {
        return;
    }

    for (auto& myParticle: pParticleSystem.particles()) {
        if (mTeleport) {
            if (myParticle->position().x > mMax.x) {
                myParticle->position().x = mMin.x;
            }
            if (myParticle->position().y > mMax.y) {
                myParticle->position().y = mMin.y;
            }
            if (myParticle->position().z > mMax.z) {
                myParticle->position().z = mMin.z;
            }

            if (myParticle->position().x < mMin.x) {
                myParticle->position().x = mMax.x;
            }
            if (myParticle->position().y < mMin.y) {
                myParticle->position().y = mMax.y;
            }
            if (myParticle->position().z < mMin.z) {
                myParticle->position().z = mMax.z;
            }
        } else {
            int     myTag      = -1;
            PVector myPosition = myParticle->position(); // Clone

            // Collision detection and reflection or velocity reset
            if (myParticle->position().x > mMax.x) {
                myParticle->position().x = mMax.x;
                myTag                    = 0;
            }
            if (myParticle->position().y > mMax.y) {
                myParticle->position().y = mMax.y;
                myTag                    = 1;
            }
            if (myParticle->position().z > mMax.z) {
                myParticle->position().z = mMax.z;
                myTag                    = 2;
            }
            if (myParticle->position().x < mMin.x) {
                myParticle->position().x = mMin.x;
                myTag                    = 3;
            }
            if (myParticle->position().y < mMin.y) {
                myParticle->position().y = mMin.y;
                myTag                    = 4;
            }
            if (myParticle->position().z < mMin.z) {
                myParticle->position().z = mMin.z;
                myTag                    = 5;
            }

            if (myTag >= 0) {
                if (mReflectFlag) {
                    if (dynamic_cast<Verlet*>(pParticleSystem.getIntegrator())) {
                        PVector myDiff = PVector::sub(myPosition, myParticle->old_position());
                        Util::reflect(myDiff, NORMALS[myTag], mCoefficientOfRestitution);
                        myParticle->old_position().sub(myDiff);
                    } else {
                        Util::reflectVelocity(*myParticle, NORMALS[myTag], mCoefficientOfRestitution);
                    }
                } else {
                    myParticle->velocity().set(0, 0, 0);
                }
            }
        }
    }
}
