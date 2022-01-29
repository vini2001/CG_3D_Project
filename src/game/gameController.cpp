#include "gameController.hpp"
#include "utils.hpp"
#include <iostream>
#include "GText.hpp"
#include "globals.hpp"
#include <glm/glm.hpp>
#include "vectors.hpp"
#include "GObject.hpp"
#include "boid.hpp"
#include "customObjects.hpp"


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
    GObject *wall = createWall();
    GObject *wall2 = createWall();
    GObject *wall3 = createWall();
    GObject *wall4 = createWall();
    this->plane = createPlane();
    this->sky = createPlane(v3(3, 173, 252), 1.0f);
    wall2->rotate(glm::vec3(0.0f, 270, 0.0f));

    wall4->rotate(glm::vec3(0.0f, 270, 0.0f));
    wall3->translate(glm::vec3(0.0f, 0.0f, worldSize));
    wall4->translate(glm::vec3(worldSize, 0.0f, 0.0f));
    this->sky->translate(glm::vec3(0.0f, worldHeight, 0.0f));

    this->tower = createTower(70.0f, 20.0f);
    this->tower->translate(glm::vec3((worldSize/2.0f), 0.0f, (worldSize/2.0f)));
    this->towerTop = v3(worldSize/2.0f, 70.0f, worldSize/2.0f);

    
    GObject *t2 = createTower(20.0f, 8.0f);
    GObject *t3 = createTower(20.0f, 8.0f);
    GObject *t4 = createTower(20.0f, 8.0f);
    GObject *t5 = createTower(20.0f, 8.0f);
    GObject *t6 = createTower(30.0f, 5.0f);
    t2->translate(glm::vec3(100.0, 0.0f, 50.0f));
    t3->translate(glm::vec3(50.0, 0.0f, 100.0f));
    t4->translate(glm::vec3(200.0, 0.0f, 50.0f));
    t5->translate(glm::vec3(50.0, 0.0f, 200.0f));
    t6->translate(glm::vec3(300.0, 0.0f, 200.0f));
    this->obstacles.push_back(tower);
    this->obstacles.push_back(t2);
    this->obstacles.push_back(t3);
    this->obstacles.push_back(t4);
    this->obstacles.push_back(t5);
    this->obstacles.push_back(t6);

    ghostBoid = new Boid(v3(100.0f, 30.0f, 100.0f), v3(1.0f, 0.0f, 0.0f));
    ghostBoid->opacity = 0.2f;
    ghostBoid->speedMultiplier = 1.06f;
    objectsPlaneText.push_back(ghostBoid);
    ghostBoid->translate(v3(30.0f, rand()%6 - 3, 30.0f));
    ghostBoid->scale(v3(0.5f, 0.5f, 0.5f));
    boids.push_back(dynamic_cast<Boid*>(ghostBoid));

    for(int i = 1; i <= 5; i++) {
        for(int j = 1; j <= 5; j++) {
            GObject *boid = new Boid(v3(20.0f * j, 30.0f, 20.0f * i));
            objectsPlaneText.push_back(boid);
            boid->translate(v3(0.0f, rand()%6 - 3, 0.0f));
            boids.push_back(dynamic_cast<Boid*>(boid));
        }
    }

    // loop boids
    for(int i = 0; i < boids.size(); i++) {
        boids[i]->originalPosition = getBoidGroupCenter() - boids[i]->getPos();
    }

    this->goalBoid = new Boid(v3(0.0f, 30.0f, 0.0f), v3(0.0f, 0.5f, 0.0f));
    goalBoid->opacity = 0.6f;
    this->goalBoid->size = 1.5f; // hack pra arrumar batidas das asas
    goalBoid->scale(v3(5.0f, 5.0f, 5.0f));
    this->goalBoid->translate(v3(worldSize/2.0f, 0.0f, worldSize/2.0f - 80.0f));
    objectsPlaneText.push_back(goalBoid);

    objects.push_back(wall);
    objects.push_back(wall2);
    objects.push_back(wall3);
    objects.push_back(wall4);
    objects.push_back(plane);
    objects.push_back(tower);
    objects.push_back(t2);
    objects.push_back(t3);
    objects.push_back(t4);
    objects.push_back(t5);
    objects.push_back(t6);

    walls.push_back(wall);
    walls.push_back(wall2);
    walls.push_back(wall3);
    walls.push_back(wall4);

    // loop objects
    for(int i = 0; i < objects.size(); i++) {
        objects[i]->recalculateNormals();
    }
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
    v3 lightPos = v3(worldSize/3.0f,  600.0f, worldSize/2.0f);
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

    switch(pressedKey) {
        case GLFW_KEY_R: {
            destroy();
            init(shader);
            break;
        }
        case GLFW_KEY_C: { // bring light to current camera position
            v3 camPos = camera->position;
            printf("Camera position: %f, %f, %f\n", camPos.x, camPos.y, camPos.z);
            v3 lightPos = v3(camera->position.x, camera->position.y, camera->position.z);
            m4 lightModel = m4(1.0f);
            lightModel = glm::translate(lightModel, lightPos);

            this->lightShader->activate();
            glUniformMatrix4fv(glGetUniformLocation(this->lightShader->id, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
            this->shader->activate();
            glUniform3f(glGetUniformLocation(this->shader->id, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
            break;
        }
        case GLFW_KEY_1: { // Camera mode 1, top of the tower
            lockedPositionBehind = false;
            lockedPositionSide = false;
            v3 boidGroupCenter = getBoidGroupCenter();
            camera->position = this->towerTop + v3(0.0f, 2.0f, 0.0f);
            camera->lookAt(boidGroupCenter);
            break;
        }
        case GLFW_KEY_2: { // Camera mode 2, behind boids
            lockedPositionBehind = true;
            lockedPositionSide = false;
            camera->lookAt(getBoidGroupCenter());
            break;
        }
        case GLFW_KEY_3: { // Camera mode 3, side of the boids
            lockedPositionSide = true;
            lockedPositionBehind = false;
            lockedPositionSideCounterClockwise = false;
            break;
        }
        case GLFW_KEY_4: { // Camera mode 4, other side of the boids
            lockedPositionSide = true;
            lockedPositionBehind = false;
            lockedPositionSideCounterClockwise = true;
            break;
        }
        case GLFW_KEY_L: { // Lock or unlock fixed camera orientation pointing to boids
            this->lockedOrientation = !this->lockedOrientation;
            break;
        }
        case GLFW_KEY_0: { // Unlock camera position if fixed to move with the boids
            this->lockedPositionBehind = false;
            this->lockedPositionSide = false;
            break;
        }
        case GLFW_KEY_P: { // Pause
            paused = !paused;
            if(!paused) debugMode = false;
            break;
        }
        case GLFW_KEY_LEFT_BRACKET: { // Debug mode
            if(!debugMode) {
                paused = true;
            }else{
                allowDebugFrames++;
            }
            debugMode = true;
            break;
        }
        case GLFW_KEY_EQUAL: {
            createRandomBoid();
            break;
        }
        case GLFW_KEY_MINUS: {
            deleteRandomBoid();
            break;
        }
        case GLFW_KEY_O: { // Increase goal boid speed
            goalBoid->speed += 0.1f;
            break;
        }
        case GLFW_KEY_I: { // Decrease goal boid speed
            goalBoid->speed -= 0.1f;
            if(goalBoid->speed < 0.0f) {
                goalBoid->speed = 0.0f;
            }
            break;
        }
        case GLFW_KEY_M: { // Increase boid group speed
            for(int i = 0; i < boids.size(); i++) {
                boids[i]->speed += 0.1f;
            }
            break;
        }
        case GLFW_KEY_N: { // Decrease boid group speed
            for(int i = 0; i < boids.size(); i++) {
                boids[i]->speed -= 0.1f;
                if(boids[i]->speed < 0.0f) {
                    boids[i]->speed = 0.0f;
                }
            }
            break;
        }
    }
    if(!game::started) return;
}

void GameController::resizeScreen() {
    delete gText;
    gText = new GText();

    camera->width = game::width;
    camera->height = game::height;
}


float rotation = 0.0f;
double prevTime = glfwGetTime();

bool acSpVSet = false;
v3 acSpVector = v3(0.0f, 0.0f, 0.0f);
void GameController::drawElements() {

    if(this->lockedOrientation) {
        // camera->lookAt(getBoidGroupCenter());
        camera->lookAt(boids[0]->getPos());
    }

    // Lock camera position behind boids 
    if(lockedPositionBehind || lockedPositionSide) {
        v3 boidGroupCenter = boids[0]->getPos();
        v3 spVec = glm::normalize(boids[0]->speedVector);

        if(lockedPositionSide) {
            if(lockedPositionSideCounterClockwise) {
                spVec = v3(-spVec.z, spVec.y, spVec.x);
            }else{
                spVec = v3(spVec.z, spVec.y, -spVec.x);
            }
        }

        if(!acSpVSet) {
            acSpVector = spVec;
            acSpVSet = true;
        }else{
            float sp = boids[0]->speed;
            float smoothFactor = 25.0;
            float changeRatio = 1.0 - (1.0 - (1.0 / (smoothFactor * (sp + 1.0)) ));
            acSpVector = acSpVector * (1-changeRatio) + spVec * changeRatio;
        }

        v3 boidsBack = boidGroupCenter - acSpVector * 55.0f;
        camera->position = boidsBack;
    }

    camera->inputs(game::window);
    camera->updateMatrix(45.0f, 0.1f, 1000.0f);

    this->shader->activate();
    glUniform3f(glGetUniformLocation(this->shader->id, "camPos"), camera->position.x, camera->position.y, camera->position.z);
    camera->matrix(this->shader, "camMatrix");

    if(vao != NULL) delete vao;

    followGoal();
    checkForObstacles();
    checkForWalls();
    
    if(!paused || allowDebugFrames > 0) {
        for(int i = 0; i < boids.size(); i++) {
            boids[i]->animate();
            boids[i]->frameUpdate();
        }

        goalBoid->animate();
        goalBoid->frameUpdate();

        if(allowDebugFrames > 0) {
            printDebug();
            allowDebugFrames--;
        }
    }
    
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

    if(paused) {
        drawText(debugMode ? "DEBUG" : "PAUSED", game::width / 2 - 80, game::height / 2, 1.0f, v3(1.0f, 1.0f, 1.0f));
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
        if(obj->destroyed) continue;
        totalVertices += obj->vertices.size();
        totalIndices += obj->indices.size();
    }
    sizeVArray = totalVertices * 12 * sizeof(GLfloat);
    sizeIArray = totalIndices * sizeof(GLuint);

    vArray = (GLfloat*) malloc(sizeVArray);
    indices = (GLuint*) malloc(sizeIArray);

    for(int i = 0; i < objects.size(); i++) {
        objects[i]->prepare(vArray, arrayPos, indices, indicesPos);
    }


    vbo1 = new VBO(vArray, sizeVArray);
    ebo1 = new EBO(indices, sizeIArray);
    
    vao->linkAttrib(*vbo1, 0, 3, GL_FLOAT, 12 * sizeof(float), (void*)0); // pos
    vao->linkAttrib(*vbo1, 1, 4, GL_FLOAT, 12 * sizeof(float), (void*)(3 * sizeof(float))); // color
    vao->linkAttrib(*vbo1, 2, 2, GL_FLOAT, 12 * sizeof(float), (void*)(7 * sizeof(float))); // texture
    vao->linkAttrib(*vbo1, 3, 3, GL_FLOAT, 12 * sizeof(float), (void*)(9 * sizeof(float))); // normal
    
    vao->unbind();
    delete vbo1;
    delete ebo1;

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 proj = glm::mat4(1.0f);

    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
    view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
    proj = glm::perspective(glm::radians(45.0f), ((float)game::width) / ((float)game::height), 0.1f, 100.0f);

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


v3 GameController::getBoidGroupCenter() {
    // loop boids and get average pos
    v3 sum = v3(0.0f, 0.0f, 0.0f);
    for(int i = 0; i < boids.size(); i++) {
        sum += boids[i]->getPos();
    }
    sum.x /= boids.size();
    sum.y /= boids.size();
    sum.z /= boids.size();
    return sum;
}

bool GameController::isValidBoidPosition(v3 pos) {
    if(pos.x < 5.0f || pos.x > worldSize - 5.0f) return false;
    if(pos.z < 5.0f || pos.z > worldSize - 5.0f) return false;
    if(pos.y < 5.0f || pos.y > worldHeight - 5.0f) return false;

    // avoid boids colisions
    for(int i = 0; i < boids.size(); i++) {
        GLfloat dist = glm::distance(pos, boids[i]->getPos());
        if(dist < 2.0f) {
            return false;
        }
    }

    return true;
}

void GameController::createRandomBoid() {
    int distance = 25;
    v3 boidsCenter = getBoidGroupCenter();
    v3 randomPos;

    int count = 0;
    GObject *boid;
    do {
        boid = new Boid(v3(0.0f, 0.0f, 0.0f));
        boid->translate(v3((rand() % distance)-distance/2 + boidsCenter.x, (rand() % 20) - 5 + boidsCenter.y, (rand() % distance)-distance/2 + boidsCenter.z));
        count++;
    }while(!isValidBoidPosition(boid->getPos()) && count < 10);
    if(count >= 10) return;
    // GObject *boid = new Boid(v3(0.0f, 0.0f, 0.0f));
    objectsPlaneText.push_back(boid);

    Boid *boidD = dynamic_cast<Boid*>(boid);
    boidD->syncWith(boids[0]);

    // boidD->rotateBoid(boids[0]->rotated);
    boid->translate(randomPos);
    boids.push_back(boidD);
}

void GameController::deleteRandomBoid() {
    // delete random boid
    if(boids.size() > 2) {
        int index = (rand() % (boids.size()-1)) + 1;
        boids[index]->destroy();
        boids.erase(boids.begin() + index);
    }
}

bool toTheLeft(v3 a, v3 b, v3 c) {
    return ((b.x - a.x) * (c.z - a.z) - (b.z - a.z) * (c.x - a.x)) > 0;
}

void GameController::checkForWalls() {
    v3 boidsSpeed = glm::normalize(goalBoid->speedVector);
    v3 boidsPos = goalBoid->getPos();

    bool set = false;

    for(int i = 0; i < this->walls.size(); i++) {
      
        v3 p1 = this->walls[i]->vertices[0].coords;
        v3 p2 = this->walls[i]->vertices[1].coords;
        v3 p3 = this->walls[i]->vertices[2].coords;
        // get plane based on 3 points
        v3 normal = glm::normalize(glm::cross(p2 - p1, p3 - p1));
        

        // get distance to closest vertice on the wall
        GLfloat distanceToWall = abs(glm::dot(boidsPos - p1, normal));
        v3 closestPoint = boidsPos - distanceToWall * normal;


        v3 newBoidPos = boidsPos + boidsSpeed;

        // if distance to a arbitrary point got smaller, this is the wall the boids are going into
        float intoWallMove = distanceToWall - abs(glm::dot(newBoidPos - p1, normal)) > 0;
        // printf("intoWallMove: %f\n", intoWallMove);

        if(intoWallMove) {
            if(distanceToWall < 60) {
                bool isToTheLeft = toTheLeft(goalBoid->getPos(), p1 + goalBoid->speedVector, closestPoint);

                goalBoid->rotating = true;
                goalBoid->setRotatingNeg(!isToTheLeft);
                set = true;
            }
        }
    }

    // add some randomness to the rotation total movement
    if(!set) {
        goalBoid->framesToDisableRotation = rand() % (int)(((float)10)/goalBoid->speed);
    }

}

bool GameController::goingToHitTower(Boid *boidTest, GObject *tower) {

    v3 boidTestPos = boidTest->getPos();
    float yPos = boidTestPos.y;
    boidTestPos.y = 0.0f;
    Tower *realTower = dynamic_cast<Tower*>(tower);
    float realRadius = realTower->height < yPos ? 0.0f : realTower->radius * ((realTower->height - yPos) / realTower->height);

    v3 nextboidTestPos = boidTestPos + boidTest->speedVector; nextboidTestPos.y = 0.0f;
    v3 towerPos = tower->getPos(); towerPos.y = 0.0f;


    // get distance to tower
    GLfloat distanceToTower = glm::distance(boidTestPos, towerPos) - realRadius;
    GLfloat nextDistanceToTower = glm::distance(nextboidTestPos, towerPos) - realRadius;

    
    // if not going to hit tower stop
    if(!(distanceToTower < 15.0f)) {
        return false;
    }

    boidTest->rotating = true;
    
    v3 vec1 = boidTest->speedVector;
    v3 vec2 = towerPos - boidTestPos;

    bool isToTheLeft = glm::dot(glm::cross(vec1, vec2), glm::normalize(boidTest->getPos())) < 0;
    if(isToTheLeft) {
        boidTest->setRotatingNeg(false);
    }else{
        boidTest->setRotatingNeg(true);
    }
    return true;
}

void GameController::checkForObstacles() {

    bool obstacleFound = false;
    for(int j = 0; j < obstacles.size(); j++){
        for(int i = 0; i < boids.size(); i++) {
            obstacleFound |= goingToHitTower(boids[i], obstacles[j]);
        }
    }

    // loop every boid (except for the first => ghost boid)
    for(int i = 1; i < boids.size(); i++) {
        // loop every boid
        for(int j = i+1; j < boids.size(); j++) {
            // if same boid, continue
            if(i == j) continue;
            // get distance to other boid
            GLfloat distance = glm::distance(boids[i]->getPos(), boids[j]->getPos());
            // if distance is small, try to avoid
            if(distance < 5.0f && !obstacleFound) {
                boids[i]->rotating = false;
                break;
            }
        }
    }
}

void GameController::followGoal() {
    
    for(int i = 0; i < boids.size(); i++) {
        // follow goal
        v3 goalPos = i == 0 ? goalBoid->getPos() : ghostBoid->getPos();
        v3 vec1 = boids[i]->speedVector;
        v3 vec2 = goalPos - boids[i]->getPos() ;

        // get angle between boid and goal
        // get angle to rotate vec1 to vec 2, between -180 and 180
        GLfloat angle = glm::degrees(glm::acos(glm::dot(vec1, vec2) / (glm::length(vec1) * glm::length(vec2))));
        
        v3 a = vec1;
        v3 b = vec2;

        bool isToTheLeft = toTheLeft(boids[i]->getPos(), boids[i]->getPos() + boids[i]->speedVector, goalPos);
    
        // don't rotate if angle to goal is small (Avoid back gamera glitching)
        boids[i]->rotating = angle > 1.0;
        boids[i]->setRotatingNeg(isToTheLeft);

        if(i > 0) {
            v3 center = getBoidGroupCenter();
            float dist = glm::distance(center, boids[i]->getPos());
            if(dist < 10.0f) {
                boids[i]->speedMultiplier = 1.0f;
            }else{
                boids[i]->speedMultiplier = 1.0f + dist/200.0f;
            }
        }
    }
}

void GameController::printDebug() {
    // PRINT COORDS of all boids, ghostboid and goalboid
    for(int i = 1; i < boids.size(); i++) {
        printf("Boid %d: %f %f %f\n", i, boids[i]->getPos().x, boids[i]->getPos().y, boids[i]->getPos().z);
    }
    printf("GhostBoid (Boids Group invisible guide): %f %f %f\n", ghostBoid->getPos().x, ghostBoid->getPos().y, ghostBoid->getPos().z);
    printf("GoalBoid (Boids Group goal): %f %f %f\n", goalBoid->getPos().x, goalBoid->getPos().y, goalBoid->getPos().z);
}