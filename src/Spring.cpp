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

#include "Spring.h"
#include "Physics.h"
#include "Util.h"

Spring::Spring(Particle*   pA,
               Particle*   pB,
               const float pSpringConstant,
               const float pSpringDamping,
               const float pRestLength)
    : mA(pA),
      mB(pB),
      mActive(true),
      mDead(false),
      mOneWay(false),
      mRestLength(pRestLength),
      mSpringConstant(pSpringConstant),
      mSpringDamping(pSpringDamping),
      mID(Physics::getUniqueID()) {}

void Spring::apply(float pDeltaTime, Physics& pParticleSystem) {
    if (!(mA->fixed() && mB->fixed())) {
        PVector mAB = PVector::sub(mA->position(), mB->position());
        float   mInvDistance;
        float   mDistance;
#if USE_FAST_SQRT == 1
        float mInvDistanceSquared = mAB.magSq();
        if (mInvDistanceSquared == 0.0f) {
            return;
        }
        mInvDistance = Util::fastInverseSqrt(mInvDistanceSquared);
        mDistance    = 1.0f / mInvDistance;
#else
        mDistance = mAB.mag();
        if (mDistance == 0.0f) {
            return;
        }
        mInvDistance = 1.0f / mDistance;
#endif // USE_FAST_SQRT
        const float mSpringForce = -mSpringConstant * (mDistance - mRestLength);
        auto        mABV         = PVector::sub(mA->velocity(), mB->velocity());
        auto        mForce       = PVector(-mSpringForce, -mSpringForce, -mSpringForce);
        Util::scale(mABV, mAB);
        mABV.mult(mInvDistance);
        mABV.mult(mSpringDamping);
        mForce.add(mABV);
        mAB.mult(-mInvDistance);
        Util::scale(mForce, mAB);

        if (mOneWay) {
            if (!mB->fixed()) {
                mForce.mult(-2);
                mB->force().add(mForce);
            }
        } else {
            if (!mA->fixed()) {
                mA->force().add(mForce);
            }
            if (!mB->fixed()) {
                mB->force().sub(mForce);
            }
        }
    }
}