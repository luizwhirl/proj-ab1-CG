#include "Gol.h"
#include <GL/glut.h>
#include <cmath>

Gol::Gol() {}

void Gol::draw() {
    float W = 0.45f; // largura do gol 
    
    // variaveis pro gol norte
    float Dn = 0.25f;  // profundidad da rede no chão
    float Hn = 0.25f;  // fator pra ilusão de altura

    // variaveis pro gol sul (retornados ao original)
    float Ds = 0.4f;
    float Hs = 0.5f;

    glEnable(GL_BLEND); // nisso a gente transforma a rede em transparerent

    // gol dicima
    // rede de fundo - do fundo no chão (Dn) até o topo de trás (Dn + Hn)
    glColor4f(0.8f, 0.8f, 0.8f, 0.3f);
    glBegin(GL_QUADS);
        glVertex2f(-W, 5.0f + Dn);
        glVertex2f( W, 5.0f + Dn);
        glVertex2f( W, 5.0f + Dn + Hn);
        glVertex2f(-W, 5.0f + Dn + Hn);
    glEnd();

    // rede do topo - do crossbar frontal (Hn) até o topo de trás (Dn + Hn)
    glColor4f(0.9f, 0.9f, 0.9f, 0.4f);
    glBegin(GL_QUADS);
        glVertex2f(-W, 5.0f + Hn);
        glVertex2f( W, 5.0f + Hn);
        glVertex2f( W, 5.0f + Dn + Hn);
        glVertex2f(-W, 5.0f + Dn + Hn);
    glEnd();

    // grid da rede
    glColor4f(1.0f, 1.0f, 1.0f, 0.4f);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
        for (float x = -W; x <= W + 0.01f; x += 0.09f) {
            glVertex2f(x, 5.0f + Dn); glVertex2f(x, 5.0f + Dn + Hn);
            glVertex2f(x, 5.0f + Hn); glVertex2f(x, 5.0f + Dn + Hn);
        }
        for (float y = 5.0f + Dn; y <= 5.0f + Dn + Hn + 0.01f; y += 0.09f) {
            glVertex2f(-W, y); glVertex2f(W, y);
        }
        for (float y = 5.0f + Hn; y <= 5.0f + Dn + Hn + 0.01f; y += 0.09f) {
            glVertex2f(-W, y); glVertex2f(W, y);
        }
    glEnd();

    // estrutura das trave
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2.0f);
    // linhas de chao i di contorno
    glBegin(GL_LINE_STRIP);
        glVertex2f(-W, 5.0f); glVertex2f(-W, 5.0f + Dn); glVertex2f(W, 5.0f + Dn); glVertex2f(W, 5.0f);
    glEnd();
    glBegin(GL_LINES);
        glVertex2f(-W, 5.0f + Dn); glVertex2f(-W, 5.0f + Dn + Hn);
        glVertex2f( W, 5.0f + Dn); glVertex2f( W, 5.0f + Dn + Hn);
    glEnd();
    // traves principasio
    glLineWidth(3.0f);
    glBegin(GL_LINE_STRIP);
        glVertex2f(-W, 5.0f);      // poste esq base
        glVertex2f(-W, 5.0f + Hn); // poste esq topo
        glVertex2f( W, 5.0f + Hn); // crossbar
        glVertex2f( W, 5.0f);      // poste dir base
    glEnd();


    float baseSul = -5.0f - Ds; 
    float topoSul = baseSul + Hs; 

    // a rede é bawsicamente um retangulo simples projetado pra cima da tela
    glColor4f(0.8f, 0.8f, 0.8f, 0.3f);
    glBegin(GL_QUADS);
        glVertex2f(-W, baseSul);       // base esq
        glVertex2f( W, baseSul);       // base dir
        glVertex2f( W, topoSul);       // topo dir
        glVertex2f(-W, topoSul);       // topo esq
    glEnd();

    //  rede - basicamente um grid d quadrados retos
    glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
        // linhas verticais
        for (float x = -W; x <= W + 0.01f; x += 0.09f) {
            glVertex2f(x, baseSul);
            glVertex2f(x, topoSul);
        }
        // linhas horizontais
        for (float y = baseSul; y <= topoSul + 0.01f; y += 0.09f) {
            glVertex2f(-W, y);
            glVertex2f( W, y);
        }
    glEnd();
    
    // as traves brancas 
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(3.0f); 
    glBegin(GL_LINE_STRIP);
        glVertex2f(-W, baseSul);      // poste esq 
        glVertex2f(-W, topoSul);      // poste esq topo
        glVertex2f( W, topoSul);      // crossbar 
        glVertex2f( W, baseSul);      // poste dir 
    glEnd();

    glLineWidth(1.0f); 
}

void Gol::resolverColisao(float& posX, float& posY, float raio) {
    float W = 0.45f; // largura do gol
    float Dn = 0.25f;  // profundidade do gol norte
    float Ds = 0.4f;   // profundidade do gol sul

    // Usamos um raio "fictício" menor para bater na rede do fundo, 
    // assim o jogador não é barrado cedo demais e visualmente entra no gol
    float raioFundo = 0.05f; 

    // GOL NORTE
    if (posY + raio > 5.0f && posY - raio < 5.0f + Dn) {
        
        // Colisão com a rede/trave lateral esquerda
        if (std::abs(posX - (-W)) < raio) {
            if (posX > -W) posX = -W + raio; 
            else posX = -W - raio;
        }
        
        // Colisão com a rede/trave lateral direita
        if (std::abs(posX - W) < raio) {
            if (posX < W) posX = W - raio;   
            else posX = W + raio;            
        }
        
        // Colisão com o fundo da rede (limitado ao espaço entre as traves laterais para não enroscar na quina)
        if (posX > -W + 0.05f && posX < W - 0.05f) { 
            if (posY + raioFundo > 5.0f + Dn) {
                posY = 5.0f + Dn - raioFundo;
            }
        }
    }
    
    // GOL SUL
    if (posY - raio < -5.0f && posY + raio > -5.0f - Ds) {
        
        // Colisão lateral esquerda
        if (std::abs(posX - (-W)) < raio) {
            if (posX > -W) posX = -W + raio; 
            else posX = -W - raio;
        }
        
        // Colisão lateral direita
        if (std::abs(posX - W) < raio) {
            if (posX < W) posX = W - raio;
            else posX = W + raio;
        }
        
        // Colisão com o fundo da rede
        if (posX > -W + 0.05f && posX < W - 0.05f) { 
            if (posY - raioFundo < -5.0f - Ds) {
                posY = -5.0f - Ds + raioFundo;
            }
        }
    }
}