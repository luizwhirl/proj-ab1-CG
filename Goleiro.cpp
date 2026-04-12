#include "Goleiro.h"
#include <GL/glut.h>
#include <cstdlib>
#include <cmath>
#include <algorithm>

Goleiro::Goleiro() {
    x = 0.0f;
    y = 4.5f;
    velocidade     = 0.015f;
    limiteEsq      = -1.5f;
    limiteDir      =  1.5f;
    largura        =  0.4f;
    altura         =  0.2f;

    alvoX                = x;
    velocidadeAtual      = 0.0f;
    ultimoTempoAtualizacao = 0;
    atrasoTempoMs        = 800;

    // patrulha suave 
    direcaoPatrulha =  1.0f;
    tempoDirecao    =  0;

    // Offset de erro
    erroHumano = 0.0f;
    isTop = true; 
    // por padrão ele é o goleiro de cima
}

Goleiro::Goleiro(float startX, float startY, float v, float lEsq, float lDir, bool isTopGoleiro) {
    x = startX;
    y = startY;
    velocidade     = v;
    limiteEsq      = lEsq;
    limiteDir      = lDir;
    largura        = 0.4f;
    altura         = 0.2f;

    alvoX                = x;
    velocidadeAtual      = 0.0f;
    ultimoTempoAtualizacao = 0;
    atrasoTempoMs        = 800;

    direcaoPatrulha = 1.0f;
    tempoDirecao    = 0;
    erroHumano      = 0.0f;
    
    isTop = isTopGoleiro; // gravah de qual lado o goleiro é
}

void Goleiro::draw() {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);

    if (isTop) {
        glColor3f(0.5f, 0.5f, 0.5f); // cinza pra alemnha pq eles sao fei
    } else {
        glColor3f(1.0f, 1.0f, 0.0f); // amarelo pro brasil proque sim 😎
    }
    
    glBegin(GL_QUADS);
        glVertex2f(-largura/2, -altura/2);
        glVertex2f( largura/2, -altura/2);
        glVertex2f( largura/2,  altura/2);
        glVertex2f(-largura/2,  altura/2);
    glEnd();

    glPopMatrix();
}

void Goleiro::update(const Bola& bola) {
    int tempoAtual = glutGet(GLUT_ELAPSED_TIME);

    // cálculo de urgência
    // quantomais perto e mais rapido a bola, meno r é o atrasodo golero
    float distancia = std::abs(bola.y - y);
    float urgencia  = 1.0f - std::min(distancia / 5.0f, 1.0f); // 0 = longe, 1 = pertinho
    int atrasoEfetivo = (int)(atrasoTempoMs * (1.0f - urgencia * 0.7f)); // reduz até 70% do atraso

    if (tempoAtual - ultimoTempoAtualizacao > atrasoEfetivo) {

        // a verificacao se a bola está no campo do goleiro depende se ele é o de cima (y<0 está longe) ou baixo (y>0 está longe)
        bool bolaLonge = isTop ? (bola.y < 0.0f) : (bola.y > 0.0f); // ADICIONADO
        
        if (bolaLonge) { // ALTERADO: Substitui o 'if (bola.y < 0.0f)' genérico
            // patrulha-> movimento mais oscilatório ao invés de pular di la prah ca
            // a cada 1.5-2.5 segundos
            if (tempoAtual - tempoDirecao > 1500 + rand() % 1000) {
                direcaoPatrulha *= -1.0f;
                tempoDirecao = tempoAtual;
            }

            float centroGol = (limiteEsq + limiteDir) / 2.0f;
            // fica próximo ao centro mas se afasta um pouco na direção da patrulha
            alvoX = centroGol + direcaoPatrulha * (limiteDir - centroGol) * 0.5f;

        } else {
            // ele vai tentar prever  onde a bola vai estar daqui a maisomenos 300ms
            // e a gente reza pra ele errar 
            float previsaoX = bola.x;
            // na previsao ele nao sai da área dele porque ele é um bom menino
            if (previsaoX < limiteEsq) previsaoX = limiteEsq;
            if (previsaoX > limiteDir) previsaoX = limiteDir;

            // erroHumano-> offset aleatório pra simularimprecisão 
            // ele só vai atualizar de vez em quando, ao inves do tempo todo
            erroHumano = ((rand() % 100) / 100.0f - 0.5f) * 1.6f; // ±0.125 unidades

            alvoX = previsaoX + erroHumano;
        }

        ultimoTempoAtualizacao = tempoAtual;
    }

    // o golero interpola velocidade pra num ta correndodemaiss
    float diff = alvoX - x;
    float velMax = velocidade * (1.0f + urgencia * 2.0f); // aumenta a velMaxi com urgencia qualqr coisa

    // empurra velocidadeAtual na direção do alvo
    float aceleracao = 0.001f + urgencia * 0.002f;
    if (diff > 0.05f)
        velocidadeAtual += aceleracao;
    else if (diff < -0.05f)
        velocidadeAtual -= aceleracao;
    else
        velocidadeAtual *= 0.85f; // freio pra qtando fica perto

    // velocidade do caba
    if (velocidadeAtual >  velMax) velocidadeAtual =  velMax;
    if (velocidadeAtual < -velMax) velocidadeAtual = -velMax;

    x += velocidadeAtual;

    // limites do caba
    if (x < limiteEsq) { x = limiteEsq; velocidadeAtual = 0.0f; }
    if (x > limiteDir) { x = limiteDir; velocidadeAtual = 0.0f; }
}

void Goleiro::resolverColisao(float& objX, float& objY, float objRaio) {
    float goleiroEsq  = x - largura/2;
    float goleiroDir  = x + largura/2;
    float goleiroCima = y + altura/2;
    float goleiroBaixo= y - altura/2;

    float objEsq  = objX - objRaio;
    float objDir  = objX + objRaio;
    float objCima = objY + objRaio;
    float objBaixo= objY - objRaio;

    if (objEsq < goleiroDir && objDir > goleiroEsq &&
        objBaixo < goleiroCima && objCima > goleiroBaixo) {
        if (objY < y)
            objY = goleiroBaixo - objRaio;
        else
            objY = goleiroCima + objRaio;
    }
}