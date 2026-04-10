#include "Campo.h"
#include <cmath>
#include <cstdlib>

constexpr float PI = 3.14159265358979323846f;

Campo::Campo() {
    grassTexture = 0;
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

// desenha as traves com um projeçãozinha pra PARECER que é um negocio diferentaooo slkk
void Campo::drawGoals() {
    float W = 0.45f; // laargura do gol 
    float D = 0.4f;  // profundidad da rede no chão
    float H = 0.5f;  // fator pra ilusão de altura

    glEnable(GL_BLEND); // nisso a gente transforma a rede em transparerent

    // gol dicima
    // rede de fundo - do fundo no chão (D) até o topo de trás (D + H)
    glColor4f(0.8f, 0.8f, 0.8f, 0.3f);
    glBegin(GL_QUADS);
        glVertex2f(-W, 5.0f + D);
        glVertex2f( W, 5.0f + D);
        glVertex2f( W, 5.0f + D + H);
        glVertex2f(-W, 5.0f + D + H);
    glEnd();

    // rede do topo - do crossbar frontal (H) até o topo de trás (D + H)
    glColor4f(0.9f, 0.9f, 0.9f, 0.4f);
    glBegin(GL_QUADS);
        glVertex2f(-W, 5.0f + H);
        glVertex2f( W, 5.0f + H);
        glVertex2f( W, 5.0f + D + H);
        glVertex2f(-W, 5.0f + D + H);
    glEnd();

    // grid da rede
    glColor4f(1.0f, 1.0f, 1.0f, 0.4f);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
        for (float x = -W; x <= W + 0.01f; x += 0.09f) {
            glVertex2f(x, 5.0f + D); glVertex2f(x, 5.0f + D + H);
            glVertex2f(x, 5.0f + H); glVertex2f(x, 5.0f + D + H);
        }
        for (float y = 5.0f + D; y <= 5.0f + D + H + 0.01f; y += 0.09f) {
            glVertex2f(-W, y); glVertex2f(W, y);
        }
        for (float y = 5.0f + H; y <= 5.0f + D + H + 0.01f; y += 0.09f) {
            glVertex2f(-W, y); glVertex2f(W, y);
        }
    glEnd();

    // estrutura das trave
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2.0f);
    // linhas de chao i di contorno
    glBegin(GL_LINE_STRIP);
        glVertex2f(-W, 5.0f); glVertex2f(-W, 5.0f + D); glVertex2f(W, 5.0f + D); glVertex2f(W, 5.0f);
    glEnd();
    glBegin(GL_LINES);
        glVertex2f(-W, 5.0f + D); glVertex2f(-W, 5.0f + D + H);
        glVertex2f( W, 5.0f + D); glVertex2f( W, 5.0f + D + H);
    glEnd();
    // traves principasio
    glLineWidth(3.0f);
    glBegin(GL_LINE_STRIP);
        glVertex2f(-W, 5.0f);      // poste esq base
        glVertex2f(-W, 5.0f + H);  // poste esq topo
        glVertex2f( W, 5.0f + H);  // crossbar
        glVertex2f( W, 5.0f);      // poste dir base
    glEnd();


    // trave dibaxo
    // a rede é bawsicamente um retangulo simples projetado pra cima da tela
    glColor4f(0.8f, 0.8f, 0.8f, 0.3f);
    glBegin(GL_QUADS);
        glVertex2f(-W, -5.0f);         // base esq
        glVertex2f( W, -5.0f);         // base dir
        glVertex2f( W, -5.0f + H);     // =topo dir
        glVertex2f(-W, -5.0f + H);     // topo esq
    glEnd();

    //  rede - basicamente um grid d quadrados retos
    glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
        // linhas verticais
        for (float x = -W; x <= W + 0.01f; x += 0.09f) {
            glVertex2f(x, -5.0f);
            glVertex2f(x, -5.0f + H);
        }
        // linhas horizontais
        for (float y = -5.0f; y <= -5.0f + H + 0.01f; y += 0.09f) {
            glVertex2f(-W, y);
            glVertex2f( W, y);
        }
    glEnd();
    
    // as traves brancas 
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(3.0f); 
    glBegin(GL_LINE_STRIP);
        glVertex2f(-W, -5.0f);      // poste esq 
        glVertex2f(-W, -5.0f + H);  // poste esq 
        glVertex2f( W, -5.0f + H);  // crossbar 
        glVertex2f( W, -5.0f);      // poste dir 
    glEnd();

    glLineWidth(1.0f); 
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

    // eai luca s num sei o que  
    // seguinte, mais tarde a gente quando for dar merge
    // vamo ajusar o código pra desenhar a bola aqui
    // ah mas por quê? 
    // meia noite eu te conto

    // os gol ssao desenhados por cima de tudo no campo
    drawGoals();
}

// metodo - checar colisao com as paredes/traves da rede
void Campo::tratarColisaoGol(float& bolaX, float& bolaY, float& velX, float& velY, float raio) {
    float W = 0.45f; // largura do gol
    float D = 0.4f;  // profundidade do gol

    // gol di cima
    // checa se a bola chegou perto da linha do gol do norte ou entrou
    if (bolaY + raio > 5.0f && bolaX > -W - raio && bolaX < W + raio) {
        
        // coolisão com a rede do fundo
        if (bolaY + raio > 5.0f + D) {
            bolaY = 5.0f + D - raio;
            velY = -velY; // inverte a velocidade para quicar
        }
        
        // cooolisão com a rede lateral esquerda - pelo lado de drentro
        if (bolaX - raio < -W && bolaY > 5.0f) {
            bolaX = -W + raio;
            velX = -velX;
        }
        
        // coolisão com a rede lateral direita - pelo lado de derntro
        if (bolaX + raio > W && bolaY > 5.0f) {
            bolaX = W - raio;
            velX = -velX;
        }
        
        // colisão da bola -por fora ou por dentro- exatamente nas traves laterais 
        // trv esqrd em (-W, 5.0)
        float distEsq = std::sqrt(std::pow(bolaX - (-W), 2) + std::pow(bolaY - 5.0f, 2));
        if (distEsq < raio) {
            velX = -velX; 
            velY = -velY; 
        }
        // trv drt em (W, 5.0)
        float distDir = std::sqrt(std::pow(bolaX - W, 2) + std::pow(bolaY - 5.0f, 2));
        if (distDir < raio) {
            velX = -velX;
            velY = -velY;
        }
    }
    
    // gol dibaxo
    // if a bola chegou perto da linha do gol do sul ou entrou
    if (bolaY - raio < -5.0f && bolaX > -W - raio && bolaX < W + raio) {
        
        // coolisão com a rede do fundo
        if (bolaY - raio < -5.0f - D) {
            bolaY = -5.0f - D + raio;
            velY = -velY; // inverte a velocidade para quicar tbmmmm ebaa
        }
        
        // coolisão com a rede lateral esquerda - por dento  tambem
        if (bolaX - raio < -W && bolaY < -5.0f) {
            bolaX = -W + raio;
            velX = -velX;
        }
        
        // clisao com a rede lateral direita - é dento
        if (bolaX + raio > W && bolaY < -5.0f) {
            bolaX = W - raio;
            velX = -velX;
        }
        
        // colisão pontual com os postes do sul
        // trv esqrd em (-W, -5.0)
        float distEsq = std::sqrt(std::pow(bolaX - (-W), 2) + std::pow(bolaY - (-5.0f), 2));
        if (distEsq < raio) {
            velX = -velX;
            velY = -velY;
        }
        // trv dirt em (W, -5.0)
        float distDir = std::sqrt(std::pow(bolaX - W, 2) + std::pow(bolaY - (-5.0f), 2));
        if (distDir < raio) {
            velX = -velX;
            velY = -velY;
        }
    }
}