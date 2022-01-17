#include "gameController.hpp"
#include "utils.hpp"
#include <iostream>
#include "GText.hpp"
#include "globals.hpp"
#include <glm/glm.hpp>
#include "vectors.hpp"

#define DEBUG_MODE_FALSE -100


GameController::GameController() {}


float spaceShipAcceleration = 0.01;
void GameController::init(Shader *shaderProgram){
    this->shader = shaderProgram;
    
    gText = new GText();
    lastAttack = getMillis();
    game::started = true;
}

void GameController::handleInput(GLuint pressedKey, GLuint pressedMouseButton, Vec2 mousePos) {
    
    if(pressedKey == GLFW_KEY_R) {
        destroy();
        init(shader);
    }
    
    if(!game::started) return;
}


void GameController::frameActions() {
    game::frameCount++;
    
    if(game::paused) {
       
    }
}

void GameController::resizeScreen() {
    delete gText;
    gText = new GText();
}



void GameController::drawElements() {
    if(vao != NULL) delete vao;
    
    vao = new VAO();
    vao->bind();
    int sizeVArray = 0;
    int sizeIArray = 0;
    
    if(vArray != NULL) { free(vArray); vArray = NULL; }
    if(indices != NULL) { free(indices); indices = NULL; }
    
    vArray = (GLfloat*) malloc(sizeVArray);
    indices = (GLuint*) malloc(sizeIArray);
    long arrayPos = 0;
    long indicesPos = 0;
    
    vbo1 = new VBO(vArray, (int) sizeVArray);
    
    ebo1 = new EBO(indices, (int) sizeIArray);
    
    vao->linkAttrib(*vbo1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
    vao->linkAttrib(*vbo1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    
    vao->unbind();
    delete vbo1;
    delete ebo1;
    
    vao->bind();
    // glDrawElements(GL_TRIANGLES, trianglesQuantity*3, GL_UNSIGNED_INT, 0);
    
    
    for(int i = 0; i < flashMessages.size(); i++) {
        if(flashMessages[i].finishAt > getMillis()) {
            FlashMessage m = flashMessages[i];
            drawText(m.text, m.x, 70 * i + m.y, flashMessages[i].scale, m.rgb);
        }else{
            flashMessages.erase(flashMessages.begin()+i--);
        }
    }
}

void GameController::drawText(string text, float x, float y, float scale, glm::vec3 colors){
    gText->renderText(text, x, y, scale, colors);
    shader->activate();
}

void GameController::destroy() {
    if(gText != NULL) delete gText;
    gText = NULL;
    flashMessages.clear();
    
}

GameController::~GameController() {
    destroy();
}
