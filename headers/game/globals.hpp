#ifndef globals_hpp
#define globals_hpp

#include <stdio.h>
#include <GLFW/glfw3.h>

#define DEBUG_ALL 1

namespace game {
    extern GLFWwindow *window;
    extern int width, height;
    extern int shootingInterval;
    extern bool colisionsEnabled;
    extern bool started;
    extern long pausedTime;
    extern long pausedAt;
    extern bool paused;
    extern long frameCount;
}
extern int debugLevel;
extern long framesSinceUpdate;

#endif /* globals_hpp */
