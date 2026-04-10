#ifndef GOL_H
#define GOL_H

class Gol {
public:
    Gol();
    
    // desenha as traves com um projeçãozinha pra PARECER que é um negocio diferentaooo slkk
    void draw();
    
    // metodo - checar colisao barrando posições diretamente
    void resolverColisao(float& posX, float& posY, float raio);
};

#endif