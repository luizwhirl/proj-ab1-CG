#include "Game.h"
#include <GL/glut.h>
#include <cstdlib> // rand() e srand()
#include <ctime>   // time()
#include <stack>
#include <iostream>
#include <cmath> 

Game* Game::instance = nullptr;

Game::Game() {
    winW = 600;
    winH = 800;

    // inicialização dos dois goleiros, definindo suas posições no eixo Y e passando true/false para a tag isTop
    goleiroAlemanha = Goleiro(0.0f, 4.5f, 0.015f, -1.5f, 1.5f, true);
    goleiroBrasil = Goleiro(0.0f, -4.5f, 0.015f, -1.5f, 1.5f, false);
}

Game* Game::getInstance() {
    if (!instance) {
        instance = new Game();
    }
    return instance;
}

void Game::init() {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // inicializa a seed do rand()
    srand(static_cast<unsigned int>(time(NULL)));
    
    campo.createGrassTexture();

    // carreguemos as texturas aquiiis
    // passano os caminhos das bichas
    campo.loadArquibancadaTextures("assets/sprites/arquibancadaA.png", "assets/sprites/ArquibancadaB.png", "assets/sprites/ArquibancadaC.png", "assets/sprites/ArquibancadaD.png");
    
    // texuta do jogaodr do chiquinho sorbetes
    jogador.loadTexture("assets/sprites/f1.png");
}

void Game::setupCamera() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = static_cast<float>(winW) / static_cast<float>(winH);
    float size = 6.0f;
    float left, right, bottom, top;

    // calculo do limite da camera
    if (winW <= winH) {
        left = -size; right = size;
        bottom = -size / aspect; top = size / aspect;
    } else {
        left = -size * aspect; right = size * aspect;
        bottom = -size; top = size;
    }

    if (input.isZoomed) {
        float zoomFactor = 0.35f;
        float viewWidth = (right - left) * zoomFactor;
        float viewHeight = (top - bottom) * zoomFactor;

        left = input.mouseWorldX - viewWidth / 2.0f;
        right = input.mouseWorldX + viewWidth / 2.0f;
        bottom = input.mouseWorldY - viewHeight / 2.0f;
        top = input.mouseWorldY + viewHeight / 2.0f;
    }

    glOrtho(left, right, bottom, top, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Game::display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    setupCamera();

    campo.draw();

    // jogador e bola vêm primeiro
    jogador.draw();
    bola.draw();
    
    // desenha o goleiro
    goleiroAlemanha.draw();
    goleiroBrasil.draw();

    // os gols sao desenhados POR ÚLTIMO pra rede e o topo aparecerem ACIMA deles visualmente
    gol.draw();

    scoreboard.draw(winW, winH);

    glutSwapBuffers();
}

void Game::reshape(int w, int h) {
    if (h == 0) h = 1;
    winW = w;
    winH = h;
    glViewport(0, 0, w, h);
}

void Game::mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        
        input.isZoomed = !input.isZoomed;
        
        if (input.isZoomed) {
            input.updateMouseWorldCoords(x, y, winW, winH);
        }
        glutPostRedisplay();
    }
}

// passivo igual lucas (????????)
void Game::mousePassiveMotion(int x, int y) {
    if (input.isZoomed) {
        input.updateMouseWorldCoords(x, y, winW, winH);
        glutPostRedisplay(); // atualiza continuamente a tela ne pq zoom i tals
    }
}

// verifica se a tecla foi clicada
void Game::keyboardClick(unsigned char key, int x, int y) {
    if (key == 'w' || key == 'W') input.isWPressed = true;
    if (key == 'a' || key == 'A') input.isAPressed = true;
    if (key == 's' || key == 'S') input.isSPressed = true;
    if (key == 'd' || key == 'D') input.isDPressed = true;
    if (key == 'j' || key == 'J') input.isJPressed = true;
    if (key == 'k' || key == 'K') input.isKPressed = true;
}

// ve se a tecla foi solto
void Game::keyboardUp(unsigned char key, int x, int y) {
    if (key == 'w' || key == 'W') input.isWPressed = false;
    if (key == 'a' || key == 'A') input.isAPressed = false;
    if (key == 's' || key == 'S') input.isSPressed = false;
    if (key == 'd' || key == 'D') input.isDPressed = false;
    if (key == 'j' || key == 'J') input.isJPressed = false;
    if (key == 'k' || key == 'K') input.isKPressed = false;
}

float Game::pitagoras(float catetoAdj, float catetoOpos) {
    float distance = sqrt(pow(catetoAdj, 2) + pow(catetoOpos, 2));
    return distance;
}

// Atualiza a posição do jogador e da bola
void Game::updatePlayer() {
    float dirX = 0;
    float dirY = 0;

    // verifica a tecla pressionada e salva a ultima posição
    if (input.isWPressed) { 
        dirY += 1; 
        jogador.lastDirection = 'W'; 
    }
    if (input.isSPressed) { 
        dirY -= 1; 
        jogador.lastDirection = 'S'; 
    }
    if (input.isDPressed) { 
        dirX += 1; 
        jogador.lastDirection = 'D'; 
    }
    if (input.isAPressed) { 
        dirX -= 1; 
        jogador.lastDirection = 'A'; 
    }

    // normaliza vetor (eu deveria ter prestado atenção na aula)
    float magnitude = pitagoras(dirX, dirY);
    if (magnitude > 0) {
        dirX = dirX / magnitude;
        dirY = dirY / magnitude;
    }

    // faz o jogador correr
    float velocidadeJogador = 0.01f; 
    jogador.x += dirX * velocidadeJogador;
    jogador.y += dirY * velocidadeJogador;

    // pega a bola se chegar perto
    if (!bola.isHeld) {
        float distance = pitagoras(bola.x - jogador.x, bola.y - jogador.y);
        if (distance < 0.4) {
            bola.velX = 0;
            bola.velY = 0;
            bola.isHeld = true;
        }
    }

    // solta a bola manualmente
    if (input.isJPressed) {
        bola.isHeld = false;
    }

    // dá o chutão 
    if (input.isKPressed && bola.isHeld) {
        switch (jogador.lastDirection) {
            case 'W': bola.velY += 0.05f; break;
            case 'A': bola.velX -= 0.05f; break;
            case 'S': bola.velY -= 0.05f; break;
            case 'D': bola.velX += 0.05f; break;
        }
        bola.isHeld = false;
    }

    // o jogador sai driblando com a bola colada no pé
    if (bola.isHeld) {
        switch (jogador.lastDirection) {
            case 'W':
                bola.x = jogador.x;
                bola.y = jogador.y + 0.4;
                break;
            case 'A':
                bola.x = jogador.x - 0.4;
                bola.y = jogador.y;
                break;
            case 'S':
                bola.x = jogador.x;
                bola.y = jogador.y - 0.4;
                break;
            case 'D':
                bola.x = jogador.x + 0.4;
                bola.y = jogador.y;
                break;
        }
    }

    if (!bola.isHeld) {
        // adiciona aceleração a bola
        bola.x += bola.velX;
        bola.y += bola.velY;

        // bola vai freiando na grama (papo de fisica)
        bola.velX *= 0.98f;
        bola.velY *= 0.98f;
    }

    // resolvendo as colisões DEPOIS do update de teclas
    // primeiro os limites do campo para não deixar ngm sair
    campo.resolverColisaoLimites(jogador.x, jogador.y, 0.2f);
    campo.resolverColisaoLimites(bola.x, bola.y, 0.1f);

    // atualiza lógica do goleiro (movimento) e colisões dele com jogador e bola
    goleiroAlemanha.update(bola);
    goleiroAlemanha.resolverColisao(jogador.x, jogador.y, 0.2f);
    goleiroAlemanha.resolverColisao(bola.x, bola.y, 0.1f);

    goleiroBrasil.update(bola);
    goleiroBrasil.resolverColisao(jogador.x, jogador.y, 0.2f);
    goleiroBrasil.resolverColisao(bola.x, bola.y, 0.1f);

    // i dispois resolve os gols por último
    gol.resolverColisao(jogador.x, jogador.y, 0.2f);
    
    // verifica o tipo do gol (1 = brasil, 2 = alemanha)
    int statusGol = gol.resolverColisao(bola.x, bola.y, 0.1f);
    
    if (statusGol == 1) {
        scoreboard.scoreBrazil();
        bola.x = 0.0f;
        bola.y = 0.0f;
        bola.isHeld = false;
    } else if (statusGol == 2) {
        // se o status for 2 pontua para a alemanha
        scoreboard.scoreGermany();
        bola.x = 0.0f;
        bola.y = 0.0f;
        bola.isHeld = false;
    }

    glutPostRedisplay();
}


// esses callbacks estáticos sao p/ conectar o c++ ao glut ne duuughhhh 
void Game::displayCallback() { Game::getInstance()->display(); }
void Game::reshapeCallback(int w, int h) { Game::getInstance()->reshape(w, h); }
void Game::mouseClickCallback(int button, int state, int x, int y) { Game::getInstance()->mouseClick(button, state, x, y); }
void Game::mousePassiveMotionCallback(int x, int y) { Game::getInstance()->mousePassiveMotion(x, y); }
void Game::keyboardClickCallback(unsigned char key, int x, int y) { Game::getInstance()->keyboardClick(key, x, y); }
void Game::keyboardUpCallback(unsigned char key, int x, int y) { Game::getInstance()->keyboardUp(key, x, y); }
void Game::idleCallback() { Game::getInstance()->updatePlayer(); }