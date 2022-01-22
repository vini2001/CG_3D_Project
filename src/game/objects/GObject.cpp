
#include "GObject.hpp"
#include <glm/glm.hpp>

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

void GObject::scale(v3 scale) {
    for(int i = 0; i < vertices.size(); i++) {
        vertices[i].coords.x *= scale.x;
        vertices[i].coords.y *= scale.y;
        vertices[i].coords.z *= scale.z;
    }
}

void GObject::rotate(v3 degrees) {
    for(int i = 0; i < vertices.size(); i++) {
        vertices[i].coords = glm::rotate(vertices[i].coords, glm::radians(degrees.x), v3(1, 0, 0));
        vertices[i].coords = glm::rotate(vertices[i].coords, glm::radians(degrees.y), v3(0, 1, 0));
        vertices[i].coords = glm::rotate(vertices[i].coords, glm::radians(degrees.z), v3(0, 0, 1));
    }
}