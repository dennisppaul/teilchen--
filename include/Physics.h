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
#include "BasicParticle.h"
#include "Particle.h"
#include "IForce.h"
#include "IConstraint.h"
#include "IIntegrator.h"
#include "PVector.h"

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
    std::vector<std::shared_ptr<IConstraint>> mConstraints;
    std::vector<std::shared_ptr<IForce>>      mForces;
    std::vector<std::shared_ptr<Particle>>    mParticles;
    std::shared_ptr<IIntegrator>              mIntegrator;

public:
    Physics();

    static long getUniqueID() {
        return ++oID;
    }

    // Particle management
    bool add(const std::shared_ptr<Particle>& pParticle, bool pPreventDuplicates = false) {
        if (pPreventDuplicates) {
            auto it = std::find(mParticles.begin(), mParticles.end(), pParticle);
            if (it != mParticles.end()) {
                return false;
            }
        }
        mParticles.push_back(pParticle);
        return true;
    }

    void add(const std::shared_ptr<Particle>& pParticle) {
        mParticles.push_back(pParticle);
    }

    void add(const std::vector<std::shared_ptr<Particle>>& pParticles) {
        mParticles.insert(mParticles.end(), pParticles.begin(), pParticles.end());
    }

    void remove(const std::shared_ptr<Particle>& pParticle) {
        mParticles.erase(std::remove(mParticles.begin(), mParticles.end(), pParticle), mParticles.end());
    }

    void remove(const std::vector<std::shared_ptr<Particle>>& pParticles) {
        for (const auto& p: pParticles) {
            remove(p);
        }
    }

    const std::vector<std::shared_ptr<Particle>>& particles() const {
        return mParticles;
    }

    std::shared_ptr<Particle> particles(int pIndex) const {
        return mParticles.at(pIndex);
    }

    std::shared_ptr<BasicParticle> makeParticle(const PVector& pPosition) {
        auto mParticle = makeParticle();
        mParticle->setPositionRef(pPosition);
        mParticle->old_position() = mParticle->position();
        return mParticle;
    }

    std::shared_ptr<BasicParticle> makeParticle() {
        auto mParticle = std::make_shared<BasicParticle>();
        mParticles.push_back(mParticle);
        return mParticle;
    }

    std::shared_ptr<BasicParticle> makeParticle(float x, float y) {
        auto mParticle = makeParticle();
        mParticle->position().set(x, y);
        mParticle->old_position() = mParticle->position();
        return mParticle;
    }

    std::shared_ptr<BasicParticle> makeParticle(float x, float y, float z) {
        auto mParticle = makeParticle();
        mParticle->position().set(x, y, z);
        mParticle->old_position() = mParticle->position();
        return mParticle;
    }

    std::shared_ptr<BasicParticle> makeParticle(float x, float y, float z, float pMass) {
        auto mParticle = makeParticle(x, y, z);
        mParticle->mass(pMass);
        return mParticle;
    }

    std::shared_ptr<BasicParticle> makeParticle(const PVector& pPosition, float pMass) {
        auto mParticle = makeParticle(pPosition);
        mParticle->mass(pMass);
        return mParticle;
    }

    template<typename T>
    std::shared_ptr<T> makeParticle() {
        auto mParticle = std::make_shared<T>();
        mParticles.push_back(mParticle);
        return mParticle;
    }

    void removeTags() const {
        for (const auto& p: mParticles) {
            p->tag(false);
        }
    }

    // // Force management
    // bool add(const std::shared_ptr<Spring>& pSpring, bool pPreventDuplicates = false) {
    //     if (pPreventDuplicates) {
    //         for (const auto& f : mForces) {
    //             auto s = std::dynamic_pointer_cast<Spring>(f);
    //             if (s && (s == pSpring || (s->a() == pSpring->a() && s->b() == pSpring->b()) || (s->b() == pSpring->a() && s->a() == pSpring->b()))) {
    //                 return false;
    //             }
    //         }
    //     }
    //     mForces.push_back(pSpring);
    //     return true;
    // }

    void add(const std::shared_ptr<IForce>& pForce) {
        mForces.push_back(pForce);
    }

    void addForces(const std::vector<std::shared_ptr<IForce>>& pForces) {
        mForces.insert(mForces.end(), pForces.begin(), pForces.end());
    }

    void remove(const std::shared_ptr<IForce>& pForce) {
        mForces.erase(std::remove(mForces.begin(), mForces.end(), pForce), mForces.end());
    }

    const std::vector<std::shared_ptr<IForce>>& forces() const {
        return mForces;
    }

    std::shared_ptr<IForce> forces(int pIndex) const {
        return mForces.at(pIndex);
    }

    void applyForces(float pDeltaTime) {
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

    // Constraint management
    void add(const std::shared_ptr<IConstraint>& pConstraint) {
        mConstraints.push_back(pConstraint);
    }

    void addConstraints(const std::vector<std::shared_ptr<IConstraint>>& pConstraints) {
        mConstraints.insert(mConstraints.end(), pConstraints.begin(), pConstraints.end());
    }

    void remove(const std::shared_ptr<IConstraint>& pConstraint) {
        mConstraints.erase(std::remove(mConstraints.begin(), mConstraints.end(), pConstraint), mConstraints.end());
    }

    const std::vector<std::shared_ptr<IConstraint>>& constraints() const {
        return mConstraints;
    }

    std::shared_ptr<IConstraint> constraints(int pIndex) const {
        return mConstraints.at(pIndex);
    }

    // Integrator management
    void setIntegratorRef(const std::shared_ptr<IIntegrator>& pIntegrator) {
        mIntegrator = pIntegrator;
    }

    std::shared_ptr<IIntegrator> getIntegrator() const {
        return mIntegrator;
    }

    void step(float pDeltaTime, int pIterations) {
        for (int i = 0; i < pIterations; ++i) {
            step(pDeltaTime / static_cast<float>(pIterations));
        }
    }

    void step(float pDeltaTime) {
        applyForces(pDeltaTime);
        mIntegrator->step(pDeltaTime, *this);
        handleParticles(pDeltaTime);
    }

private:
    void handleParticles(float pDeltaTime) {
        for (auto& p: mParticles) {
            p->force().set(0, 0, 0);
            p->age(p->age() + pDeltaTime);

            if (p->dead()) {
                remove(p);
            }

            // Handle optimization and NaN recovery, if needed
        }
    }
};
