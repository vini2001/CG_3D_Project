#ifndef GText_hpp
#define GText_hpp

#include <stdio.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <iostream>
#include <string>
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "shaderClass.hpp"

using namespace std;


struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
    unsigned int VAO, VBO;
};

class GText{
public:
    void fontsLoad();
    void doSomething();
    void renderText(string text, float x, float y, float scale, glm::vec3 color);
    GText();
    ~GText();
    
    unsigned int VAO, VBO;
    Shader textShader;
};

#endif /* GText_hpp */
