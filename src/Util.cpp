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

#include "Util.h"
#include "Physics.h"

Particle* Util::findParticleByProximity(Physics& pPhysics, float x, float y, float z, float pSelectionRadius) {
    return findParticleByProximity(pPhysics.particles(), PVector(x, y, z), pSelectionRadius);
}

Particle* Util::findParticleByProximity(Physics& pPhysics, const PVector& pPosition, float pSelectionRadius) {
    return findParticleByProximity(pPhysics.particles(), pPosition, pSelectionRadius);
}

void Util::reflectVelocity(Particle& pParticle, const PVector& pNormal, float pCoefficientOfRestitution) {
    PVector& mVelocity = pParticle.velocity(); // Get velocity reference

    // Normal component
    PVector TMP_NORMAL = pNormal;
    TMP_NORMAL.mult(pNormal.dot(mVelocity)); // Scale by dot product of normal and velocity

    // Tangent component
    PVector TMP_TANGENT = PVector::sub(mVelocity, TMP_NORMAL); // Subtract normal component from velocity

    // Negate normal component with coefficient of restitution
    TMP_NORMAL.mult(-pCoefficientOfRestitution);

    // Set reflection vector: mVelocity = TMP_TANGENT + negated TMP_NORMAL
    mVelocity = PVector::add(TMP_TANGENT, TMP_NORMAL);

    // Update old position if needed
    if (Physics::HINT_UPDATE_OLD_POSITION) {
        pParticle.old_position().set(pParticle.position());
    }
}