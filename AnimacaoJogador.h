#ifndef ANIMACAO_JOGADOR_H
#define ANIMACAO_JOGADOR_H

#include <GL/glut.h>

class AnimacaoJogador {
public:
    // armazenah o ID da textura do sprite
    GLuint textureIdle; 
    GLuint textureWalk1;
    GLuint textureWalk2;

    // ids das texturas das costas
    GLuint textureBackIdle;
    GLuint textureBackWalk1;
    GLuint textureBackWalk2;

    // ids das texturas di lado
    GLuint textureSideIdle;
    GLuint textureSideWalk1;
    GLuint textureSideWalk2;

    // variaveis de controle da animacao
    bool isWalking;
    int frameCounter;
    int currentWalkFrame;

    // é o caba que constroi a animacao ese
    AnimacaoJogador();

    // metodo pra carregar todas as texturas usadas pelo jogador
    void loadTextures();
    
    // metodo auxiliar pra carregar uma textura especifica
    GLuint loadTextureSingle(const char* filename);

    // atualiza o estado de movimento da animacao
    void setWalking(bool walking);

    // metodo draw  recebe a direcao para saber qual sprite usar e kwal espelhar
    void draw(float x, float y, char direction);
};

#endif