
#ifndef boid_hpp
#define boid_hpp

#include "GObject.hpp"
#include <glm/glm.hpp>

typedef glm::vec3 v3;
typedef glm::vec2 v2;

class Boid : public GObject {
    public:
        void animate();
        void frameUpdate();
        Boid(v3 translation);

        GLfloat rotating = 0.0f;

        v3 getPos();

    private:
        int leftWingId;
        int rightWingId;

        bool goingUp = true;

        bool animationStarted = false;
        float yCoord;

};

#endif