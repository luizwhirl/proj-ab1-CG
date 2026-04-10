#ifndef BOLA_H
#define BOLA_H

class Bola {
public:
    float x;
    float y;
    bool isHeld = false;

    // voce vai ver quando minha mae comprar uma bola quadrada
    Bola(); 

    // faz a bola
    void draw();
    
    // mexe a bola
    void update(char);
};

#endif