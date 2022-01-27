
#include "GObject.hpp"
#include <glm/glm.hpp>
typedef glm::vec3 v3;

GObject::GObject() {
    this->vertices = vector<GVertice>();
    this->indices = vector<int>();
}

int GObject::addVertice(GVertice v) {
    vertices.push_back(v);
    return vertices.size() - 1;
}

void GObject::addTriangle(int v1, int v2, int v3) {
    indices.push_back(v1);
    indices.push_back(v2);
    indices.push_back(v3);

    verticesGroup.push_back(vector<int>({v1, v2, v3}));
}

void GObject::translate(v3 translation) {
    for(int i = 0; i < vertices.size(); i++) {
        vertices[i].coords += translation;
    }
}

int GObject::getSize() {
    return vertices.size() * 11;
}

void GObject::animate() {}

void GObject::recalculateNormals() {
    if(!generateNormals) return;
    
    for(int i = 0; i < verticesGroup.size(); i++) {
        v3 ve1 = vertices[verticesGroup[i][0]].coords;
        v3 ve2 = vertices[verticesGroup[i][1]].coords;
        v3 ve3 = vertices[verticesGroup[i][2]].coords;

        v3 normal = glm::normalize(glm::cross(ve2 - ve1, ve3 - ve1));

        vertices[verticesGroup[i][0]].normal += normal;
        vertices[verticesGroup[i][1]].normal += normal;
        vertices[verticesGroup[i][2]].normal += normal;
    }

    for(int i = 0; i < vertices.size(); i++) {
        vertices[i].normal = glm::normalize(vertices[i].normal);
    }
}

void GObject::prepare(GLfloat* vArray, long &arrayPos, GLuint* iArray, long &indicesPos) {

    // Each vertice represents 11 positions on the vArray array. So for each 11 positions, 1 vertice has been added. At this point, we need to know how many vertices have been added to determine the starting ID of this shape's vertices
    int startingId = (int) arrayPos/11;

    for(int i = 0; i < vertices.size(); i++) {
        vArray[arrayPos++] = vertices[i].coords.x;
        vArray[arrayPos++] = vertices[i].coords.y;
        vArray[arrayPos++] = vertices[i].coords.z;
        vArray[arrayPos++] = vertices[i].colors.x;
        vArray[arrayPos++] = vertices[i].colors.y;
        vArray[arrayPos++] = vertices[i].colors.z;
        vArray[arrayPos++] = vertices[i].texture.x;
        vArray[arrayPos++] = vertices[i].texture.y;
        vArray[arrayPos++] = vertices[i].normal.x;
        vArray[arrayPos++] = vertices[i].normal.y;
        vArray[arrayPos++] = vertices[i].normal.z;
    }
    
    for(int i = 0; i < indices.size(); i++) {
        iArray[indicesPos++] = indices[i] + startingId;
    }
}

void GObject::frameUpdate() {
    translate(this->speedVector * this->speed * this->speedMultiplier);
    recalculateNormals();
}

void GObject::scale(v3 scale) {
    for(int i = 0; i < vertices.size(); i++) {
        vertices[i].coords.x *= scale.x;
        vertices[i].coords.y *= scale.y;
        vertices[i].coords.z *= scale.z;
    }
}

void GObject::rotate(v3 degrees) {
    v3 origin = vertices[0].coords;
    for(int i = 0; i < vertices.size(); i++) {
        vertices[i].coords -= origin;
        vertices[i].coords = glm::rotate(vertices[i].coords, glm::radians(degrees.x), v3(1, 0, 0));
        vertices[i].coords = glm::rotate(vertices[i].coords, glm::radians(degrees.y), v3(0, 1, 0));
        vertices[i].coords = glm::rotate(vertices[i].coords, glm::radians(degrees.z), v3(0, 0, 1));
        vertices[i].coords += origin;
    }
}

v3 GObject::getPos() {
    return vertices[0].coords;
}