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
    campo.loadArquibancadaTexture("assets/sprites/arquibancadaA.png");
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

    // os gols sao desenhados POR ÚLTIMO pra rede e o topo aparecerem ACIMA deles visualmente
    gol.draw();

    scoreboard.draw(winW, winH);

    // butaos de teste
    scoreboard.drawTestButtons(winW, winH);

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
        
        // mas primeiro vamos checar os botoes de teste (scoreboard resolve e checa a logica)
        if (scoreboard.checkButtonLeftClick(x, y, winW, winH) || scoreboard.checkButtonRightClick(x, y, winW, winH)) {
            glutPostRedisplay();
            return;
        }

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
}

// ve se a tecla foi solto
void Game::keyboardUp(unsigned char key, int x, int y) {
    if (key == 'w' || key == 'W') input.isWPressed = false;
    if (key == 'a' || key == 'A') input.isAPressed = false;
    if (key == 's' || key == 'S') input.isSPressed = false;
    if (key == 'd' || key == 'D') input.isDPressed = false;
    if (key == 'j' || key == 'J') input.isJPressed = false;
}


void Game::updatePlayer(){
     std::stack<char> teclas;
 
    if(input.isWPressed == true ) {
        teclas.push('W');
    }

    if(input.isAPressed == true ){
        teclas.push('A');
    }

    if(input.isSPressed == true ){
        teclas.push('S');
    }

    if(input.isDPressed == true ){
        teclas.push('D');
    }

    if (!bola.isHeld){
        float catetoAdj = (bola.x - jogador.x);
        float catetoOpos = (bola.y - jogador.y);
        float distance = sqrt(pow(catetoAdj, 2) + pow(catetoOpos, 2));

        if (distance < 0.5){
            bola.isHeld = true;
            bola.x = bola.x+0.07;
            bola.y = bola.y+0.07;
        }
    }

    if(input.isJPressed == true ){
        bola.isHeld = false;
    }

    while(!teclas.empty()){
        char tecla = teclas.top();
        jogador.update(tecla);
        if(bola.isHeld){
            bola.update(tecla);
        }
        teclas.pop();
    }

    // Resolvendo as colisões DEPOIS do update de teclas
    // Primeiro os limites do campo para não deixar ngm sair
    campo.resolverColisaoLimites(jogador.x, jogador.y, 0.2f);
    campo.resolverColisaoLimites(bola.x, bola.y, 0.1f);

    // Em seguida resolve os gols por último
    gol.resolverColisao(jogador.x, jogador.y, 0.2f);
    gol.resolverColisao(bola.x, bola.y, 0.1f);

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