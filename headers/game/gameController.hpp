#ifndef gameController_hpp
#define gameController_hpp

#include <stdio.h>
#include <vector>
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"
#include "GText.hpp"
#include "vectors.hpp"
#include "texture.hpp"
#include "camera.hpp"
#include "GObject.hpp"
#include "boid.hpp"


using namespace std;


struct FlashMessage {
    float x, y;
    string text;
    glm::vec3 rgb;
    long finishAt;
    float scale;
    
    FlashMessage(string text, float x, float y, glm::vec3 rgb, long finishAt) : text(text), x(x), y(y), rgb(rgb), finishAt(finishAt), scale(0.6) {}
    
    FlashMessage(string text, float x, float y, glm::vec3 rgb, long finishAt, float scale) : text(text), x(x), y(y), rgb(rgb), finishAt(finishAt), scale(scale) {}
};

class GameController {
public:
    void drawElements();
    void drawObjects(vector<GObject*> &objects, Texture *tex);
    void detectColisions();
    void handleInput(GLuint pressedKey, GLuint pressedMouseButton, Vec2 mousePos);
    void frameActions();
    void drawText(string text, float x, float y, float scale, glm::vec3 colors);
    void destroy();
    void resizeScreen();
    
    ~GameController();
    
    bool playerAlive = false;
    
    void init(Shader *shaderProgram);
    void initLight();
    
    GameController();
    
private:
    VAO* vao = NULL;
    VBO* vbo1;
    EBO* ebo1;
    GLfloat* vArray = NULL;
    GLuint* indices = NULL;
    GText *gText;
    
    vector<FlashMessage> flashMessages;
    Shader *shader;

    VAO *lightVAO;
    Shader *lightShader;

    Texture *brickTex;
    Texture *skyTex;
    Camera *camera;

    vector<GObject*> objects;
    vector<GObject*> objectsPlaneText;
    vector<Boid*> boids;

    vector<GObject*> walls;

    GObject* plane;
    GObject* sky;
    GObject* tower;

    v3 towerTop;
    
    bool boidsRotating = false;
    
    long lastAttack = 0;

    v3 getBoidGroupCenter();

    bool lockedOrientation = true;
    bool lockedPositionBehind = false;
};

#endif /* gameController_hpp */
