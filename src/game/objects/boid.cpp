#include "boid.hpp"


void Boid::animate() {
    float animationLimit = 0.6f;
    if(!animationStarted) {
        yCoord = vertices[leftWingId].coords.y;
        animationStarted = true;
    }

    if(goingUp) {
        vertices[leftWingId].coords.y += 0.1;
        vertices[rightWingId].coords.y += 0.1;
        if(vertices[leftWingId].coords.y >= yCoord+animationLimit) {
            goingUp = false;
        }
    } else {
        vertices[leftWingId].coords.y -= 0.1;
        vertices[rightWingId].coords.y -= 0.1;
        if(vertices[leftWingId].coords.y <= yCoord+-animationLimit) {
            goingUp = true;
        }
    }
    
}

void Boid::frameUpdate() {
    GObject::frameUpdate();

    if(rotating <= 0.0f) return;
    GLfloat rotateSpeed = 0.5f;
    this->rotate(v3(0, rotateSpeed, 0));
    
    // rotate speed vector
    this->speed = glm::rotateY(this->speed, glm::radians(rotateSpeed));
    rotating -= rotateSpeed;
}

// set the calculated pos to a vertice in the trunk so it doesn't flap as the wings do
v3 Boid::getPos() {
    return this->vertices[5].coords;
}

Boid::Boid(v3 translation) {
    float heightWings = 1.5f;
    float headLength = 2.0f;
    float backWidth = 2.0f;
    float tailLength = 1.7f;

    v3 wingsColor = v3(.419f, 0.519f, 0.919f);
    v3 trunkColor = v3(0.2f, 0.2f, 0.2f);
    v3 backColor = v3(0.04f, 0.04f, 0.04f);

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
    this->scale(v3(0.2f, 0.2f, 0.2f));

    this->generateNormals = true;
}