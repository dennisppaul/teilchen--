#include "Umgebung.h"
#include "Physics.h"
#include "Gravity.h"
#include "Spring.h"
#include "Particle.h"
#include "ViscousDrag.h"
#include "StableSpringQuad.h"
#include "Box.h"
#include "RungeKutta.h"
#include "TeilchenDrawLib.h"

using namespace umgebung;

class UmgebungApp final : public PApplet {

    /*
     * this sketch demonstrates how to connect four particles and six springs to form a
     * stable quad made from springs, this a construct that allows to emulate something
     * similar to a *body*. this sketch also demonstrates how to use `StableSpringQuad`
     * to achieve the same result.
     *
     * press mouse to drag corner of stable quad.
     */

    /* create a particle system */
    Physics   mPhysics;
    Particle* mRoot = nullptr;

    void settings() override {
        size(640, 480);
    }

    void setup() override {
        /* use `RungeKutta` as it produces more stable results in applications like these */
        mPhysics.replace_integrator(new RungeKutta());

        Gravity* mGravity   = Gravity::make();
        mGravity->force().y = 98.1f;
        mPhysics.add(mGravity);

        /* add drag to smooth the spring interaction */
        mPhysics.add(new ViscousDrag(0.2f));

        /* add a container */
        const auto mBox = new Box();
        mBox->min().set(0, 0, 0);
        mBox->max().set(width, height, 0);
        mPhysics.add(mBox);

        /* create root */
        Particle* a = mPhysics.makeParticle(0, 0);
        Particle* b = mPhysics.makeParticle(100, 0);
        Particle* c = mPhysics.makeParticle(100, 100);
        Particle* d = mPhysics.makeParticle(0, 100);

        /* create stable quad from springs */
        /* first the edge-springs ... */
        constexpr float mSpringConstant = 100;
        constexpr float mSpringDamping  = 5;
        mPhysics.makeSpring(a, b, mSpringConstant, mSpringDamping);
        mPhysics.makeSpring(b, c, mSpringConstant, mSpringDamping);
        mPhysics.makeSpring(c, d, mSpringConstant, mSpringDamping);
        mPhysics.makeSpring(d, a, mSpringConstant, mSpringDamping);
        /* ... then the diagonal-springs */
        mPhysics.makeSpring(a, c, mSpringConstant, mSpringDamping);
        mPhysics.makeSpring(b, d, mSpringConstant, mSpringDamping);

        /* define 'a' as root particle for mouse interaction */
        mRoot = a;
        mRoot->fixed(true);
        mRoot->radius(10);

        /* create stable quad with `StableSpringQuad` */
        new StableSpringQuad(mPhysics, d, c, mPhysics.makeParticle(100, 200), mPhysics.makeParticle(0, 200));
    }

    void draw() override {
        /* handle particles */
        if (isMousePressed) {
            mRoot->fixed(true);
            mRoot->position().set(mouseX, mouseY);
        } else {
            mRoot->fixed(false);
        }

        mPhysics.step(1.0f / frameRate);

        /* draw particles *manually* and springs using `DrawLib` */
        background(1.0f);
        noStroke();
        fill(0.0f);
        for (Particle* p: mPhysics.particles()) {
            ellipse(p->position().x, p->position().y, 5, 5);
        }
        TeilchenDrawLib::drawSprings(g, mPhysics, color(0, 0.25));

        /* highlight root particle */
        noStroke();
        fill(0.0f);
        ellipse(mRoot->position().x, mRoot->position().y, 15, 15);
    }
};

PApplet* umgebung::instance() {
    return new UmgebungApp();
}