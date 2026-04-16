#include "PowerUp.h"
#include <GL/glut.h>
#include <cstdlib>
#include <cmath>

#include "stb_image.h"
#include <iostream>

PowerUp::PowerUp() {
    active = false;
    x = 0.0f;
    y = 0.0f;
    type = 1;
    // textura iniciada em texturas em 0
    texBota = 0;
    texEscudo = 0;
}

// func completa para carregar os png na memória da placa de vídeo
void PowerUp::loadTextures() {
    int width, height, channels;
    unsigned char* data;

    // carreca bota.png
    data = stbi_load("assets/sprites/itens/bota.png", &width, &height, &channels, 4);
    if (data) {
        glGenTextures(1, &texBota);
        glBindTexture(GL_TEXTURE_2D, texBota);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);
    } else {
        std::cout << "Falha ao carregar bota.png" << std::endl;
    }

    // carrega escudo.png
    data = stbi_load("assets/sprites/itens/escudo.png", &width, &height, &channels, 4);
    if (data) {
        glGenTextures(1, &texEscudo);
        glBindTexture(GL_TEXTURE_2D, texEscudo);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);
    } else {
        std::cout << "Falha ao carregar escudo.png" << std::endl;
    }
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

    // habilia textura 2D e o Alpha Blending para o fundo do png ficar transparente
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // define a cor base como branco para manter as cores reais das texturas do pnggg
    glColor3f(1.0f, 1.0f, 1.0f);

    if (type == 1) {
        glBindTexture(GL_TEXTURE_2D, texBota); // define a textura ativa para a bota
    } else if (type == 2) {
        glBindTexture(GL_TEXTURE_2D, texEscudo); // define a textura ativa para o escudo
    }

    glBegin(GL_QUADS);
        // glTexCoord2f foram adicionados antes de cada vértice para esticar a textura no quad
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-0.15f, -0.15f);
        glTexCoord2f(1.0f, 0.0f); glVertex2f( 0.15f, -0.15f);
        glTexCoord2f(1.0f, 1.0f); glVertex2f( 0.15f,  0.15f);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-0.15f,  0.15f);
    glEnd();

    // reseta cor sinao fode as textura do jogo
    glColor3f(1.0f, 1.0f, 1.0f);

    // desliga o GL_TEXTURE_2D para não quebrar a renderização dos objetos de cores lisas/sem textura
    glDisable(GL_TEXTURE_2D);

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