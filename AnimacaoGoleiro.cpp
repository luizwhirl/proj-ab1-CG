#include "AnimacaoGoleiro.h"
#include <iostream>
#include "stb_image.h"

AnimacaoGoleiro::AnimacaoGoleiro() {
    textureIdle = 0;
    textureWalk1 = 0;
    textureWalk2 = 0;
    
    isWalking = false; 
    frameCounter = 0;
    currentWalkFrame = 1;
}

// carregah as imagens de acordo com o lado (norte ou sul)
void AnimacaoGoleiro::loadTextures(bool isTop) {
    if (isTop) {
        // do norte
        textureIdle = loadTextureSingle("assets/sprites/rival/golero/caranguejo-frente1.png");
        textureWalk1 = loadTextureSingle("assets/sprites/rival/golero/caranguejo-frente1.png");
        textureWalk2 = loadTextureSingle("assets/sprites/rival/golero/caranguejo-frente2.png");
    } else {
        // do sul
        textureIdle = loadTextureSingle("assets/sprites/chiquinho/golero/caranguejo-costa1.png");
        textureWalk1 = loadTextureSingle("assets/sprites/chiquinho/golero/caranguejo-costa1.png");
        textureWalk2 = loadTextureSingle("assets/sprites/chiquinho/golero/caranguejo-costa2.png");
    }
}

GLuint AnimacaoGoleiro::loadTextureSingle(const char* filename) {
    GLuint texID = 0;
    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 4); 
    
    if (data) {
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    } else {
        std::cerr << "erro ao carregar textura: " << filename << std::endl;
    }
    
    return texID;
}

void AnimacaoGoleiro::setWalking(bool walking) {
    isWalking = walking;
}

// lógica de animação com base no isWalking
void AnimacaoGoleiro::draw(float x, float y, float largura, float altura) {
    GLuint currentTexture = textureIdle;
    
    if (isWalking) {
        frameCounter++;
        // a cada 60 atualizacoes, troca o frame
        if (frameCounter >= 60) {
            currentWalkFrame = (currentWalkFrame == 1) ? 2 : 1;
            frameCounter = 0;
        }
        
        if (currentWalkFrame == 1) {
            currentTexture = textureWalk1;
        } else {
            currentTexture = textureWalk2;
        }
    } else {
        currentTexture = textureIdle;
        frameCounter = 0;
    }

    glPushMatrix(); 
    glTranslatef(x, y, 0.0f); 

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, currentTexture);
    
    glColor3f(1.0f, 1.0f, 1.0f);
    
    // o sprite tem exatamente as mesma dimensões dos vértices usados no AnimacaoJogador.cpp
    // -0.15 a 0.15 de largura e -0.2 a 0.2 de altura
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-0.15f, -0.2f); // inferior esquerdo
        glTexCoord2f(1.0f, 0.0f); glVertex2f( 0.15f, -0.2f); // inferior direito
        glTexCoord2f(1.0f, 1.0f); glVertex2f( 0.15f,  0.2f); // superior direito
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-0.15f,  0.2f); // superior esquerdo
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}