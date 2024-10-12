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

#include "PVector.h"
#include "Constraint.h"

class Physics;
class Verlet;
class Particle;

using namespace umgebung;

class Box final : public Constraint {

    static std::vector<PVector> NORMALS;
    bool                        mActive = true;
    float                       mCoefficientOfRestitution;
    bool                        mDead = false;
    long                        mID{};
    PVector                     mMax;
    PVector                     mMin;
    bool                        mReflectFlag;
    bool                        mTeleport;

public:
    Box(const PVector& pMin, const PVector& pMax);

    Box() : Box(PVector(), PVector()) {}

    void teleport(const bool pTeleportState) {
        mTeleport = pTeleportState;
    }

    void reflect(const bool pReflectState) {
        mReflectFlag = pReflectState;
    }

    PVector& min() {
        return mMin;
    }

    PVector& max() {
        return mMax;
    }

    void coefficientofrestitution(const float pCoefficientOfRestitution) {
        mCoefficientOfRestitution = pCoefficientOfRestitution;
    }

    float coefficientofrestitution() const {
        return mCoefficientOfRestitution;
    }

    void apply(Physics& pParticleSystem) override;

    bool active() const override {
        return mActive;
    }

    void active(const bool pActiveState) override {
        mActive = pActiveState;
    }

    bool dead() const override {
        return mDead;
    }

    void dead(const bool pDead) override {
        mDead = pDead;
    }

    long ID() const override {
        return mID;
    }

    static long getUniqueID() {
        static long uniqueID = 0;
        return uniqueID++;
    }
};
