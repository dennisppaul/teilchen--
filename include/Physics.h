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

#include <iostream>
#include <vector>

#include "Particle.h"
#include "Force.h"
#include "Constraint.h"
#include "Integrator.h"
#include "BasicParticle.h"
#include "PVector.h"
#include "Spring.h"

using namespace umgebung;

class Physics {
public:
    static bool            VERBOSE;
    static constexpr float EPSILON = 0.001f;
    static bool            HINT_UPDATE_OLD_POSITION;
    static long            oID;

    bool HINT_OPTIMIZE_STILL                      = true;
    bool HINT_RECOVER_NAN                         = true;
    bool HINT_REMOVE_DEAD                         = true;
    bool HINT_SET_VELOCITY_FROM_PREVIOUS_POSITION = true;

private:
    std::vector<Constraint*> mConstraints;
    std::vector<Force*>      mForces;
    std::vector<Particle*>   mParticles;
    Integrator*              mIntegrator;

public:
    Physics();

    static long getUniqueID() {
        return ++oID;
    }

    bool add(Particle* pParticle, const bool pPreventDuplicates = false) {
        if (pPreventDuplicates) {
            const auto it = std::find(mParticles.begin(), mParticles.end(), pParticle);
            if (it != mParticles.end()) {
                return false;
            }
        }
        mParticles.push_back(pParticle);
        return true;
    }

    void add(Particle* pParticle) {
        mParticles.push_back(pParticle);
    }

    void add(const std::vector<Particle*>& pParticles) {
        mParticles.insert(mParticles.end(), pParticles.begin(), pParticles.end());
    }

    void remove(Particle* pParticle) {
        mParticles.erase(std::remove(mParticles.begin(), mParticles.end(), pParticle), mParticles.end());
    }

    void remove(const std::vector<Particle*>& pParticles) {
        for (const auto& p: pParticles) {
            remove(p);
        }
    }

    const std::vector<Particle*>& particles() const {
        return mParticles;
    }

    Particle* particles(const int pIndex) const {
        return mParticles.at(pIndex);
    }

    BasicParticle* makeParticle() {
        const auto mParticle = new BasicParticle();
        mParticles.push_back(mParticle);
        return mParticle;
    }

    BasicParticle* makeParticle(const PVector& pPosition) {
        const auto mParticle = makeParticle();
        mParticle->setPositionRef(pPosition);
        mParticle->old_position() = mParticle->position();
        return mParticle;
    }

    BasicParticle* makeParticle(const float x, const float y) {
        const auto mParticle = makeParticle();
        mParticle->position().set(x, y);
        mParticle->old_position() = mParticle->position();
        return mParticle;
    }

    BasicParticle* makeParticle(const float x, const float y, const float z) {
        const auto mParticle = makeParticle();
        mParticle->position().set(x, y, z);
        mParticle->old_position() = mParticle->position();
        return mParticle;
    }

    BasicParticle* makeParticle(const float x, const float y, const float z, const float pMass) {
        const auto mParticle = makeParticle(x, y, z);
        mParticle->mass(pMass);
        return mParticle;
    }

    BasicParticle* makeParticle(const PVector& pPosition, const float pMass) {
        const auto mParticle = makeParticle(pPosition);
        mParticle->mass(pMass);
        return mParticle;
    }

    template<typename T>
    T* makeParticle() {
        auto mParticle = new T();
        mParticles.push_back(mParticle);
        return mParticle;
    }

    void removeTags() const {
        for (const auto& p: mParticles) {
            p->tag(false);
        }
    }

    /* force management */

    bool add(Spring* pSpring, const bool pPreventDuplicates = false) {
        if (pPreventDuplicates) {
            for (const auto& f: mForces) {
                auto s = dynamic_cast<Spring*>(f);
                if (s && (s == pSpring || (s->a() == pSpring->a() && s->b() == pSpring->b()) || (s->b() == pSpring->a() && s->a() == pSpring->b()))) {
                    return false;
                }
            }
        }
        mForces.push_back(pSpring);
        return true;
    }

    void add(Force* pForce) {
        mForces.push_back(pForce);
    }

    void addForces(std::vector<Force*>& pForces) {
        mForces.insert(mForces.end(), pForces.begin(), pForces.end());
    }

    void remove(Force* pForce) {
        mForces.erase(std::remove(mForces.begin(), mForces.end(), pForce), mForces.end());
    }

    const std::vector<Force*>& forces() const {
        return mForces;
    }

    Force* forces(const int pIndex) const {
        return mForces.at(pIndex);
    }

    void applyForces(const float pDeltaTime) {
        for (const auto& p: mParticles) {
            if (!p->fixed()) {
                p->accumulateInnerForce(pDeltaTime);
            }
        }

        for (const auto& f: mForces) {
            if (f->active()) {
                f->apply(pDeltaTime, *this);
            }
        }
    }

    template<typename T>
    T* makeForce() {
        T* mForce;
        try {
            mForce = new T();
            mForces.push_back(mForce);
        } catch (const std::exception& ex) {
            (void) ex;
            mForce = nullptr;
        }
        return mForce;
    }

    Spring* makeSpring(Particle* pA, Particle* pB) {
        const auto mSpring = new Spring(pA, pB);
        mForces.push_back(mSpring);
        return mSpring;
    }

    Spring* makeSpring(Particle* pA, Particle* pB, float pRestLength) {
        const auto mSpring = new Spring(pA, pB, pRestLength);
        mForces.push_back(mSpring);
        return mSpring;
    }

    Spring* makeSpring(Particle* pA, Particle* pB, float pSpringConstant, float pSpringDamping) {
        const auto mSpring = new Spring(pA, pB, pSpringConstant, pSpringDamping);
        mForces.push_back(mSpring);
        return mSpring;
    }

    Spring* makeSpring(Particle* pA, Particle* pB, float pSpringConstant, float pSpringDamping, float pRestLength) {
        const auto mSpring = new Spring(pA, pB, pSpringConstant, pSpringDamping, pRestLength);
        mForces.push_back(mSpring);
        return mSpring;
    }

    void add(Constraint* pConstraint) {
        mConstraints.push_back(pConstraint);
    }

    void addConstraints(const std::vector<Constraint*>& pConstraints) {
        mConstraints.insert(mConstraints.end(), pConstraints.begin(), pConstraints.end());
    }

    void remove(const Constraint* pConstraint) {
        mConstraints.erase(std::remove(mConstraints.begin(), mConstraints.end(), pConstraint), mConstraints.end());
    }

    const std::vector<Constraint*>& constraints() const {
        return mConstraints;
    }

    Constraint* constraints(const int pIndex) const {
        return mConstraints.at(pIndex);
    }

    void setIntegratorRef(Integrator* pIntegrator) {
        mIntegrator = pIntegrator;
    }

    Integrator* getIntegrator() const {
        return mIntegrator;
    }

    void replace_integrator(Integrator* pIntegrator) {
        delete mIntegrator;
        mIntegrator = pIntegrator;
    }

    void step(const float pDeltaTime, const int pIterations) {
        for (int i = 0; i < pIterations; ++i) {
            step(pDeltaTime / static_cast<float>(pIterations));
        }
    }

    void step(const float pDeltaTime) {
        handleForces();
        mIntegrator->step(pDeltaTime, *this);
        handleParticles(pDeltaTime);
        handleConstraints();
        postHandleParticles(pDeltaTime);
    }

private:
    void handleForces();
    void handleParticles(float pDeltaTime);
    void handleConstraints();
    void postHandleParticles(float pDeltaTime) const;
};
