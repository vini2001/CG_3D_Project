#ifndef GOBJECT_CLASS_H
#define GOBJECT_CLASS_H


#include <glm/glm.hpp>
#include <vector>
#include "GVertice.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>


using namespace std;
typedef glm::vec3 v3;
typedef glm::vec2 v2;

class GObject {
    public:
        vector<GVertice> vertices;
        vector<int> indices;

        int addVertice(GVertice v);
        void addTriangle(int v1, int v2, int v3);

        void translate(v3 translation);

        int getSize();

        void prepare(GLfloat *array, long &arrayPos, GLuint *indices, long &indicesPos);
        void scale(v3 scale);
        void rotate(v3 rotation);

        GObject();

     
};

#endif