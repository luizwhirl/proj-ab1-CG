#include "Jogador.h"
#include <GL/glut.h>
#include <utility>
#include <iostream>

Jogador::Jogador() {
    x = 1.0f;
    y = 1.0f;
}

// pede pra classe de animacao carregar todas as texturas
void Jogador::loadTexture() {
    animacao.loadTextures();
}

// repassa o estado de movimento pra animacao
void Jogador::setAndando(bool andando) {
    animacao.setWalking(andando);
}

// da a luz ao jogador
void Jogador::draw() {
    // tamo passano lastDirection pra animação saber pra onde oliar
    animacao.draw(x, y, lastDirection);
}