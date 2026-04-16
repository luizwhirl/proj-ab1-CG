#include "Bola.h"
#include <GL/glut.h>
#include <utility>
#include <cmath>

Bola::Bola() {
    x = 0.0f;
    y = 0.0f;
    // inicializacao das variaveis de rastro
    lastX = 0.0f;
    lastY = 0.0f;
}

// implementacao do loadTextures da bola que chama o loadTextures da sua animacao
void Bola::loadTextures() {
    animacao.loadTextures();
}

void Bola::draw() {
    // verifica se houve mudanca significativa na posicao entre esse frame e o anterior
    bool isMoving = (std::abs(x - lastX) > 0.0001f || std::abs(y - lastY) > 0.0001f);

    // a gente usa o draw da classe animacao ao inves de desenhar o quadrado branco aqui
    animacao.draw(x, y, isMoving);

    // satualiza as ultimas posicoes conhecidas
    lastX = x;
    lastY = y;
    
}