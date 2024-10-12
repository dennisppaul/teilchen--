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
#include "Util.h"
#include "PGraphics.h"

class TeilchenDrawLib {
public:
    static void drawSprings(PGraphics* g, const Physics& pParticleSystem, const uint32_t pColor) {
        if (g == nullptr) {
            return;
        }
        /* draw springs */
        g->stroke(pColor);
        for (int i = 0; i < pParticleSystem.forces().size(); i++) {
            if (Util::is_instance_of<Spring>(pParticleSystem.forces(i))) {
                auto* mSpring = dynamic_cast<Spring*>(pParticleSystem.forces(i));
                g->line(mSpring->a()->position().x,
                        mSpring->a()->position().y,
                        mSpring->b()->position().x,
                        mSpring->b()->position().y);
            }
        }
    }
};