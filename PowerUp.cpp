#include "PowerUp.h"
#include <GL/glut.h>
#include <cstdlib>
#include <cmath>

PowerUp::PowerUp() {
    active = false;
    x = 0.0f;
    y = 0.0f;
    type = 1;
}

void PowerUp::spawn() {
    active = true;
    
    // escolhe aleatoriamente o tipo: 1 ou 2
    type = (rand() % 2) + 1;

    // sorteia um dos 4 extremos do campo
    int canto = rand() % 4;
    float limiteX = 2.4f; // borda X aprox
    float limiteY = 4.4f; // borda Y aprox
    
    if (canto == 0) { x = -limiteX; y = -limiteY; }
    else if (canto == 1) { x = limiteX; y = -limiteY; }
    else if (canto == 2) { x = -limiteX; y = limiteY; }
    else { x = limiteX; y = limiteY; }
}

void PowerUp::draw() {
    if (!active) return;

    glPushMatrix();
    glTranslatef(x, y, 0.0f);

    if (type == 1) {
        glColor3f(1.0f, 0.0f, 0.0f); // vemlho
    } else if (type == 2) {
        glColor3f(0.0f, 0.0f, 1.0f); // azu
    }

    glBegin(GL_QUADS);
        glVertex2f(-0.15f, -0.15f);
        glVertex2f( 0.15f, -0.15f);
        glVertex2f( 0.15f,  0.15f);
        glVertex2f(-0.15f,  0.15f);
    glEnd();

    // reseta cor sinao fode as textura do jogo
    glColor3f(1.0f, 1.0f, 1.0f);

    glPopMatrix();
}

bool PowerUp::checkCollision(float px, float py) {
    if (!active) return false;
    
    // cálculo de distância
    float distancia = std::sqrt(std::pow(px - x, 2) + std::pow(py - y, 2));
    if (distancia < 0.35f) { 
        // raio do jogador (0.2) + poder (0.15)
        return true;
    }
    return false;
}