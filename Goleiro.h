#ifndef GOLEIRO_H
#define GOLEIRO_H

#include "Bola.h"
#include "AnimacaoGoleiro.h"

class Goleiro {
public:
    float x, y;
    float velocidade;
    float limiteEsq, limiteDir;
    float largura, altura;

    float alvoX;
    float velocidadeAtual;      
    int   ultimoTempoAtualizacao;
    int   atrasoTempoMs;

    // a patrulha é só pra dar uma vida ao goleiro
    // quando o jogador com a bola tiver longe, elefica debobeira
    float direcaoPatrulha; // +1 ou -1
    int   tempoDirecao;

    // só pro bicho numficar roubado 
    float erroHumano;
    
    bool isTop; // essa praguinha define se é o goleiro de cima (alemanha buuu) ou baixo (brasil ihaaa)

    // instância da classe de animação
    AnimacaoGoleiro animacao; 
    
    Goleiro();
    // pro construtor saber o lado dele - recebe isTopGoleiro
    Goleiro(float startX, float startY, float v, float lEsq, float lDir, bool isTopGoleiro);

    void loadTexture(); // cha ma para carregar as texturas

    void draw();
    void update(const Bola& bola);
    void resolverColisao(float& posX, float& posY, float raio);
};

#endif