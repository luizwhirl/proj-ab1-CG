#ifndef CAMPO_H
#define CAMPO_H

#include <GL/glut.h>

class Campo {
private:
    // variavel do id da grama random
    GLuint grassTexture;
    GLuint arquibancadaTexture;
    GLuint arquibancadaTextureB;
    GLuint arquibancadaTextureC; 
    GLuint arquibancadaTextureD;

    void drawCircle(float cx, float cy, float r, int num_segments);
    void drawFieldLines();

    // metodos para desenhar os placeholders
    void drawArquibancada();

public:
    Campo();
    
    // gerar nossa graminha randomizada
    void createGrassTexture();
    
    // carregar as arquibancadas
    void loadArquibancadaTextures(const char* filepathA, const char* filepathB, const char* filepathC, const char* filepathD); 
    void draw();

    // metodo para barrar a saida do gramado
    void resolverColisaoLimites(float& posX, float& posY, float raio);
};

#endif