#ifndef vectors_hpp
#define vectors_hpp

#include <stdio.h>
#include <GLFW/glfw3.h>

class Vec2 {
    public:
        Vec2(float x, float y);
        Vec2();
        float x, y;
};


class Vec3 {
    public:
        Vec3(float r, float g, float b);
        Vec3();
        float r, g, b;
};

#endif /* vectors_hpp */
