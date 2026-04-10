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
    void drawArquibancadas();
    void drawGoals();

public:
    Campo();
    
    // gerar nossa graminha randomizada
    void createGrassTexture();
    void draw();
};

#endif