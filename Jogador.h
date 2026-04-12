#ifndef JOGADOR_H
#define JOGADOR_H

#include <GL/glut.h>

class Jogador {
public:
    float x;
    float y;
    char lastDirection = 'W';
    
    // armazenah o ID da textura do sprite
    GLuint textureID; 

    // corinthians é gigante
    Jogador(); 

    // da a luz ao jogador
    void draw();
    
    // carrega o sprite
    void loadTexture(const char* filename);
};

#endif