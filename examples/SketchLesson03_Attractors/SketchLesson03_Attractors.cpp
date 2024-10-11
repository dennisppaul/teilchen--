#include "Umgebung.h"
#include "Physics.h"
#include "Particle.h"
#include "Attractor.h"
#include "ViscousDrag.h"
#include "Teleporter.h"

using namespace umgebung;

class UmgebungApp final : public PApplet {

    /*
     * this sketch demonstrates how to create and use an `Attractor` and how to teleport particles.
     *
     * press mouse to toggle attractor between postive and *negative* attraction.
     */

    AttractorPtr mAttractorPtr = Attractor::make();

    /* create a particle system */
    Physics mPhysics;

    void settings() override {
        size(640, 480);
    }

    void setup() override {
        /* create a viscous force that slows down all motion */
        const auto mDrag   = ViscousDrag::make();
        mDrag->coefficient = 0.75f;
        mPhysics.add(mDrag);

        /* teleport particles from one edge of the screen to the other */
        const auto mTeleporter = Teleporter::make();
        mTeleporter->min().set(0, 0);
        mTeleporter->max().set(width, height);
        mPhysics.add(mTeleporter);

        /* create some particles */
        for (int i = 0; i < 1000; i++) {
            const IParticlePtr mParticle = mPhysics.makeParticle();
            mParticle->position().set(random(width), random(height));
            mParticle->mass(random(1.0f, 5.0f));
        }

        /* create an attractor */
        mAttractorPtr->radius(100);
        mAttractorPtr->strength(150);
        mPhysics.add(mAttractorPtr);
    }

    void draw() override {
        /* set attractor to mouse position */
        mAttractorPtr->position().set(mouseX, mouseY);

        /* update the particle system */
        const float mDeltaTime = 1.0f / frameRate;
        mPhysics.step(mDeltaTime);

        /* draw */
        background(1);

        /* draw all the particles in particle system */
        fill(0);
        noStroke();
        for (int i = 0; i < mPhysics.particles().size(); i++) {
            const IParticlePtr mParticle = mPhysics.particles(i);
            ellipse(mParticle->position().x, mParticle->position().y, 5, 5);
        }

        /* draw attractor */
        noFill();
        stroke(0, 0.25f);
        // strokeWeight(1.0f);
        ellipse(mAttractorPtr->position().x, mAttractorPtr->position().y, mAttractorPtr->radius() * 2, mAttractorPtr->radius() * 2);
        if (mAttractorPtr->strength() < 0) {
            noStroke();
            fill(0);
        } else {
            stroke(0);
            // strokeWeight(4.0f);
        }
        ellipse(mAttractorPtr->position().x, mAttractorPtr->position().y, mAttractorPtr->radius() / 2, mAttractorPtr->radius() / 2);
    }

    void mousePressed() override {
        /* flip the direction of the attractors strength. */
        float mInvertedStrength = -1 * mAttractorPtr->strength();
        /* a negative strength turns the attractor into a repulsor */
        mAttractorPtr->strength(mInvertedStrength);
    }
};

PApplet* umgebung::instance() {
    return new UmgebungApp();
}