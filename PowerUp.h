#ifndef POWERUP_H
#define POWERUP_H

class PowerUp {
public:
    float x;
    float y;
    bool active;
    int type; // 1 = vermelho - velocidade
              // 2 = azul - invencibilidade

    PowerUp();
    
    // metodo para spawnar o item em um canto do campo
    void spawn();
    
    // para desenhar o poder na tela
    void draw();
    
    // checa colisão com o jogador
    bool checkCollision(float px, float py);
};

#endif