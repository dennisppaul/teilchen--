#include "Umgebung.h"
#include "Physics.h"
#include "BasicParticle.h"

using namespace umgebung;

class UmgebungApp final : public PApplet {

    BasicParticlePtr mParticle = nullptr;
    Physics          mPhysics;

    void settings() override {
        size(1024, 768);
    }

    void setup() override {
        mParticle = mPhysics.makeParticle();
    }

    void draw() override {
        const float mDeltaTime = 1.0f / frameRate;
        mPhysics.step(mDeltaTime);

        background(1);
        fill(0);
        noStroke();
        ellipse(mParticle->position().x, mParticle->position().y, 5, 5);

        /* reset particle s position and velocity */
        if (isMousePressed) {
            mParticle->position().set(mouseX, mouseY);
            mParticle->velocity().set(mouseX - pmouseX, mouseY - pmouseY);
            mParticle->velocity().mult(10);
            println("velocity: ", mParticle->velocity().x, ", ", mParticle->velocity().y);
        }
    }
};

PApplet* umgebung::instance() {
    return new UmgebungApp();
}