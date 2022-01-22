#include "GVertice.hpp"
#include "GObject.hpp"


GObject* createPyramid() {

    // Vertices coordinates
    vector<GLfloat> verticesP({ 
        //     COORDINATES     /        COLORS          /    TexCoord   /        NORMALS       //
        -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side
        -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
        0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
        0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side

        -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
        -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
        0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,     -0.8f, 0.5f,  0.0f, // Left Side

        -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
        0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
        0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f, -0.8f, // Non-facing side

        0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
        0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
        0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.8f, 0.5f,  0.0f, // Right side

        0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
        -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
        0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f,  0.8f  // Facing side
    });

    // Indices for vertices order
    vector<GLuint> indicesP({ 
        0, 1, 2, // Bottom side
        0, 2, 3, // Bottom side
        4, 6, 5, // Left side
        7, 9, 8, // Non-facing side
        10, 12, 11, // Right side
        13, 15, 14 // Facing side
    });

    GObject *pyramid = new GObject();
    
    for(int i = 0; i < verticesP.size(); i += 11) {
        v3 coords = v3(verticesP[i], verticesP[i+1], verticesP[i+2]);
        v3 colors = v3(verticesP[i+3], verticesP[i+4], verticesP[i+5]);
        v2 texture = v2(verticesP[i+6], verticesP[i+7]);
        v3 normal = v3(verticesP[i+8], verticesP[i+9], verticesP[i+10]);
        GVertice v(coords, colors, texture, normal);
        pyramid->addVertice(v);
    }
    

    // Create indices
    for(int i = 0; i < indicesP.size(); i += 3) {
        pyramid->addTriangle(indicesP[i], indicesP[i+1], indicesP[i+2]);
    }
    return pyramid;
}

GObject* createBoid(v3 translation) {
    GObject* boid = new GObject();

    float heightWings = 1.5f;
    float headLength = 2.0f;
    float backWidth = 2.0f;
    float tailLength = 1.7f;

    v3 wingsColor = v3(.419f, 0.519f, 0.919f);
    v3 trunkColor = v3(0.2f, 0.2f, 0.2f);
    v3 backColor = v3(0.04f, 0.04f, 0.04f);

    // LEFT WING
    int leftMi = boid->addVertice(GVertice(v3(0.0f, 0.0f, 0.0f), wingsColor, v2(0.0f, 0.0f)));
    int v2i = boid->addVertice(GVertice(v3(3.0f, -heightWings/2, 1.5f), trunkColor, v2(5.0f, 0.0f)));
    int v3i = boid->addVertice(GVertice(v3(2.6f, -heightWings/2, -1.5f), trunkColor, v2(5.0f, 0.0f)));
    int v4i = boid->addVertice(GVertice(v3(3.0f, heightWings/2, 1.5f), trunkColor, v2(5.0f, 0.0f)));
    int v5i = boid->addVertice(GVertice(v3(2.6f, heightWings/2, -1.5f), trunkColor, v2(5.0f, 0.0f)));
    
    boid->addTriangle(leftMi, v2i, v3i);
    boid->addTriangle(leftMi, v4i, v5i);
    boid->addTriangle(leftMi, v2i, v4i);
    boid->addTriangle(leftMi, v3i, v5i);

    //TRUNK
    int trli = boid->addVertice(GVertice(v3(3.4f, -heightWings/2, -1.5f), trunkColor, v2(5.0f, 0.0f)));
    int trhi = boid->addVertice(GVertice(v3(3.4f, heightWings/2, -1.5f), trunkColor, v2(5.0f, 0.0f)));
    boid->addTriangle(v2i, v3i, trli);
    boid->addTriangle(v4i, v5i, trhi);

    //HEAD
    int headI = boid->addVertice(GVertice(v3(3.0f, 0.0f, -1.5 - headLength), v3(0.52f, 0.12f, 0.2f), v2(5.0f, 0.0f)));
    boid->addTriangle(v5i, trhi, headI);
    boid->addTriangle(v3i, trli, headI);
    boid->addTriangle(v3i, v5i, headI);
    boid->addTriangle(trli, trhi, headI);

    //RIGHT WING
    int righMi = boid->addVertice(GVertice(v3(6.0f, 0.0f, 0.0f), wingsColor, v2(0.0f, 0.0f)));
    boid->addTriangle(righMi, v2i, trli);
    boid->addTriangle(righMi, v4i, trhi);
    boid->addTriangle(righMi, v2i, v4i);
    boid->addTriangle(righMi, trli, trhi);

    //TAIL
    int tailL = boid->addVertice(GVertice(v3(3.0f - backWidth/2, 0, 1.5f + tailLength), backColor, v2(0.0f, 0.0f)));
    int tailR = boid->addVertice(GVertice(v3(3.0f + backWidth/2, 0, 1.5f + tailLength), backColor, v2(0.0f, 0.0f)));
    boid->addTriangle(tailL, tailR, v4i);
    boid->addTriangle(tailL, tailR, v2i);
    boid->addTriangle(tailR, v2i, v4i);
    boid->addTriangle(tailL, v2i, v4i);


    boid->rotate(v3(0.0f, 180.0f, 0.0f));
    boid->translate(translation);
    boid->scale(v3(0.2f, 0.2f, 0.2f));

    return boid;
}


GObject* createTower() {

    GLfloat radius = 20.0f;
    GLfloat height = 70.0f;
    GLfloat circunference = 2.0f * M_PI * radius;
    int segments = 200;
    GLfloat segmentWidth = circunference / segments;

    v3 towerColor = v3(0.219f, 0.219f, 0.219f);

    GObject* tower = new GObject();
    GVertice  verCenter = GVertice(v3(0.0f, 0.0f, 0.0f), towerColor, v2(0.5f, 0.5f));
    GVertice  verTop = GVertice(v3(0.0f, height, 0.0f), towerColor + v3(0.5f, 0.5f, 0.5f), v2(4.0f, 4.0f));
    int vci = tower->addVertice(verCenter);
    int vti = tower->addVertice(verTop);

    float x = radius;
    float y = 0.0f;
    GVertice  ver1 = GVertice(v3(x, 0.0f, y), towerColor - v3(0.1f, 0.1f, 0.1f), v2(0.0f, 0.0f));
    int leftMi = tower->addVertice(ver1);

    float angle = 1.0f;
    while(true) {
        towerColor +=  angle > 180 ? -v3(0.002f, 0.002f, 0.002f) : v3(0.002f, 0.002f, 0.002f);
        if(angle > 360.0f) angle = 360.0f;

        float angleRadians = glm::radians(angle + 0.0f);
        x = radius * cos(angleRadians);
        y = radius * sin(angleRadians);
        GVertice  ver2 = GVertice(v3(x, 0.0f, y), towerColor - v3(0.1f, 0.1f, 0.1f), ver1.texture + v2(0.0f, angle > 180 ? -0.01f : 0.01f));
        int v2i = tower->addVertice(ver2);

        tower->addTriangle(vci, leftMi, v2i);
        tower->addTriangle(vti, leftMi, v2i);

        leftMi = v2i;
        ver1 = ver2;

        if(angle >= 360.0f) break;

        angle += (360/segments);
    }
        
    return tower;
}

GObject* createWall() {
    GObject* wall = new GObject();

    GVertice ver1 = GVertice(v3(0.0f, 0.0f, 0.0f), v3(0.219f, 0.219f, 0.219f), v2(0.0f, 0.0f));
    GVertice ver2 = GVertice(v3(0.0f, 1.0f, 0.0f), v3(0.219f, 0.219f, 0.219f), v2(5.0f, 0.0f));

    int leftMi = wall->addVertice(ver1);
    int v2i = wall->addVertice(ver2);
    int v3i, v4i;

    for(int i = 1; i < 25; i++) {
        GVertice ver3 = GVertice(v3(i*1.0f, 0.0f, 0.0f), v3(0.219f, 0.219f, 0.219f), i%2 == 1 ? v2(5.0f, 5.0f) : v2(0.0f, 0.0f));
        GVertice ver4 = GVertice(v3(i*1.0f, 1.0f, 0.0f), v3(0.219f, 0.219f, 0.219f),  i%2 == 1 ? v2(0.0f, 5.0f) : v2(5.0f, 0.0f));

        v3i = wall->addVertice(ver3);
        v4i = wall->addVertice(ver4);

        wall->addTriangle(leftMi, v2i, v3i);
        wall->addTriangle(v2i, v3i, v4i);

        leftMi = v3i;
        v2i = v4i;
        ver1 = ver3;
        ver2 = ver4;
    }


    // for(int i = 0; i < wall->vertices.size(); i++) {
    //     printf("%.2f, %.2f, %.2f\n", wall->vertices[i].coords.x, wall->vertices[i].coords.y, wall->vertices[i].coords.z);
    // }
    
    wall->scale(v3(16.0f, 300.0f, 1.0f));

    // for(int i = 0; i < wall->vertices.size(); i++) {
    //     printf("%.2f, %.2f, %.2f\n", wall->vertices[i].coords.x, wall->vertices[i].coords.y, wall->vertices[i].coords.z);
    // }

    return wall;
}


GObject* createPlane(v3 color, float textureScale) {
    color /= 255.0f;
    GObject* wall = new GObject();

    GVertice ver1 = GVertice(v3(0.0f, 0.0f, 0.0f), color, v2(0.0f, 0.0f));
    GVertice ver2 = GVertice(v3(0.0f, 0.0f, 384.0f), color, v2(5.0f, 0.0f));

    int leftMi = wall->addVertice(ver1);
    int v2i = wall->addVertice(ver2);
    int v3i, v4i;

    for(int i = 1; i < 25; i++) {
        GVertice ver3 = GVertice(v3(i*1.0f, 0.0f, 0.0f), color, textureScale * (i%2 == 1 ? v2(5.0f, 5.0f) : v2(0.0f, 0.0f)));
        GVertice ver4 = GVertice(v3(i*1.0f, 0.0f, 384.0f), color,  textureScale * (i%2 == 1 ? v2(0.0f, 5.0f) : v2(5.0f, 0.0f)));

        v3i = wall->addVertice(ver3);
        v4i = wall->addVertice(ver4);

        wall->addTriangle(leftMi, v2i, v3i);
        wall->addTriangle(v2i, v3i, v4i);

        leftMi = v3i;
        v2i = v4i;
        ver1 = ver3;
        ver2 = ver4;
    }

    wall->scale(v3(16.0f, 100.0f, 1.0f));
    return wall;
}



GObject* createPlane() {
  return createPlane(v3(157, 163, 165), 1);
}

