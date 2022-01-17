#include "VAO.hpp"
#include <iostream>
#include "globals.hpp"

// Constructor that generates a VAO ID
VAO::VAO(){
    glGenVertexArrays(1, &ID);
}

// Links a VBO to the VAO using a certain layout
void VAO::linkAttrib(VBO& vbo, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset){
    vbo.bind();
    glVertexAttribPointer(layout, numComponents, type, GL_FALSE, (int)stride, offset);
    glEnableVertexAttribArray(layout);
    vbo.unbind();
}

// Binds the VAO
void VAO::bind(){
    glBindVertexArray(ID);
}

// Unbinds the VAO
void VAO::unbind(){
    glBindVertexArray(0);
}

// Deletes the VAO
void VAO::deleteIt(){
    glDeleteVertexArrays(1, &ID);
}

void VAO::destroy() {
    if(debugLevel == DEBUG_ALL) std::cout << "VAO destroy" << std::endl;
    unbind();
    deleteIt();
}

VAO::~VAO() {
    destroy();
}
