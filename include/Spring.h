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

#include "Force.h"
#include "Connection.h"
#include "Particle.h"
#include "PVector.h"

class Spring final : public Force, public Connection {

    Particle* mA;
    Particle* mB;
    bool       mActive;
    bool       mDead;
    bool       mOneWay;
    float      mRestLength;
    float      mSpringConstant;
    float      mSpringDamping;
    const long mID;

public:
#define USE_FAST_SQRT 1

    Spring(Particle* pA, Particle* pB)
        : Spring(pA,
                 pB,
                 2.0f,
                 0.1f,
                 PVector::dist(pA->position(), pB->position())) {}

    Spring(Particle* pA,
           Particle* pB,
           float      pSpringConstant,
           float      pSpringDamping,
           float      pRestLength);

    Spring(Particle* pA, Particle* pB, const float pRestLength)
        : Spring(pA, pB, 2.0f, 0.1f, pRestLength) {}

    Spring(Particle* pA, Particle* pB, const float pSpringConstant, const float pSpringDamping)
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

    Particle* a() override {
        return mA;
    }

    Particle* b() override {
        return mB;
    }

    Particle* a(Particle* pA) {
        return mA = pA;
    }

    Particle* b(Particle* pB) {
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

    void apply(float pDeltaTime, Physics& pParticleSystem) override ;

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

    static Spring* make(Particle* pA, Particle* pB) {
        return new Spring(pA, pB);
    }

    static Spring* make(Particle* pA, Particle* pB, float pSpringConstant, float pSpringDamping, float pRestLength) {
        return new Spring(pA, pB, pSpringConstant, pSpringDamping, pRestLength);
    }

    static Spring* make(Particle* pA, Particle* pB, float pRestLength) {
        return new Spring(pA, pB, pRestLength);
    }

    static Spring* make(Particle* pA, Particle* pB, float pSpringConstant, float pSpringDamping) {
        return new Spring(pA, pB, pSpringConstant, pSpringDamping);
    }
};