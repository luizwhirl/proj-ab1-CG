#ifndef JOGADOR_H
#define JOGADOR_H

class Jogador {
public:
    float x;
    float y;
    char lastDirection = 'W';

    // corinthians é gigante
    Jogador(); 

    // da a luz ao jogador
    void draw();
};

#endif