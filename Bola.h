#ifndef BOLA_H
#define BOLA_H

class Bola {
public:
    float x;
    float y;
    int statusPosse = 0; // 0 -> livre / 1 -> time aliado / 2-> time rival
    int idRival = -1;

    float velX = 0;
    float velY = 0;
    int framesIntocavel = 0;

    // voce vai ver quando minha mae comprar uma bola quadrada
    Bola(); 

    // faz a bola
    void draw();
};

#endif