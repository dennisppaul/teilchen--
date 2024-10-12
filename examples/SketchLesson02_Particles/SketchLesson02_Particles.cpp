#include "Umgebung.h"
#include "Physics.h"
#include "Particle.h"
#include "Gravity.h"

using namespace umgebung;

class UmgebungApp final : public PApplet {

    /*
     * this sketch demonstrates how to create and handle multiple particles and remove individual
     * particles.
     *
     * drag mouse to spawn particles.
     */

    /* create a particle system */
    Physics mPhysics;

    void settings() override {
        size(640, 480);
        antialiasing = 8;
    }

    void setup() override {
        /* create a gravitational force and add it to the particle system */
        const auto mGravity = Gravity::make(0, 30, 0);
        mPhysics.add(mGravity);
    }

    void draw() override {
        if (isMousePressed) {
            /* create and add a particle to the system */
            const auto mParticle = mPhysics.makeParticle();
            /* set particle to mouse position with random velocity */
            mParticle->position().set(mouseX, mouseY);
            mParticle->velocity().set(random(-20, 20), random(-50));
        }

        /* remove particles right before they hit the edge of the screen */
        for (int i = 0; i < mPhysics.particles().size(); i++) {
            Particle* mParticle = mPhysics.particles(i);
            if (mParticle->position().y > height * 0.9f) {
                /* particles can be marked dead and will be removed on the next call to `Physics.step()` */
                mParticle->dead(true);
            }
        }

        /* update the particle system */
        const float mDeltaTime = 1.0f / frameRate;
        mPhysics.step(mDeltaTime);

        /* draw all the particles in the system */
        background(1);
        fill(0.0f);
        noStroke();
        for (int i = 0; i < mPhysics.particles().size(); i++) {
            Particle* mParticle = mPhysics.particles(i);
            ellipse(mParticle->position().x, mParticle->position().y, 5, 5);
        }

        /* draw edge */
        stroke(0, 0.25f);
        line(0, height * 0.9f, 20, height * 0.9f);
        line(width - 20, height * 0.9f, width, height * 0.9f);
    }
};

PApplet* umgebung::instance() {
    return new UmgebungApp();
}