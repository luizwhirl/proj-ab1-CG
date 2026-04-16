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

    // constante para o numero de variacoes de torcedores 
    // azul, ray, manu, luca, indo, rosa
    static const int NUM_TORCEDORES = 6;

    // ids das texturas dos torcedores - arrays para suportar os 6 tipos
    GLuint torcedorNorteIdleTexture[NUM_TORCEDORES];
    GLuint torcedorNorteComemoraTexture[NUM_TORCEDORES];
    GLuint torcedorSulIdleTexture[NUM_TORCEDORES];
    GLuint torcedorSulComemoraTexture[NUM_TORCEDORES];
    GLuint torcedorOesteIdleTexture[NUM_TORCEDORES];
    GLuint torcedorOesteComemoraTexture[NUM_TORCEDORES];

    void drawCircle(float cx, float cy, float r, int num_segments);
    void drawFieldLines();

    // metodos para desenhar os placeholders
    void drawArquibancada(bool isComemorando);

public:
    Campo();
    
    // gerar nossa graminha randomizada
    void createGrassTexture();
    
    // carregar as arquibancadas
    void loadArquibancadaTextures(const char* filepathA, const char* filepathB, const char* filepathC, const char* filepathD); 
    
    // função para carregar os sprites dos torcedores (separando norte, sul e oeste/leste)
    void loadTorcedorTextures(const char* idleNortePath, const char* comemoraNortePath, const char* idleSulPath, const char* comemoraSulPath, const char* idleOestePath, const char* comemoraOestePath);

    // parâmetro isComemorando, com valor padrão false
    void draw(bool isComemorando = false);

    // metodo para barrar a saida do gramado
    void resolverColisaoLimites(float& posX, float& posY, float raio);
};

#endif