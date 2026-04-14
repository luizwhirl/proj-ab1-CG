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
    goleiroRival = Goleiro(0.0f, 4.5f, 0.015f, -1.5f, 1.5f, true);
    goleiroAliado = Goleiro(0.0f, -4.5f, 0.015f, -1.5f, 1.5f, false);
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
    
    for (int x=0; x<timeAliado.size(); x++){
        // classe AnimacaoJogador carrega todos os sprites do jogador internamente
        // o loadtexture vai chamar ela dispois
        timeAliado[x].loadTexture();
    }
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
    for (int x=0; x<timeAliado.size(); x++){
        timeAliado[x].draw();
    }
    bola.draw();
    
    // desenha o goleiro
    goleiroRival.draw();
    goleiroAliado.draw();

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
    if (key == 'l' || key == 'L') input.isLPressed = true;
}

// ve se a tecla foi solto
void Game::keyboardUp(unsigned char key, int x, int y) {
    if (key == 'w' || key == 'W') input.isWPressed = false;
    if (key == 'a' || key == 'A') input.isAPressed = false;
    if (key == 's' || key == 'S') input.isSPressed = false;
    if (key == 'd' || key == 'D') input.isDPressed = false;
    if (key == 'j' || key == 'J') input.isJPressed = false;
    if (key == 'k' || key == 'K') input.isKPressed = false;
    if (key == 'l' || key == 'L') input.isLPressed = false;

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
    // o jogador só muda a direção pra qual está olhando se tiver a bola
    if (input.isWPressed) { 
        dirY += 1; 
        if (bola.isHeld) timeAliado[indiceJogador].lastDirection = 'W'; 
    }
    if (input.isSPressed) { 
        dirY -= 1; 
        if (bola.isHeld) timeAliado[indiceJogador].lastDirection = 'S'; 
    }
    if (input.isDPressed) { 
        dirX += 1; 
        if (bola.isHeld) timeAliado[indiceJogador].lastDirection = 'D'; 
    }
    if (input.isAPressed) { 
        dirX -= 1; 
        if (bola.isHeld) timeAliado[indiceJogador].lastDirection = 'A'; 
    }

    // normaliza vetor (eu deveria ter prestado atenção na aula)
    float magnitude = pitagoras(dirX, dirY);
    bool isMoving = false; // variav de controle para saber se jogador ta em movimento

    if (magnitude > 0) {
        dirX = dirX / magnitude;
        dirY = dirY / magnitude;
        isMoving = true; 
        // se magnitude > 0
        //  entao ele ta tentano se move
    }

    // grante que os outros jogadores do time que não estão 
    // sendo controlados parem a animação
    for (int x = 0; x < timeAliado.size(); x++) {
        timeAliado[x].setAndando(false);
    }
    
    // o jogador so anda e atualiza sua animação de movimento se estiver com a bola
    if (bola.isHeld) {
        // atualizao estado da animação do jogador atual com base no movimento
        timeAliado[indiceJogador].setAndando(isMoving);

        // faz o jogador correr
        float velocidadeJogador = 0.01f; 
        timeAliado[indiceJogador].x += dirX * velocidadeJogador;
        timeAliado[indiceJogador].y += dirY * velocidadeJogador;
    }

    // pega a bola se chegar perto
    if (!bola.isHeld) {
        // verifica TODOS os jogadores do time aliado, não apenas o atual
        for (int i = 0; i < timeAliado.size(); i++) {
            float distance = pitagoras(
                bola.x - timeAliado[i].x, 
                bola.y - timeAliado[i].y
            );
            // se algum jogador esbarrar na bola, ele a pega e vira o jogador controlado
            if (distance < 0.4 && bola.framesIntocavel == 0) {
                bola.velX = 0;
                bola.velY = 0;
                bola.isHeld = true;
                indiceJogador = i; 
                break; // achou um: não precisa continuar checando
            }
        }
    }

    // isso mesmo garotinho joga la bola para o papai
    if (input.isJPressed && !input.wasJPressed && bola.isHeld) {
        // achar o jogador mais proximo
        float menorDist = 100000000.0f;
        int maisProx = -1;
        for(int x=0; x < timeAliado.size(); x++){
            if(x != indiceJogador){
                float distancia = pitagoras(
                timeAliado[indiceJogador].x - timeAliado[x].x,
                timeAliado[indiceJogador].y - timeAliado[x].y
                );

                if (distancia < menorDist){
                    menorDist = distancia;
                    maisProx = x;
                }
            }
        }
        // torna a bola intocavel por uns frames durante o passe
        bola.framesIntocavel = 15;

        // calculamos a direção do jogador
        float vectorX = (timeAliado[maisProx].x - timeAliado[indiceJogador].x)/menorDist;
        float vectorY = (timeAliado[maisProx].y - timeAliado[indiceJogador].y)/menorDist;

        // calcula a velocidade da bola
        float velBola = menorDist * 0.02f;

        if(velBola < 0.04f){
            velBola = 0.04f;
        }
        // o chute é ajustado de acordo com a distância
        bola.velX += vectorX * (velBola);
        bola.velY += vectorY * (velBola);
        bola.isHeld = false;
        indiceJogador = maisProx;
    }

    // dá o chutão 
    // verifica !input.wasKPressed para o jogador que receber a bola não chutar imediatamente de volta
    if (input.isKPressed && !input.wasKPressed && bola.isHeld) {
        switch (timeAliado[indiceJogador].lastDirection) {
            case 'W': bola.velY += 0.05f; break;
            case 'A': bola.velX -= 0.05f; break;
            case 'S': bola.velY -= 0.05f; break;
            case 'D': bola.velX += 0.05f; break;
        }
        bola.isHeld = false;
        // deixa a bola intocável por alguns frames após o chute também, para desgrudar do pé
        bola.framesIntocavel = 10;
    }

    // troca de jogador
    if (input.isLPressed && !input.wasLPressed) {
        indiceJogador++;
        if (indiceJogador >= timeAliado.size()) {
            indiceJogador = 0;
        }
    }

    // o jogador sai driblando com a bola colada no pé
    if (bola.isHeld) {
        switch (timeAliado[indiceJogador].lastDirection) {
            case 'W':
                bola.x = timeAliado[indiceJogador].x;
                bola.y = timeAliado[indiceJogador].y + 0.4;
                break;
            case 'A':
                bola.x = timeAliado[indiceJogador].x - 0.4;
                bola.y = timeAliado[indiceJogador].y;
                break;
            case 'S':
                bola.x = timeAliado[indiceJogador].x;
                bola.y = timeAliado[indiceJogador].y - 0.4;
                break;
            case 'D':
                bola.x = timeAliado[indiceJogador].x + 0.4;
                bola.y = timeAliado[indiceJogador].y;
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
        
        // faz a bola ser controlada por conta própria quando solta
        // usamos o mesmo direcional de input (wasd) convertido em dirX/dirY
        float velocidadeBolaViva = 0.01f;
        bola.x += dirX * velocidadeBolaViva;
        bola.y += dirY * velocidadeBolaViva;
    }

    // resolvendo as colisões DEPOIS do update de teclas
    // primeiro os limites do campo para não deixar ngm sair
    campo.resolverColisaoLimites(timeAliado[indiceJogador].x, timeAliado[indiceJogador].y, 0.2f);
    campo.resolverColisaoLimites(bola.x, bola.y, 0.1f);

    // atualiza lógica do goleiro (movimento) e colisões dele com jogador e bola
    goleiroRival.update(bola);
    goleiroRival.resolverColisao(timeAliado[indiceJogador].x, timeAliado[indiceJogador].y, 0.2f);
    goleiroRival.resolverColisao(bola.x, bola.y, 0.1f);

    goleiroAliado.update(bola);
    goleiroAliado.resolverColisao(timeAliado[indiceJogador].x, timeAliado[indiceJogador].y, 0.2f);
    goleiroAliado.resolverColisao(bola.x, bola.y, 0.1f);

    // i dispois resolve os gols por último
    gol.resolverColisao(timeAliado[indiceJogador].x, timeAliado[indiceJogador].y, 0.2f);
    
    // verifica o tipo do gol (1 = brasil, 2 = alemanha)
    int statusGol = gol.resolverColisao(bola.x, bola.y, 0.1f);
    
    if (statusGol == 1) {
        scoreboard.scoreAliado();
        bola.x = 0.0f;
        bola.y = 0.0f;
        bola.velX = 0.0f; 
        bola.velY = 0.0f; 
        bola.isHeld = false;
    } else if (statusGol == 2) {
        // se o status for 2 pontua para a alemanha
        scoreboard.scoreRival();
        bola.x = 0.0f;
        bola.y = 0.0f;
        bola.velX = 0.0f;
        bola.velY = 0.0f;
        bola.isHeld = false;
    }

    input.wasJPressed = input.isJPressed;
    input.wasLPressed = input.isLPressed;
    input.wasKPressed = input.isKPressed;

    if (bola.framesIntocavel > 0){
        bola.framesIntocavel--;
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