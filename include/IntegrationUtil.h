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
#include <memory>
#include <iostream>
#include "Particle.h"
#include "Derivate3f.h"

class IntegrationUtil {
public:
    static void calculateDerivatives(const std::vector<std::shared_ptr<Particle>>& pParticles,
                                     std::vector<Derivate3f>&                      pDerivates) {
        try {
            for (size_t i = 0; i < pParticles.size(); ++i) {
                if (i < pDerivates.size()) {
                    // Set derivative's position (velocity components)
                    pDerivates[i].px = pParticles[i]->velocity().x;
                    pDerivates[i].py = pParticles[i]->velocity().y;
                    pDerivates[i].pz = pParticles[i]->velocity().z;

                    // Set derivative's velocity (acceleration = force / mass)
                    pDerivates[i].vx = pParticles[i]->force().x / pParticles[i]->mass();
                    pDerivates[i].vy = pParticles[i]->force().y / pParticles[i]->mass();
                    pDerivates[i].vz = pParticles[i]->force().z / pParticles[i]->mass();
                }
            }
        } catch (const std::exception& ex) {
            std::cerr << "Error calculating derivatives: " << ex.what() << std::endl;
        }
    }

    template<typename T>
    static void checkContainerSize(const int pSize, std::vector<T>& pContainer) {
        int mDiff = pSize - static_cast<int>(pContainer.size());
        if (mDiff > 0) {
            for (int i = 0; i < mDiff; ++i) {
                try {
                    pContainer.emplace_back(T()); // Use default constructor
                } catch (const std::exception& ex) {
                    std::cerr << "Error resizing container: " << ex.what() << std::endl;
                }
            }
        } else if (mDiff < 0) {
            pContainer.erase(pContainer.end() + mDiff, pContainer.end());
        }
    }
};
