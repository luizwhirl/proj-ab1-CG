#include "Campo.h"
#include <cmath>
#include <cstdlib>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

constexpr float PI = 3.14159265358979323846f;

Campo::Campo() {
    grassTexture = 0;
    arquibancadaTexture = 0;
}

void Campo::createGrassTexture() {
    const int TEX_SIZE = 128; // 128x128 pixels 
    unsigned char data[TEX_SIZE * TEX_SIZE * 3];

    // paleta de cores no carai
    unsigned char palette[4][3] = {
        { 90, 145,  65},  // verde médio base
        {105, 160,  70},  // verde mais claro
        { 75, 120,  50},  // verde escuro
        {110, 130,  55}   // verde amarelado
    };

    for (int i = 0; i < TEX_SIZE * TEX_SIZE; ++i) {
        int colorIndex = rand() % 4;
        
        data[i * 3]     = palette[colorIndex][0];
        data[i * 3 + 1] = palette[colorIndex][1];
        data[i * 3 + 2] = palette[colorIndex][2];
    }

    glGenTextures(1, &grassTexture);
    glBindTexture(GL_TEXTURE_2D, grassTexture);
    
    // enviando dados da textura para o opengll
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TEX_SIZE, TEX_SIZE, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    //  - nao aplica blur ao escalajar
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // rete a a rexteura ao longo do campo
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void Campo::loadArquibancadaTexture(const char* filepath) {
    glGenTextures(1, &arquibancadaTexture);
    glBindTexture(GL_TEXTURE_2D, arquibancadaTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // oia que legal tem ate função de virar
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); 
    
    unsigned char *data = stbi_load(filepath, &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    } else {
        std::cerr << "Falha ao carregar a textura da arquibancada: " << filepath << std::endl;
    }
}

void Campo::drawArquibancada() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, arquibancadaTexture);
    glColor3f(1.0f, 1.0f, 1.0f);

    float largura = 3.5f;
    float alturaProporcional = 2.33f;

    // arquibankada norte 
    float baseNorte = 5.5f;
    float topoNorte = baseNorte + alturaProporcional;
    
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-largura, baseNorte);
        glTexCoord2f(1.0f, 0.0f); glVertex2f( largura, baseNorte);
        glTexCoord2f(1.0f, 1.0f); glVertex2f( largura, topoNorte);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-largura, topoNorte);
    glEnd();

    // os neguinho tao pulando so na norte
    glDisable(GL_TEXTURE_2D); // textura desativada

    float tempo = glutGet(GLUT_ELAPSED_TIME) / 1000.0f; 
    float tam = 0.06f; // tamanh do kwadrado 

    int numFileiras = 2;
    int numQuadradinhos = 8;

    // ajusta o X e Y inicial e final para garantir que fiquem estritamente "dentro" do concreto
    // o x original vai de -3.5 a 3.5
    //  O Y original vai de 5.5 a 7.83
    float startX = -2.8f; 
    float endX = 2.8f;    
    float startY = 6.66f; // fileira Y do di cima
    float endY = 7.0f;   // fileira Y do di cima

    glBegin(GL_QUADS);
    for (int row = 0; row < numFileiras; ++row) {
        for (int col = 0; col < numQuadradinhos; ++col) {
            
            // interpola linearmente a posição de acordo com a coluna e fileira
            float x = startX + col * ((endX - startX) / (numQuadradinhos - 1));
            float y = startY + row * ((endY - startY) / (numFileiras - 1));
            
            float offset = x * 3.5f + y * 2.5f; 
            
            // pulo suave de 
            float pulo = (std::sin(tempo * 16.0f + offset) + 1.0f) * 0.5f * 0.12f; 

            // random cores 
            float corMix = std::sin(x * 12.0f + y * 8.0f);
            if (corMix > 0.5f) glColor3f(0.9f, 0.1f, 0.1f);
            else if (corMix > -0.5f) glColor3f(1.0f, 1.0f, 1.0f);
            else glColor3f(0.1f, 0.1f, 0.1f);

            float qX = x;
            float qY = y + pulo;

            // quadradis
            glVertex2f(qX - tam, qY - tam);
            glVertex2f(qX + tam, qY - tam);
            glVertex2f(qX + tam, qY + tam);
            glVertex2f(qX - tam, qY + tam);
        }
    }
    glEnd();

    // reativa a textura para o resto do campo funcionar normal
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, arquibancadaTexture);
    glColor3f(1.0f, 1.0f, 1.0f);

    // arkibankada sulll
    float baseSul = -5.5f;
    float topoSul = baseSul - alturaProporcional;

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-largura, topoSul);
        glTexCoord2f(1.0f, 1.0f); glVertex2f( largura, topoSul);
        glTexCoord2f(1.0f, 0.0f); glVertex2f( largura, baseSul);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-largura, baseSul);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void Campo::drawCircle(float cx, float cy, float r, int num_segments) {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < num_segments; i++) {
        float theta = 2.0f * PI * static_cast<float>(i) / static_cast<float>(num_segments);
        float x = r * std::cos(theta);
        float y = r * std::sin(theta);
        glVertex2f(cx + x, cy + y);
    }
    glEnd();
}

// desenhar as linhas do campo na vertical
void Campo::drawFieldLines() {
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2.0f);

    glBegin(GL_LINE_LOOP);
        glVertex2f(-3.0f, -5.0f);
        glVertex2f( 3.0f, -5.0f);
        glVertex2f( 3.0f,  5.0f);
        glVertex2f(-3.0f,  5.0f);
    glEnd();

    // meio do campo
    glBegin(GL_LINES);
        glVertex2f(-3.0f, 0.0f);
        glVertex2f( 3.0f, 0.0f);
    glEnd();

    // meio do meio do campo - o círculo
    drawCircle(0.0f, 0.0f, 1.0f, 36);

    // lado dibaxo - grande area
    glBegin(GL_LINE_STRIP);
        glVertex2f(-1.5f, -5.0f);
        glVertex2f(-1.5f, -3.5f);
        glVertex2f( 1.5f, -3.5f);
        glVertex2f( 1.5f, -5.0f);
    glEnd();

    // lado dibaxo 2 - pqeuna area
    glBegin(GL_LINE_STRIP);
        glVertex2f(-0.6f, -5.0f);
        glVertex2f(-0.6f, -4.2f);
        glVertex2f( 0.6f, -4.2f);
        glVertex2f( 0.6f, -5.0f);
    glEnd();

    // lado di cimah - grande area de novo wtf 
    glBegin(GL_LINE_STRIP);
        glVertex2f(-1.5f,  5.0f);
        glVertex2f(-1.5f,  3.5f);
        glVertex2f( 1.5f,  3.5f);
        glVertex2f( 1.5f,  5.0f);
    glEnd();

    // lado di xima 2 - pquenea area 
    glBegin(GL_LINE_STRIP);
        glVertex2f(-0.6f,  5.0f);
        glVertex2f(-0.6f,  4.2f);
        glVertex2f( 0.6f,  4.2f);
        glVertex2f( 0.6f,  5.0f);
    glEnd();
}

void Campo::draw() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, grassTexture);
    
    // esse repeats via definr quantas vezes a etxtura da grama vai se repetir
    float repeats = 15.0f; 

    // lógica de faixas
    int numFaixas = 11; // 11 faixas distribuidas ao longo (nisso bem distribuído de -5.5 a 5.5)
    float campoInicioY = -5.5f;
    float campoFimY = 5.5f;
    float alturaFaixa = (campoFimY - campoInicioY) / numFaixas;

    glBegin(GL_QUADS);
    for (int i = 0; i < numFaixas; ++i) {
        float yBase = campoInicioY + i * alturaFaixa;
        float yTopo = yBase + alturaFaixa;

        // altaernancia de cores de casa lado
        if (i % 2 == 0) {
            glColor3f(1.0f, 1.0f, 1.0f); // tim base da textura - claro
        } else {
            glColor3f(0.85f, 0.90f, 0.85f); // multiplika a textura por um tom mais escuro
        }

        // calkuloda coordenada UV da textura para que ela não quebre na divisa das faixas
        float vBase = (yBase - campoInicioY) / (campoFimY - campoInicioY) * repeats;
        float vTopo = (yTopo - campoInicioY) / (campoFimY - campoInicioY) * repeats;

        // faixa horizontal
        glTexCoord2f(0.0f, vBase);       glVertex2f(-3.5f, yBase);
        glTexCoord2f(repeats, vBase);    glVertex2f( 3.5f, yBase);
        glTexCoord2f(repeats, vTopo);    glVertex2f( 3.5f, yTopo);
        glTexCoord2f(0.0f, vTopo);       glVertex2f(-3.5f, yTopo);
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);

    drawFieldLines();

    // Desenhando a arquibancada por baixo
    drawArquibancada();
}

void Campo::resolverColisaoLimites(float& posX, float& posY, float raio) {
    float limiteX = 3.5f; // limites laterais do gramado verde
    float limiteY = 5.5f; // limites superior e inferior do gramado

    // Colisão com as laterais (X)
    if (posX - raio < -limiteX) {
        posX = -limiteX + raio;
    } else if (posX + raio > limiteX) {
        posX = limiteX - raio;
    }

    // Colisão com as linhas de fundo (Y)
    if (posY - raio < -limiteY) {
        posY = -limiteY + raio;
    } else if (posY + raio > limiteY) {
        posY = limiteY - raio;
    }
}