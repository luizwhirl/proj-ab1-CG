#ifndef CAMPO_H
#define CAMPO_H

#include <GL/glut.h>

class Campo {
private:
    // variavel do id da grama random
    GLuint grassTexture;

    void drawCircle(float cx, float cy, float r, int num_segments);
    void drawFieldLines();

    // metodos para desenhar os placeholders e as traves
    void drawGoals();

public:
    Campo();
    
    // gerar nossa graminha randomizada
    void createGrassTexture();
    void draw();

    // metodo para tratar a colisao da bola com os gols
    void tratarColisaoGol(float& bolaX, float& bolaY, float& velX, float& velY, float raioBola);
};

#endif