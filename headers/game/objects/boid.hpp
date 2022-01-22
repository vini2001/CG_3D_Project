
#ifndef boid_hpp
#define boid_hpp

#include "GObject.hpp"
#include <glm/glm.hpp>

typedef glm::vec3 v3;
typedef glm::vec2 v2;

class Boid : public GObject {
    public:
        void animate();
        Boid(v3 translation);

    private:
        int leftWingId;
        int rightWingId;

        bool goingUp = true;

        bool animationStarted = false;
        float yCoord;

};

#endif