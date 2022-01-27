
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

        bool rotating = 0.0f;
        bool rotatingNeg = false;
        vector<GLfloat> rotatingQueue;

        v3 getPos();

        void rotateBoid(float degree);
        void syncWith(Boid *boid);

        float rotated = 0.0f;

        int framesToDisableRotation = -1;

        v3 originalPosition;

    private:
        int leftWingId;
        int rightWingId;

        bool goingUp = true;

        bool animationStarted = false;
        float yCoord;

        float size = 0.3f;

};

#endif