#include "AnimacaoJogador.h"
#include <iostream>
#include "stb_image.h"

AnimacaoJogador::AnimacaoJogador() {
    textureIdle = 0; // inicia sem textira, ai depois carrega
    
    // inicia as texturas de andar e as variaveis de controle
    textureWalk1 = 0;
    textureWalk2 = 0;
    
    // inicia as variaveis de textura de costas e lado
    textureBackIdle = 0;
    textureBackWalk1 = 0;
    textureBackWalk2 = 0;
    
    textureSideIdle = 0;
    textureSideWalk1 = 0;
    textureSideWalk2 = 0;
    
    isWalking = false; 
    frameCounter = 0;
    currentWalkFrame = 1;
}

// carrega tudo as texturas separado
// recebe a flag isRival para escolher qual pack de textura carregar
void AnimacaoJogador::loadTextures(bool isRival) {
    if (isRival) {
        // sprites do time rival 
        textureIdle = loadTextureSingle("assets/sprites/rival/loiro-frente.png");
        textureWalk1 = loadTextureSingle("assets/sprites/rival/loiro-frenteAnda.png");
        textureWalk2 = loadTextureSingle("assets/sprites/rival/loiro-frenteAnda2.png");
        
        textureBackIdle = loadTextureSingle("assets/sprites/rival/loiro-costa.png");
        textureBackWalk1 = loadTextureSingle("assets/sprites/rival/loiro-costaAnda.png");
        textureBackWalk2 = loadTextureSingle("assets/sprites/rival/loiro-costaAnda2.png");
        
        textureSideIdle = loadTextureSingle("assets/sprites/rival/loiro-lado.png");
        textureSideWalk1 = loadTextureSingle("assets/sprites/rival/loiro-ladoAnda.png");
        textureSideWalk2 = loadTextureSingle("assets/sprites/rival/loiro-ladoAnda2.png");
    } else {
        // sprites dos aliado
        textureIdle = loadTextureSingle("assets/sprites/frente.png");
        textureWalk1 = loadTextureSingle("assets/sprites/frente-anda.png");
        textureWalk2 = loadTextureSingle("assets/sprites/frente-anda2.png");
        
        textureBackIdle = loadTextureSingle("assets/sprites/costa.png");
        textureBackWalk1 = loadTextureSingle("assets/sprites/costa-anda.png");
        textureBackWalk2 = loadTextureSingle("assets/sprites/costa-anda2.png");
        
        textureSideIdle = loadTextureSingle("assets/sprites/lado.png");
        textureSideWalk1 = loadTextureSingle("assets/sprites/lado-anda.png");
        textureSideWalk2 = loadTextureSingle("assets/sprites/lado-anda2.png");
    }
}

// implementando o carregamento do png e criando a textura do opengiló
// retornando o GLuint da textura carregada
GLuint AnimacaoJogador::loadTextureSingle(const char* filename) {
    GLuint texID = 0;
    int width, height, channels;
    // força 4 canais (RGBA) para garantir o canal alfa da transparência
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 4); 
    
    if (data) {
        // usa a variavel local texID
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);
        
        // define os parâmetros da textura pra suavizar e num ficar distorcidooo
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        // envia a imagem pra praca video kkk ei vei isso é uma resenha porra nunca esperei de um dia ter q fazer isso
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        
        stbi_image_free(data);
    } else {
        std::cerr << "erro ao carregar textura: " << filename << std::endl;
    }
    
    return texID; // retorna o ID gerado
}

// se o jogador ta andando ou parado, atualiza
void AnimacaoJogador::setWalking(bool walking) {
    isWalking = walking;
}

//  recebe direction como argumento. e ai ele pode saber a direção (sério?😱😱😱😱)
void AnimacaoJogador::draw(float x, float y, char direction) {
    GLuint currentTexture = textureIdle;
    
    // ponteros temporarios -> facilita a troca de conjuntos de sprites
    GLuint tIdle = textureIdle;
    GLuint tW1 = textureWalk1;
    GLuint tW2 = textureWalk2;
    
    bool flipX = false; // essa bixinhavai dizer se a imagem de lado vai ser espelhada
    
    // loogika pra escolher kwal pacote de imagens usar baseado na direção apontada
    if (direction == 'W') {
        tIdle = textureBackIdle;
        tW1 = textureBackWalk1;
        tW2 = textureBackWalk2;
    } else if (direction == 'S') {
        tIdle = textureIdle; 
        tW1 = textureWalk1;
        tW2 = textureWalk2;
    } else if (direction == 'A') {
        tIdle = textureSideIdle;
        tW1 = textureSideWalk1;
        tW2 = textureSideWalk2;
    } else if (direction == 'D') {
        tIdle = textureSideIdle;
        tW1 = textureSideWalk1;
        tW2 = textureSideWalk2;
        flipX = true; 
        // oia o flip ai hehe
    }
    
    // logika di alternar a imagem se caso tiver andando
    if (isWalking) {
        frameCounter++;
        // a cada 60 atualizacoes, troca o frame
        // trocar aki qualquer coisa 
        if (frameCounter >= 60) {
            currentWalkFrame = (currentWalkFrame == 1) ? 2 : 1;
            frameCounter = 0;
        }
        
        if (currentWalkFrame == 1) {
            currentTexture = tW1; // tW1 da direcao atual
        } else {
            currentTexture = tW2; //tW2 da direcao atual
        }
    } else {
        currentTexture = tIdle; // e esse força aficar parado com o sprite certo da direção
        frameCounter = 0;
    }

    glPushMatrix(); 
    
    glTranslatef(x, y, 0.0f); 

    // habilitand texturas e selecionando a do jogador do chiquinho sorbetes
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, currentTexture);
    
    // branca- não escurece nem tinge a imagem
    glColor3f(1.0f, 1.0f, 1.0f);
    
    // coordenadas do jogador 
    glBegin(GL_QUADS);
        // se jogador tiver virado pra direita
        // desenhamos a mesma textura mas trocando as coords X pra dar efeito de q virou
        if (flipX) {
            glTexCoord2f(1.0f, 0.0f); glVertex2f(-0.15f, -0.2f); // infer esq (virado esse)
            glTexCoord2f(0.0f, 0.0f); glVertex2f( 0.15f, -0.2f); // infer dir (e os de baxo tudinho)
            glTexCoord2f(0.0f, 1.0f); glVertex2f( 0.15f,  0.2f); // supr dir 
            glTexCoord2f(1.0f, 1.0f); glVertex2f(-0.15f,  0.2f); // supr esq 
        } else {
            glTexCoord2f(0.0f, 0.0f); glVertex2f(-0.15f, -0.2f); // infer esq (ai aqui ja é normal)
            glTexCoord2f(1.0f, 0.0f); glVertex2f( 0.15f, -0.2f); // infer dir
            glTexCoord2f(1.0f, 1.0f); glVertex2f( 0.15f,  0.2f); // supr dir
            glTexCoord2f(0.0f, 1.0f); glVertex2f(-0.15f,  0.2f); // supr esq
        }
    glEnd();

    // desabilita a textura para não interferir no resto do desenho do jogo
    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}