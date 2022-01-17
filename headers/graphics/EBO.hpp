#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include <glad/glad.h>

class EBO {
    public:
        GLuint ID;
        EBO(GLuint* vertices, int size);
        
        void bind();
        void unbind();
        void deleteIt();
    
        ~EBO();
};

#endif
