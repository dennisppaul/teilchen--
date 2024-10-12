#include "Umgebung.h"
#include "Physics.h"
#include "Particle.h"
#include "Gravity.h"

using namespace umgebung;

class UmgebungApp final : public PApplet {

    /*
     * this sketch demonstrates how to create a particle system with a single particle in it and a
     * gravity force pulling it downward.
     *
     * drag mouse to fling particle.
     */

    Physics   mPhysics;
    Particle* mParticle = nullptr;

    void settings() override {
        size(640, 480);
    }

    void setup() override {
        /* create a gravitational force */
        const auto mGravity = Gravity::make();
        /* the direction of the gravity is defined by the 'force' vector */
        mGravity->force().set(0, 30);
        /* forces, like gravity or any other force, can be added to the system. they will be automatically applied to
         all particles */
        mPhysics.add(mGravity);

        /* create a particle and add it to the system */
        mParticle = mPhysics.makeParticle();
    }

    void draw() override {
        /* update the particle system. this applies the gravity to the particle */
        const float mDeltaTime = 1.0f / frameRate;
        mPhysics.step(mDeltaTime);

        /* draw particle */
        background(1);
        fill(0);
        noStroke();
        ellipse(mParticle->position().x, mParticle->position().y, 5, 5);

        /* reset particle s position and velocity */
        if (isMousePressed) {
            mParticle->position().set(mouseX, mouseY);
            mParticle->velocity().set(mouseX - pmouseX, mouseY - pmouseY);
            mParticle->velocity().mult(10);
        }
    }
};

PApplet* umgebung::instance() {
    return new UmgebungApp();
}