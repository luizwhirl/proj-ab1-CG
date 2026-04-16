#ifndef ANIMACAO_GOLEIRO_H
#define ANIMACAO_GOLEIRO_H

#include <GL/glut.h>

// classe para gerenciar a animação dos sprites do goleiro
class AnimacaoGoleiro {
public:
    GLuint textureIdle; 
    GLuint textureWalk1;
    GLuint textureWalk2;

    bool isWalking;
    int frameCounter;
    int currentWalkFrame;

    AnimacaoGoleiro();

    // recebe isTop para saber qual sprite carregar
    void loadTextures(bool isTop);
    
    GLuint loadTextureSingle(const char* filename);

    void setWalking(bool walking);

    //  recebe as dimensões para aplicar na textura
    void draw(float x, float y, float largura, float altura);
};

#endif