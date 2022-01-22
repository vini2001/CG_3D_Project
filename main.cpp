#include <iostream>
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include "linmath.h"
#include <stdlib.h>
#include <stdio.h>
#include "utils.hpp"

#include "vectors.hpp"
#include "shaderClass.hpp"

#include <stb/stb_image.h>
#include <map>
#include "globals.hpp"
#include "gameController.hpp"
#include "GText.hpp"
#include "texture.hpp"

using namespace std;

GameController gameController;

static GLuint pressedKey;
static GLuint pressedMouseButton = -1;
static map<GLuint, bool> keyIsPressed;

vector<GLuint> supportedKeys = {
    GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_ESCAPE, GLFW_PRESS, GLFW_KEY_S,
    GLFW_KEY_R, GLFW_KEY_D
};
static bool holdingKey;
static void key_callback(GLFWwindow* windowGame, int key, int scancode, int action, int mods){
    switch (key) {
        case GLFW_KEY_ESCAPE:
        case GLFW_PRESS:
            glfwSetWindowShouldClose(windowGame, GLFW_TRUE);
            break;
        default:
            // if(!(find(supportedKeys.begin(), supportedKeys.end(), key) != supportedKeys.end())) {
            //     // unsupported key
            //     break;
            // }

            if(pressedKey == key && action == GLFW_RELEASE) {
                pressedKey = (GLuint) NULL;
                keyIsPressed[pressedKey] = false;
            }else if(action == GLFW_PRESS || action == GLFW_REPEAT){
                if(action == GLFW_PRESS) {
                    pressedKey = key;
                }
                keyIsPressed[pressedKey] = true;
                holdingKey = action == GLFW_REPEAT;
            }
            break;
    }
}

void mouse_button_callback(GLFWwindow* windowGame, int button, int action, int mods){
    if (action == GLFW_PRESS) {
        pressedMouseButton = button;
    }else{
        pressedMouseButton = -1;
    }
}

void WindowSizeCallback(GLFWwindow* window, int width, int height){
    game::width = width;
    game::height = height;
    
    gameController.resizeScreen();
    gameController.drawElements();
}

static bool firstChange = true;
Vec2 mousePos;
static void cursor_position_callback(GLFWwindow* window, double x, double y){
    // if(firstChange) {
    //     x = 0;
    //     firstChange = false;
    // }
    // if(abs(x) > game::width / 2) {
    //     x = x > 0 ? ( game::width / 2 ) : -(game::width / 2);
    //     glfwSetCursorPos(game::window, x, y);
    // }
    // if(abs(y) > game::height / 2) {
    //     y = y > 0 ? (game::height / 2) : -(game::height / 2);
    //     glfwSetCursorPos(game::window, x, y);
    // }
    // mousePos = Vec2(x, y);
}

int main(void){

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
//    glfwWindowHint(GLFW_REFRESH_RATE, 15);
    
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
     
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    
    game::window = glfwCreateWindow(1440, 900, "CG game", NULL, NULL);
    if(game::window == NULL) {
        cout << "Failed to create windowGame" << endl;
        glfwTerminate();
        return 0;
    }
    
    glfwSetKeyCallback(game::window, key_callback);
    glfwSetMouseButtonCallback(game::window, mouse_button_callback);
    glfwSetCursorPosCallback(game::window, cursor_position_callback);
    glfwSetWindowSizeCallback(game::window, WindowSizeCallback);
    // glfwSetInputMode(game::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwGetWindowSize(game::window, &game::width, &game::height);
    
    cout << "height: " << game::height << ", width: " << game::width << endl;
    
    glfwMakeContextCurrent(game::window);
    gladLoadGL();
    
    // Needed for text to work
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    
    Shader shaderProgram("shaders/default.vert", "shaders/default.frag");
    gameController.init(&shaderProgram);
    
    vector<int> fpsV = {60, 60, 60};
    int fpsCount = 0;
    int fpsSum = 0;
    
    long lastRealTime = getRealMillis();
    long lastGameTime = getMillis();
    
    long lastFpsUpdate = getMillis();
    string fpsS;

    while (!glfwWindowShouldClose(game::window)) {
        
        long gameTimeEllapsed = getMillis() - lastGameTime;
        
        // first triggered frame on debug mode will be instant, so make sure it jumps about 10 ms = 1frame
        if(gameTimeEllapsed == 0) gameTimeEllapsed = 10;
        
        long realTimeEllapsed = getRealMillis() - lastRealTime;
        
        // every 10 ms do the render process
        if(realTimeEllapsed > 10) {
            
            glEnable(GL_DEPTH_TEST);

            // getMillis() will subtract the paused time, so framesSinceUpdate will always subtract the paused time to avoid time jumps
            lastGameTime = getMillis();
            framesSinceUpdate = gameTimeEllapsed;
            
            lastRealTime = getRealMillis();
            
            glfwPollEvents();
            
            fpsV.erase(fpsV.begin()+0);
            fpsV.push_back(1000/gameTimeEllapsed);
            int fps = (fpsV[0]+fpsV[1]+fpsV[2])/3;
            fpsCount++; fpsSum += fps;
            
            glClearColor(0.03f, 0.06f, 0.08f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


            gameController.handleInput(pressedKey, pressedMouseButton, mousePos);
            pressedKey = -1;
            pressedMouseButton = -1; // set as -1 to avoid duplicated actions

            gameController.frameActions();
            gameController.drawElements();
            
            // update fps every 100 so it doesn't flash too fast
            if(getMillis() - lastFpsUpdate > 100) {
                fpsS = "FPS "; fpsS.append(to_string(fps));
                lastFpsUpdate = getMillis();
            }
            
            gameController.drawText(fpsS, game::width-70, game::height-30, 0.3f, fps < 60 ? glm::vec3(0.8, 0.1f, 0.1f) :  glm::vec3(0.1, 0.7f, 0.1f));
            
            
            glfwSwapBuffers(game::window);
        }
    }
    
    gameController.destroy();
    glfwDestroyWindow(game::window);
    glfwTerminate();
    return 0;
}
