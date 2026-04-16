#ifndef BOLA_H
#define BOLA_H

#include "AnimacaoBola.h"

class Bola {
public:
    float x;
    float y;
    
    // variaveis para rastrear a posicao no frame anterior e deduzir se a bola esta se movendo
    float lastX;
    float lastY;

    int statusPosse = 0; // 0 -> livre / 1 -> time aliado / 2-> time rival
    int idRival = -1;

    float velX = 0;
    float velY = 0;
    int framesIntocavel = 0;
    
    // innstancia da classe de animacao da bola
    AnimacaoBola animacao; 

    // voce vai ver quando minha mae comprar uma bola quadrada
    Bola(); 

    // metodo para carregar as texturas da bola
    void loadTextures(); 

    // faz a bola
    void draw();
};

#endif