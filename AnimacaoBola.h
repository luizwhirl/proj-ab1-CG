#ifndef ANIMACAO_BOLA_H
#define ANIMACAO_BOLA_H

#include <GL/glut.h>

// essa bixinha é aclasse responsavel por gerenciar a animacao da bola
class AnimacaoBola {
public:
    GLuint textureFrames[6]; // array: guarda as 6 texturas/frames da bola
    int frameCounter;        // contador de atualizacoes para trocar o frame
    int currentFrame;        // indice da image atual 0-6 (6)

    AnimacaoBola();

    // carrega os 6 png  dah bolah
    void loadTextures();
    
    // metodo auxiliar para carregar uma textura especifica
    GLuint loadTextureSingle(const char* filename);

    // desenha a bola na tela i recebe isMoving para saber se deve alternar as imagens
    void draw(float x, float y, bool isMoving);
};

#endif