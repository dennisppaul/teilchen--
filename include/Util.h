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
#include <cmath>
#include <random>

#include "PVector.h"
#include "Particle.h"
// #include "TriangleDeflector.h"
// #include "TriangleDeflectorIndexed.h"

class Physics;

class Util {
    static constexpr auto     ALMOST_THRESHOLD = 0.001f;
    static std::random_device rd;
    static std::mt19937       RND_GENERATOR;

public:
    template<typename T, typename U>
    static bool is_instance_of(const U* ptr) {
        return dynamic_cast<const T*>(ptr) != nullptr;
    }

    static bool almost(const PVector& p0, const PVector& p1) {
        return std::abs(p1.x - p0.x) < ALMOST_THRESHOLD && std::abs(p1.y - p0.y) < ALMOST_THRESHOLD && std::abs(p1.z - p0.z) < ALMOST_THRESHOLD;
    }

    static bool almost(float a, float b) {
        return std::abs(b - a) < ALMOST_THRESHOLD;
    }

    static PVector cross(const PVector& p1, const PVector& p2) {
        return PVector::cross(p1, p2);
    }

    static float distance(const PVector& p1, const PVector& p2) {
        const float dx = p1.x - p2.x;
        const float dy = p1.y - p2.y;
        const float dz = p1.z - p2.z;
        return std::sqrt(dx * dx + dy * dy + dz * dz);
    }

    static float distanceSquared(const PVector& p0, const PVector& p1) {
        const float dx = p0.x - p1.x;
        const float dy = p0.y - p1.y;
        const float dz = p0.z - p1.z;
        return (dx * dx + dy * dy + dz * dz);
    }

    static void divide(PVector& p, const PVector& pVector) {
        p.x /= pVector.x;
        p.y /= pVector.y;
        p.z /= pVector.z;
    }

    static float fastInverseSqrt(float v) {
        const float half = 0.5f * v;
        int         i    = *reinterpret_cast<int*>(&v);
        i                = 0x5f375a86 - (i >> 1);
        v                = *reinterpret_cast<float*>(&i);
        return v * (1.5f - half * v * v);
    }

    static Particle* findParticleByProximity(Physics* pPhysics, float x, float y, float z, float pSelectionRadius);
    static Particle* findParticleByProximity(Physics* pPhysics, const PVector& pPosition, float pSelectionRadius);

    static Particle* findParticleByProximity(const std::vector<Particle*>& pParticles, float x, float y, float z, float pSelectionRadius) {
        return findParticleByProximity(pParticles, PVector(x, y, z), pSelectionRadius);
    }

    static Particle* findParticleByProximity(const std::vector<Particle*>& pParticles, const PVector& pPosition, float pSelectionRadius) {
        std::vector<Particle*> mCloseParticles;
        for (const auto& p: pParticles) {
            if (PVector::dist(pPosition, p->position()) < pSelectionRadius) {
                mCloseParticles.push_back(p);
            }
        }
        if (mCloseParticles.empty()) {
            return nullptr;
        }
        Particle* mClosestParticle = mCloseParticles[0];
        float      mClosestDistance = PVector::dist(pPosition, mClosestParticle->position());
        for (size_t i = 1; i < mCloseParticles.size(); i++) {
            const float mDistance = PVector::dist(pPosition, mCloseParticles[i]->position());
            if (mDistance < mClosestDistance) {
                mClosestDistance = mDistance;
                mClosestParticle = mCloseParticles[i];
            }
        }
        return mClosestParticle;
    }

    static bool inside2DPolygon(const PVector& pPoint, const std::vector<PVector>& pPolygon) {
        const float x = pPoint.x;
        const float y = pPoint.y;
        int         c = 0;
        for (size_t i = 0, j = pPolygon.size() - 1; i < pPolygon.size(); j = i++) {
            if (((pPolygon[i].y <= y && y < pPolygon[j].y) || (pPolygon[j].y <= y && y < pPolygon[i].y)) &&
                (x < (pPolygon[j].x - pPolygon[i].x) * (y - pPolygon[i].y) / (pPolygon[j].y - pPolygon[i].y) + pPolygon[i].x)) {
                c = (c + 1) % 2;
            }
        }
        return c == 1;
    }

    static bool isNaN(const PVector& p) {
        return std::isnan(p.x) || std::isnan(p.y) || std::isnan(p.z);
    }

    static void scale(PVector& v1, const PVector& v2) {
        v1.x *= v2.x;
        v1.y *= v2.y;
        v1.z *= v2.z;
    }

    static void setVelocityAndOldPosition(Particle* pParticle, const PVector& pNewVelocity) {
        pParticle->velocity().set(pNewVelocity);
        const PVector mOldPosition = PVector::sub(pParticle->position(), pParticle->velocity());
        pParticle->old_position().set(mOldPosition);
    }

    static float circumcenter_triangle(const PVector& a, const PVector& b, const PVector& c, PVector& result) {
        const PVector ac    = PVector::sub(c, a);
        const PVector ab    = PVector::sub(b, a);
        const PVector abXac = PVector::cross(ab, ac);

        const PVector p0                   = PVector::mult(PVector::cross(abXac, ab), ac.magSq());
        const PVector p1                   = PVector::mult(PVector::cross(ac, abXac), ab.magSq());
        const PVector p2                   = PVector::add(p0, p1);
        const PVector toCircumsphereCenter = PVector::mult(p2, 1.0f / (2.0f * abXac.magSq()));
        const float   circumsphereRadius   = toCircumsphereCenter.mag();

        const PVector ccs = PVector::add(a, toCircumsphereCenter);
        result.set(ccs);

        return circumsphereRadius;
    }

    static bool point_in_triangle(const PVector& a, const PVector& b, const PVector& c, const PVector& point) {
        const PVector v0    = PVector::sub(c, a);
        const PVector v1    = PVector::sub(b, a);
        const PVector v2    = PVector::sub(point, a);
        const double  d00   = v0.dot(v0);
        const double  d01   = v0.dot(v1);
        const double  d02   = v0.dot(v2);
        const double  d11   = v1.dot(v1);
        const double  d12   = v1.dot(v2);
        const double  denom = d00 * d11 - d01 * d01;
        const double  u     = (d11 * d02 - d01 * d12) / denom;
        const double  v     = (d00 * d12 - d01 * d02) / denom;
        return u >= 0 && v >= 0 && u + v <= 1;
    }

    static float distance_point_plane(const PVector& point, const PVector& plane_origin, const PVector& plane_normal) {
        const PVector d         = PVector::sub(plane_origin, point);
        const float   dot       = plane_normal.dot(d);
        const float   magnitude = plane_normal.mag();
        return dot / magnitude;
    }

    static PVector project_vector_onto_plane(const PVector& vector, const PVector& plane_normal) {
        const float dot = vector.x * plane_normal.x + vector.y * plane_normal.y + vector.z * plane_normal.z;
        const float x   = vector.x - dot * plane_normal.x;
        const float y   = vector.y - dot * plane_normal.y;
        const float z   = vector.z - dot * plane_normal.z;
        return PVector{x, y, z};
    }

    static PVector project_point_onto_plane(const PVector& point, const PVector& plane_origin, const PVector& plane_normal) {
        const PVector v         = PVector::sub(point, plane_origin);
        const float   dot       = plane_normal.dot(v);
        const float   magnitude = plane_normal.mag();
        const PVector projection((v.x - dot * plane_normal.x) / magnitude,
                                 (v.y - dot * plane_normal.y) / magnitude,
                                 (v.z - dot * plane_normal.z) / magnitude);
        return PVector::add(plane_origin, projection);
    }

    static PVector project_point_onto_line(const PVector& point, const PVector& line_point_a, const PVector& line_point_b) {
        PVector lineDirection = PVector::sub(line_point_b, line_point_a);
        lineDirection.normalize();
        const float projection = PVector::dot(lineDirection, PVector::sub(point, line_point_a));
        return PVector::add(line_point_a, PVector::mult(lineDirection, projection));
    }

    static PVector project_point_onto_line_segment(const PVector& point, const PVector& p1, const PVector& p2) {
        PVector     lineDirection = PVector::sub(p2, p1);
        const float lineLength    = lineDirection.mag();
        lineDirection.normalize();

        const float projection = PVector::dot(lineDirection, PVector::sub(point, p1));

        if (projection < 0) {
            return p1;
        }
        if (projection > lineLength) {
            return p2;
        }
        return PVector::add(p1, PVector::mult(lineDirection, projection));
    }

    static bool is_parallel(const PVector& vector, const PVector& normal) {
        return PVector::dot(vector, normal) == 0;
    }
};
