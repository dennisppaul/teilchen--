#include "Umgebung.h"
#include "Physics.h"
#include "Particle.h"

using namespace umgebung;

class UmgebungApp final : public PApplet {

    /*
     * this sketch demonstrates how to create a particle system with a single particle in it.
     *
     * drag mouse to fling particle.
     */

    Particle* mParticle = nullptr;
    Physics   mPhysics;

    void settings() override {
        size(640, 480);
    }

    void setup() override {
        mParticle = mPhysics.makeParticle();
    }

    void draw() override {
        const float mDeltaTime = 1.0f / frameRate;
        mPhysics.step(mDeltaTime);

        background(1.0f);
        fill(0.0f);
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