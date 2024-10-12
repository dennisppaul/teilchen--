#include "Umgebung.h"
#include "Physics.h"
#include "Spring.h"
#include "ViscousDrag.h"

using namespace umgebung;

class UmgebungApp final : public PApplet {

    /*
     * this sketch demonstrates how to create a `Spring` that connects two particles. it also
     * demonstrates how to create a `ViscousDrag` to slow down particle motion over time.
     *
     * drag mouse to move particle.
     */

    /* create a particle system */
    Physics   mPhysics;
    Spring*   mSpring = nullptr;

    void settings() override {
        size(640, 480);
    }

    void setup() override {
        /* create a viscous force that slows down all motion; 0 means no slowing down. */
        const auto mDrag = ViscousDrag::make(0.25f);
        mPhysics.add(mDrag);

        /* create two particles that we can connect with a spring */
        IParticle* mA = mPhysics.makeParticle();
        mA->position().set(width / 2.0f - 50, height / 2.0f);

        IParticle* mB = mPhysics.makeParticle();
        mB->position().set(width / 2.0f + 50, height / 2.0f);

        /* create a spring force that connects two particles.
         * note that there is more than one way to create a spring.
         * in our case the restlength of the spring is defined by the
         * particles current position.
         */
        mSpring = mPhysics.makeSpring(mA, mB);
    }

    void draw() override {
        /* set first particle to mouse position */
        if (isMousePressed) {
            mSpring->a()->position().set(mouseX, mouseY);
        }

        /* update the particle system */
        const float mDeltaTime = 1.0f / frameRate;
        mPhysics.step(mDeltaTime);

        /* draw particles and connecting line */
        background(255);
        noFill();
        stroke(0, 63);
        line(mSpring->a()->position().x, mSpring->a()->position().y, mSpring->b()->position().x, mSpring->b()->position().y);
        fill(0);
        noStroke();
        ellipse(mSpring->a()->position().x, mSpring->a()->position().y, 5, 5);
        ellipse(mSpring->b()->position().x, mSpring->b()->position().y, 15, 15);
    }
};

PApplet* umgebung::instance() {
    return new UmgebungApp();
}