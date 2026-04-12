#include "Jogador.h"
#include <GL/glut.h>
#include <utility>
#include <iostream>
#include "stb_image.h"

Jogador::Jogador() {
    x = 1.0f;
    y = 1.0f;
    textureID = 0; // inicia sem textira, ai depois carrega
}

// implementando o carregamento do png e criando a textura do opengiló
void Jogador::loadTexture(const char* filename) {
    int width, height, channels;
    // força 4 canais (RGBA) para garantir o canal alfa da transparência
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 4); 
    
    if (data) {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        
        // define os parâmetros da textura pra suavizar e num ficar distorcidooo
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        // envia a imagem pra praca video kkk ei vei isso é uma resenha porra nunca esperei de um dia ter q fazer isso
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        
        stbi_image_free(data);
    } else {
        std::cerr << "Erro ao carregar textura do jogador: " << filename << std::endl;
    }
}

void Jogador::draw() {
    glPushMatrix(); 
    
    glTranslatef(x, y, 0.0f); 

    // habilitand texturas e selecionando a do jogador do chiquinho sorbetes
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    // branca- não escurece nem tinge a imagem
    glColor3f(1.0f, 1.0f, 1.0f);
    
    // coordenadas do jogador 
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-0.15f, -0.2f); // infer esq
        glTexCoord2f(1.0f, 0.0f); glVertex2f( 0.15f, -0.2f); // infer dir
        glTexCoord2f(1.0f, 1.0f); glVertex2f( 0.15f,  0.2f); // supr dir
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-0.15f,  0.2f); // infer esq
    glEnd();

    // desabilita a textura para não interferir no resto do desenho do jogo
    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}