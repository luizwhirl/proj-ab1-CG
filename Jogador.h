#ifndef JOGADOR_H
#define JOGADOR_H

#include <GL/glut.h>
#include "AnimacaoJogador.h"

class Jogador {
public:
    float x;
    float y;
    char lastDirection = 'W';
    
    // objeto de animacao ablablabla
    AnimacaoJogador animacao; 

    // corinthians é gigante
    Jogador(); 

    // da a luz ao jogador
    void draw();
    
    // carrega o sprite
    void loadTexture(); 
    
    // metodo pro jogadorsaber qando deve comecar a animacao de andar
    void setAndando(bool andando); 
};

#endif