#include "AnimacaoBola.h"
#include <iostream>
#include "stb_image.h"

// iniciazlia as variaveis da animacao da bola
AnimacaoBola::AnimacaoBola() {
    for (int i = 0; i < 6; i++) {
        textureFrames[i] = 0;
    }
    frameCounter = 0;
    currentFrame = 0;
}

// carrega os sprites da bola
void AnimacaoBola::loadTextures() {
    textureFrames[0] = loadTextureSingle("assets/sprites/bola/bola.png");
    textureFrames[1] = loadTextureSingle("assets/sprites/bola/bola2.png");
    textureFrames[2] = loadTextureSingle("assets/sprites/bola/bola3.png");
    textureFrames[3] = loadTextureSingle("assets/sprites/bola/bola4.png");
    textureFrames[4] = loadTextureSingle("assets/sprites/bola/bola5.png");
    textureFrames[5] = loadTextureSingle("assets/sprites/bola/bola6.png");
}

// implementa do carregamento da textura usando stb_image, igual ao do jogador
GLuint AnimacaoBola::loadTextureSingle(const char* filename) {
    GLuint texID = 0;
    int width, height, channels;
    // força 4 canais (RGBA)
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 4); 
    
    if (data) {
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        
        stbi_image_free(data);
    } else {
        std::cerr << "erro ao carregar textura da bola: " << filename << std::endl;
    }
    
    return texID;
}

// metodo que desenha e anima a bola
void AnimacaoBola::draw(float x, float y, bool isMoving) {
    // logica para alternar a imagem caso a bola teja se movendo
    if (isMoving) {
        frameCounter++;
        // a cada X atualizacoes, troca o frame
        // pra qm quiser mudar, o frame counter ae ó
        if (frameCounter >= 10) {
            currentFrame++;
            if (currentFrame > 5) {
                currentFrame = 0; // reseta para a primera image
            }
            frameCounter = 0;
        }
    }

    glPushMatrix(); 
    glTranslatef(x, y, 0.0f); 

    // halitano as  textura e selecionando o frame atual da bola
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureFrames[currentFrame]);
    
    // cor branca para nao tingir a bolaaa
    glColor3f(1.0f, 1.0f, 1.0f);
    
    // cordenadas da bola
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-0.1f, -0.1f);
        glTexCoord2f(1.0f, 0.0f); glVertex2f( 0.1f, -0.1f);
        glTexCoord2f(1.0f, 1.0f); glVertex2f( 0.1f,  0.1f);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-0.1f,  0.1f);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}