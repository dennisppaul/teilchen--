#include "Umgebung.h"
#include "Physics.h"
#include "Spring.h"
#include "ViscousDrag.h"

using namespace umgebung;

class UmgebungApp final : public PApplet {

    /*
     * this sketch demonstrates how to connect multiple particles with springs.
     *
     * move close to particle and press mouse to create springs and particles.
     */

    /* create a particle system */
    Physics     mPhysics;
    ParticlePtr mRoot;

    void settings() override {
        size(640, 480);
    }

    void setup() override {
        /* create a viscous force that slows down all motion; 0 means no slowing down. */
        const auto mDrag = ViscousDrag::make(0.25f);
        mPhysics.add(mDrag);

        /* create a particle to which we will connect springs */
        mRoot = mPhysics.makeParticle(width / 2.0f, height / 2.0f, 0.0f);
        /* we give the root particle a higher mass so it doesn t move as easily */
        mRoot->mass(30);
    }

    void draw() override {
        /* create a particle at mouse position and connect it to the root particle through a spring */
        if (isMousePressed) {
            /* find the particle closest to the mouse */
            const IParticlePtr mNeighborParticle = Util::findParticleByProximity(&mPhysics, mouseX, mouseY, 0, 20);
            if (mNeighborParticle != nullptr) {
                const IParticlePtr mParticle = mPhysics.makeParticle(mouseX, mouseY, 0);
                const SpringPtr    mSpring   = mPhysics.makeSpring(mNeighborParticle, mParticle);
                /* restlength defines the desired length of the spring. in this case it is the
                distance between the two particles. */
                const float mRestlength = mSpring->restlength();
                /* we modify the restlength to add a bit of energy into the system */
                mSpring->restlength(10 + mRestlength * random(2.0f, 4.0f));
            }
        }

        /* update the particle system */
        const float mDeltaTime = 1.0f / frameRate;
        mPhysics.step(mDeltaTime);

        /* draw particles and connecting line */
        background(1);

        /* draw springs */
        noFill();
        stroke(0, 0.125f);
        for (const auto& i: mPhysics.forces()) {
            if (Util::is_instance_of<Spring>(i)) {
                const auto mSSpring = std::dynamic_pointer_cast<Spring>(i);
                line(mSSpring->a()->position().x,
                     mSSpring->a()->position().y,
                     mSSpring->b()->position().x,
                     mSSpring->b()->position().y);
            }
        }
        /* draw particles */
        fill(0);
        noStroke();
        for (const auto& i: mPhysics.particles()) {
            ellipse(i->position().x, i->position().y, 5, 5);
        }
    }
};

PApplet* umgebung::instance() {
    return new UmgebungApp();
}