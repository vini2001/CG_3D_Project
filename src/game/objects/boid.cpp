#include "boid.hpp"


void Boid::animate() {
    float animationLimit = size*2.0;
    if(!animationStarted) {
        yCoord = vertices[leftWingId].coords.y;
        animationStarted = true;
    }

    if(goingUp) {
        vertices[leftWingId].coords.y += size/3.0;
        vertices[rightWingId].coords.y += size/3.0;
        if(vertices[leftWingId].coords.y >= yCoord+animationLimit) {
            goingUp = false;
        }
    } else {
        vertices[leftWingId].coords.y -= size/3.0;
        vertices[rightWingId].coords.y -= size/3.0;
        if(vertices[leftWingId].coords.y <= yCoord+-animationLimit) {
            goingUp = true;
        }
    }
    
}

void Boid::frameUpdate() {
    GObject::frameUpdate();

    if(!rotating) {
        return;
    }

    if(framesToDisableRotation > -1) {
        framesToDisableRotation--;
        if(framesToDisableRotation == -1) {
            rotating = false;
            return;
        }
    }

    GLfloat rotateSpeed = 5.0f * speed * speedMultiplier;
    this->rotateBoid(rotatingNeg ? -rotateSpeed : rotateSpeed);
}

void Boid::rotateBoid(float degree) {
    this->rotate(v3(0, degree, 0));
    
    // rotate speed vector
    this->speedVector = glm::rotateY(this->speedVector, glm::radians(degree));
    rotated += degree;
}

// set the calculated pos to a vertice in the trunk so it doesn't flap as the wings do
v3 Boid::getPos() {
    return this->vertices[5].coords;
}

void Boid::syncWith(Boid *boid) {
    this->rotating = boid->rotating;
    this->rotateBoid(boid->rotated);
    this->speed = boid->speed;
}

Boid::Boid(v3 translation) : Boid(translation, v3(0.0f, 0.0f, 0.0f)) {}

Boid::Boid(v3 translation, v3 color) {
    float heightWings = 1.5f;
    float headLength = 2.0f;
    float backWidth = 2.0f;
    float tailLength = 1.7f;

    bool userDefaultColors = false;
    if(color.x == 0.0f && color.y == 0.0f && color.z == 0.0f) {
        userDefaultColors = true;
    }

    v3 wingsColor = userDefaultColors ? v3(.419f, 0.519f, 0.919f) : color;
    v3 trunkColor = userDefaultColors ? v3(0.2f, 0.2f, 0.2f) : color;
    v3 backColor = userDefaultColors ? v3(0.04f, 0.04f, 0.04f) : color;

    // LEFT WING
    this->leftWingId = this->addVertice(GVertice(v3(0.0f, 0.0f, 0.0f), wingsColor, v2(0.0f, 0.0f)));
    int v2i = this->addVertice(GVertice(v3(3.0f, -heightWings/2, 1.5f), trunkColor, v2(5.0f, 0.0f)));
    int v3i = this->addVertice(GVertice(v3(2.6f, -heightWings/2, -1.5f), trunkColor, v2(5.0f, 0.0f)));
    int v4i = this->addVertice(GVertice(v3(3.0f, heightWings/2, 1.5f), trunkColor, v2(5.0f, 0.0f)));
    int v5i = this->addVertice(GVertice(v3(2.6f, heightWings/2, -1.5f), trunkColor, v2(5.0f, 0.0f)));
    
    this->addTriangle(this->leftWingId, v2i, v3i);
    this->addTriangle(this->leftWingId, v4i, v5i);
    this->addTriangle(this->leftWingId, v2i, v4i);
    this->addTriangle(this->leftWingId, v3i, v5i);

    //TRUNK
    int trli = this->addVertice(GVertice(v3(3.4f, -heightWings/2, -1.5f), trunkColor, v2(5.0f, 0.0f)));
    int trhi = this->addVertice(GVertice(v3(3.4f, heightWings/2, -1.5f), trunkColor, v2(5.0f, 0.0f)));
    this->addTriangle(v2i, v3i, trli);
    this->addTriangle(v4i, v5i, trhi);

    //HEAD
    int headI = this->addVertice(GVertice(v3(3.0f, 0.0f, -1.5 - headLength), v3(0.52f, 0.12f, 0.2f), v2(5.0f, 0.0f)));
    this->addTriangle(v5i, trhi, headI);
    this->addTriangle(v3i, trli, headI);
    this->addTriangle(v3i, v5i, headI);
    this->addTriangle(trli, trhi, headI);

    //RIGHT WING
    this->rightWingId = this->addVertice(GVertice(v3(6.0f, 0.0f, 0.0f), wingsColor, v2(0.0f, 0.0f)));
    this->addTriangle(v2i, this->rightWingId, trli);
    this->addTriangle(v4i, this->rightWingId, trhi);
    this->addTriangle(v2i, this->rightWingId, v4i);
    this->addTriangle(trli, this->rightWingId, trhi);

    //TAIL
    int tailL = this->addVertice(GVertice(v3(3.0f - backWidth/2, 0, 1.5f + tailLength), backColor, v2(0.0f, 0.0f)));
    int tailR = this->addVertice(GVertice(v3(3.0f + backWidth/2, 0, 1.5f + tailLength), backColor, v2(0.0f, 0.0f)));
    this->addTriangle(tailL, tailR, v4i);
    this->addTriangle(tailL, tailR, v2i);
    this->addTriangle(tailR, v2i, v4i);
    this->addTriangle(tailL, v2i, v4i);


    this->rotate(v3(0.0f, 180.0f, 0.0f));
    this->translate(translation);
    this->scale(v3(size, size, size));

    this->generateNormals = true;

    this->speedVector = v3(0.00f, 0.00f, 1.0f);
    this->speed = 0.1f;
}