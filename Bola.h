#ifndef BOLA_H
#define BOLA_H

class Bola {
public:
    float x;
    float y;
    bool isHeld = false;
    float velX = 0;
    float velY = 0;

    // voce vai ver quando minha mae comprar uma bola quadrada
    Bola(); 

    // faz a bola
    void draw();
};

#endif