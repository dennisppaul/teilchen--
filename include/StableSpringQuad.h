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

#include "Physics.h"
#include "PVector.h"
#include "Particle.h"
#include "Spring.h"

class StableSpringQuad {
public:
    Particle* a;
    Spring*   ab;
    Spring*   ac;
    Particle* b;
    Spring*   bc;
    Spring*   bd;
    Particle* c;
    Spring*   cd;
    Particle* d;
    Spring*   da;

    StableSpringQuad(Physics& pParticleSystem, const PVector& pA, const PVector& pB, const PVector& pC, const PVector& pD) {
        a = pParticleSystem.makeParticle();
        b = pParticleSystem.makeParticle();
        c = pParticleSystem.makeParticle();
        d = pParticleSystem.makeParticle();

        a->setPositionRef(pA);
        b->setPositionRef(pB);
        c->setPositionRef(pC);
        d->setPositionRef(pD);

        // Edges
        constexpr float mSpringConstant = 100.0f;
        constexpr float mSpringDamping  = 5.0f;
        ab                              = pParticleSystem.makeSpring(a, b, mSpringConstant, mSpringDamping);
        bc                              = pParticleSystem.makeSpring(b, c, mSpringConstant, mSpringDamping);
        cd                              = pParticleSystem.makeSpring(c, d, mSpringConstant, mSpringDamping);
        da                              = pParticleSystem.makeSpring(d, a, mSpringConstant, mSpringDamping);

        // Diagonals
        ac = pParticleSystem.makeSpring(a, c, mSpringConstant, mSpringDamping);
        bd = pParticleSystem.makeSpring(b, d, mSpringConstant, mSpringDamping);
    }

    StableSpringQuad(Physics& pParticleSystem, Particle* pA, Particle* pB, Particle* pC, Particle* pD) {
        a = pA;
        b = pB;
        c = pC;
        d = pD;

        // Edges
        constexpr float mSpringConstant = 500.0f;
        constexpr float mSpringDamping  = 5.0f;
        ab                              = pParticleSystem.makeSpring(a, b, mSpringConstant, mSpringDamping);
        bc                              = pParticleSystem.makeSpring(b, c, mSpringConstant, mSpringDamping);
        cd                              = pParticleSystem.makeSpring(c, d, mSpringConstant, mSpringDamping);
        da                              = pParticleSystem.makeSpring(d, a, mSpringConstant, mSpringDamping);

        // Diagonals
        ac = pParticleSystem.makeSpring(a, c, mSpringConstant, mSpringDamping);
        bd = pParticleSystem.makeSpring(b, d, mSpringConstant, mSpringDamping);
    }
};
