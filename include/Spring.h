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
#include "IConnection.h"
#include "IParticle.h"
#include "Util.h"
#include "PVector.h"

class Spring final : public IForce, public IConnection {

    IParticle* mA;
    IParticle* mB;
    bool       mActive;
    bool       mDead;
    bool       mOneWay;
    float      mRestLength;
    float      mSpringConstant;
    float      mSpringDamping;
    const long mID;

public:
#define USE_FAST_SQRT 1

    Spring(IParticle* pA, IParticle* pB)
        : Spring(pA,
                 pB,
                 2.0f,
                 0.1f,
                 PVector::dist(pA->position(), pB->position())) {}

    Spring(IParticle* pA,
           IParticle* pB,
           float      pSpringConstant,
           float      pSpringDamping,
           float      pRestLength);

    Spring(IParticle* pA, IParticle* pB, const float pRestLength)
        : Spring(pA, pB, 2.0f, 0.1f, pRestLength) {}

    Spring(IParticle* pA, IParticle* pB, const float pSpringConstant, const float pSpringDamping)
        : Spring(pA, pB, pSpringConstant, pSpringDamping, PVector::dist(pA->position(), pB->position())) {}

    void setRestLengthByPosition() {
        restlength(PVector::dist(mA->position(), mB->position()));
    }

    float restlength() const {
        return mRestLength;
    }

    void restlength(const float pRestLength) {
        mRestLength = pRestLength;
    }

    IParticle* a() override {
        return mA;
    }

    IParticle* b() override {
        return mB;
    }

    IParticle* a(IParticle* pA) {
        return mA = pA;
    }

    IParticle* b(IParticle* pB) {
        return mB = pB;
    }

    float currentLength() const {
        return PVector::dist(mA->position(), mB->position());
    }

    float strength() const {
        return mSpringConstant;
    }

    void strength(const float pSpringConstant) {
        mSpringConstant = pSpringConstant;
    }

    float damping() const {
        return mSpringDamping;
    }

    void damping(const float pSpringDamping) {
        mSpringDamping = pSpringDamping;
    }

    void setOneWay(const bool pOneWayState) {
        mOneWay = pOneWayState;
    }

    void apply(float pDeltaTime, Physics& pParticleSystem) override {
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
            float   mSpringForce = -mSpringConstant * (mDistance - mRestLength);
            PVector mABV         = PVector::sub(mA->velocity(), mB->velocity());
            PVector mForce       = PVector(-mSpringForce, -mSpringForce, -mSpringForce);
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

    bool dead() const override {
        return mA->dead() || mB->dead() || mDead;
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

    static Spring* make(IParticle* pA, IParticle* pB) {
        return new Spring(pA, pB);
    }

    static Spring* make(IParticle* pA, IParticle* pB, float pSpringConstant, float pSpringDamping, float pRestLength) {
        return new Spring(pA, pB, pSpringConstant, pSpringDamping, pRestLength);
    }

    static Spring* make(IParticle* pA, IParticle* pB, float pRestLength) {
        return new Spring(pA, pB, pRestLength);
    }

    static Spring* make(IParticle* pA, IParticle* pB, float pSpringConstant, float pSpringDamping) {
        return new Spring(pA, pB, pSpringConstant, pSpringDamping);
    }
};