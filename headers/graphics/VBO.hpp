#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glad/glad.h>

class VBO {
    public:
        GLuint ID;
        VBO(GLfloat* vertices, int size);
        
        void bind();
        void unbind();
        void deleteIt();
    
        ~VBO();
};

#endif
