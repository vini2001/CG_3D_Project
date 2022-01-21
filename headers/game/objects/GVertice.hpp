#ifndef GVERTICE_CLASS_H
#define GVERTICE_CLASS_H


#include <glm/glm.hpp>

using namespace std;
typedef glm::vec3 v3;
typedef glm::vec2 v2;

class GVertice {
    public:
        v3 coords, colors;
        v2 texture;
        v3 normal;
        
        GVertice(v3 coords, v3 colors, v2 texture, v3 normal);

     
};
#endif