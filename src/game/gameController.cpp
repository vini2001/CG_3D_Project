#include "gameController.hpp"
#include "utils.hpp"
#include <iostream>
#include "GText.hpp"
#include "globals.hpp"
#include <glm/glm.hpp>
#include "vectors.hpp"
#include "GObject.hpp"
#include "customObjects.hpp"

#define DEBUG_MODE_FALSE -100


GameController::GameController() {}


GLfloat lightVertices[] =
{ //     COORDINATES     //
	-0.1f, -0.1f,  0.1f,
	-0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f,  0.1f,
	-0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f,  0.1f
};

GLuint lightIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};


float spaceShipAcceleration = 0.01;
void GameController::init(Shader *shaderProgram){
    this->shader = shaderProgram;
    
    gText = new GText();
    game::started = true;

    glfwGetWindowSize(game::window, &game::width, &game::height);
    this->camera = new Camera(game::width, game::height, glm::vec3(5.0f, 1.0f, 5.0f));

    initLight();

    // Texture
	brickTex = new Texture("brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	brickTex->texUnit(*shaderProgram, "tex0", 0);

    skyTex = new Texture("whitetex.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	skyTex->texUnit(*shaderProgram, "tex0", 0);


    // Objects
    objects.clear();
    GLfloat pi = 3.14159265f;
    GObject *pyr1 = createPyramid();
    GObject *pyr2 = createPyramid();
    GObject *pyr3 = createPyramid();
    GObject *wall = createWall();
    GObject *wall2 = createWall();
    GObject *wall3 = createWall();
    GObject *wall4 = createWall();
    this->plane = createPlane();
    this->sky = createPlane(v3(3, 173, 252), 1.0f);
    wall2->rotate(glm::vec3(0.0f, 270, 0.0f));

    wall4->rotate(glm::vec3(0.0f, 270, 0.0f));
    wall3->translate(glm::vec3(0.0f, 0.0f, 384.0f));
    wall4->translate(glm::vec3(384.0f, 0.0f, 0.0f));
    this->sky->translate(glm::vec3(0.0f, 300.0f, 0.0f));

    this->tower = createTower();
    this->tower->translate(glm::vec3((384.0f/2.0f), 0.0f, (384.0f/2.0f)));

    for(int i = 1; i <= 15; i++) {
        for(int j = 1; j <= 15; j++) {
            GObject *boid = createBoid(v3(10.0f * j, abs(10.0f - j%20)*5.0f, 10.0f * i));
            objectsPlaneText.push_back(boid);
        }
    }

    pyr2->translate(v3(10.0f, 0.0f, 10.0f));
    pyr3->translate(v3(0.0f, 2.3f,  1.5f));
    objects.push_back(pyr1);
    objects.push_back(pyr2);
    objects.push_back(pyr3);
    objects.push_back(wall);
    objects.push_back(wall2);
    objects.push_back(wall3);
    objects.push_back(wall4);
    objects.push_back(plane);
    objects.push_back(tower);

    objectsPlaneText.push_back(this->sky);
}

void GameController::initLight() {
    this->lightShader = new Shader("shaders/light.vert", "shaders/light.frag");

    lightVAO = new VAO();
    lightVAO->bind();

    VBO lightVBO(lightVertices, sizeof(lightVertices));
    EBO lightEBO(lightIndices, sizeof(lightIndices));

    lightVAO->linkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
    lightVAO->unbind();
    lightVBO.unbind();
    lightEBO.unbind();

    v3 lightColor = v3(1.0f, 1.0f, 1.0f);
    v3 lightPos = v3(384.0f/2.0f,  500.0f, 384.0f/2.0f);
    m4 lightModel = m4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);

    v3 pyramidPos = v3(0.0f, 0.0f, 0.0f);
    m4 pyramidModel = m4(1.0f);
    pyramidModel = glm::translate(pyramidModel, pyramidPos);

    lightShader->activate();
    glUniformMatrix4fv(glGetUniformLocation(lightShader->id, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
    glUniform3f(glGetUniformLocation(lightShader->id, "lightColor"), lightColor.x, lightColor.y, lightColor.z);
    this->shader->activate();
    glUniformMatrix4fv(glGetUniformLocation(this->shader->id, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
    glUniform3f(glGetUniformLocation(this->shader->id, "lightColor"), lightColor.x, lightColor.y, lightColor.z);
    glUniform3f(glGetUniformLocation(this->shader->id, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
}

void GameController::handleInput(GLuint pressedKey, GLuint pressedMouseButton, Vec2 mousePos) {
    
    if(pressedKey == GLFW_KEY_R) {
        destroy();
        init(shader);
    }else if(pressedKey == GLFW_KEY_C) { // Bring light to camera
        // print camera position
        v3 camPos = camera->position;
        printf("Camera position: %f, %f, %f\n", camPos.x, camPos.y, camPos.z);
        v3 lightPos = v3(camera->position.x, camera->position.y, camera->position.z);
        m4 lightModel = m4(1.0f);
        lightModel = glm::translate(lightModel, lightPos);

        this->lightShader->activate();
        glUniformMatrix4fv(glGetUniformLocation(this->lightShader->id, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
        this->shader->activate();
        glUniform3f(glGetUniformLocation(this->shader->id, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
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


float rotation = 0.0f;
double prevTime = glfwGetTime();
void GameController::drawElements() {

    camera->inputs(game::window);
    camera->updateMatrix(45.0f, 0.1f, 1000.0f);

    this->shader->activate();
    glUniform3f(glGetUniformLocation(this->shader->id, "camPos"), camera->position.x, camera->position.y, camera->position.z);
    camera->matrix(this->shader, "camMatrix");

    if(vao != NULL) delete vao;
    
    drawObjects(objects, brickTex);
    drawObjects(objectsPlaneText, skyTex);

    this->lightShader->activate();
    camera->matrix(lightShader, "camMatrix");
    lightVAO->bind();
    glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);
    
    
    for(int i = 0; i < flashMessages.size(); i++) {
        if(flashMessages[i].finishAt > getMillis()) {
            FlashMessage m = flashMessages[i];
            drawText(m.text, m.x, 70 * i + m.y, flashMessages[i].scale, m.rgb);
        }else{
            flashMessages.erase(flashMessages.begin()+i--);
        }
    }
}

void GameController::drawObjects(vector<GObject*> &objects, Texture* tex) {
    vao = new VAO();
    vao->bind();
    tex->bind();
    int sizeVArray = 0;
    int sizeIArray = 0;
    
    if(vArray != NULL) { free(vArray); vArray = NULL; }
    if(indices != NULL) { free(indices); indices = NULL; }
    
    long arrayPos = 0;
    long indicesPos = 0;
    int totalVertices = 0;
    int totalIndices = 0;


    for(int i = 0; i < objects.size(); i++) {
        GObject *obj = objects[i];
        totalVertices += obj->vertices.size();
        totalIndices += obj->indices.size();
    }
    sizeVArray = totalVertices * 11 * sizeof(GLfloat);
    sizeIArray = totalIndices * sizeof(GLuint);

    vArray = (GLfloat*) malloc(sizeVArray);
    indices = (GLuint*) malloc(sizeIArray);

    for(int i = 0; i < objects.size(); i++) {
        objects[i]->prepare(vArray, arrayPos, indices, indicesPos);
    }


    vbo1 = new VBO(vArray, sizeVArray);
    ebo1 = new EBO(indices, sizeIArray);
    
    vao->linkAttrib(*vbo1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0); // pos
    vao->linkAttrib(*vbo1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float))); // color
    vao->linkAttrib(*vbo1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float))); // texture
    vao->linkAttrib(*vbo1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float))); // normal
    
    vao->unbind();
    delete vbo1;
    delete ebo1;

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 proj = glm::mat4(1.0f);

    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
    view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
    proj = glm::perspective(glm::radians(45.0f), (float)game::width / (float)game::height, 0.1f, 100.0f);

    int modelLoc = glGetUniformLocation(this->shader->id, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    int viewLoc = glGetUniformLocation(this->shader->id, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    int projLoc = glGetUniformLocation(this->shader->id, "proj");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

    GLuint uniID = glGetUniformLocation(this->shader->id, "scale");
    // Assigns a value to the uniform; NOTE: Must always be done after activating the Shader Program
    glUniform1f(uniID, 0.5f);
    vao->bind();


    glDrawElements(GL_TRIANGLES, sizeIArray / sizeof(int), GL_UNSIGNED_INT, 0);
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
