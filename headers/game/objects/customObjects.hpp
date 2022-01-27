#ifndef customObjects_h
#define customObjects_h
    GObject* createPyramid();
    GObject* createWall();
    GObject* createPlane();
    GObject* createPlane(v3 color, float textureScale);
    GObject* createTower(GLfloat height, GLfloat width);
#endif