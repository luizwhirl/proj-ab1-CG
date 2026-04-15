#include "Jogador.h"
#include <GL/glut.h>
#include <utility>
#include <iostream>

Jogador::Jogador(float startX, float startY) {
    x = startX;
    y = startY;
    baseX = startX;
    baseY = startY;
}
// pede pra classe de animacao carregar todas as texturas
// repassa parametro isRival para a classe animacao
void Jogador::loadTexture(bool isRival) {
    animacao.loadTextures(isRival);
}

// repassa o estado de movimento pra animacao
void Jogador::setAndando(bool andando) {
    animacao.setWalking(andando);
}

// da a luz ao jogador
void Jogador::draw() {
    // tamo passano lastDirection pra animação saber pra onde oliar
    animacao.draw(this->x, this->y, lastDirection);
}