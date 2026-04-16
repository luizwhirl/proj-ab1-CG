#ifndef POWERUP_H
#define POWERUP_H

#include <GL/glut.h>

class PowerUp {
public:
    float x;
    float y;
    bool active;
    int type; // 1 = vermelho - velocidade
              // 2 = azul - invencibilidade

    // ids pra armazenar as texturas carregadas
    GLuint texBota;
    GLuint texEscudo;

    PowerUp();

    // método para inicializar icarregar as imagens das texturas
    void loadTextures();
    
    // metodo para spawnar o item em um canto do campo
    void spawn();
    
    // pra desenhar o poder na tela
    void draw();
    
    // checa colisão com o jogador
    bool checkCollision(float px, float py);
};

#endif