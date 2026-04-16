#ifndef JOGADOR_H
#define JOGADOR_H

#include <GL/glut.h>
#include "AnimacaoJogador.h"

class Jogador {
public:
    float x;
    float y;
    char lastDirection = 'W';
    float baseX;
    float baseY;
    
    // objeto de animacao ablablabla
    AnimacaoJogador animacao; 

    // corinthians é gigante
    Jogador(float startX, float startY); 

    // da a luz ao jogador
    void draw();
    
    // carrega o sprite
    // adiciona parametro isRival para decidir qual textura carregar
    void loadTexture(bool isRival = false); 
    
    // metodo pro jogadorsaber qando deve comecar a animacao de andar
    void setAndando(bool andando); 
};

#endif