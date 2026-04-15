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

    // define a posicao dos jogadores em uma formacao 4-4-2 (a melhor digasssi di passagi)
    std::vector<std::pair<float, float>> posicoes442 = {
    {-1.0f, -0.5f}, {1.0f, -0.5f},                                // Ataque
    {-2.0f, -1.5f}, {-0.8f, -2.0f}, {0.8f, -2.0f}, {2.0f, -1.5f}, // Meio  
    {-2.0f, -3.5f}, {-0.8f, -4.0f}, {0.8f, -4.0f}, {2.0f, -3.5f}, // Defesa          
    };

    // printa o time rival
    for (int i = 0; i < 10; i++) {
        timeRival.push_back(Jogador(posicoes442[i].first * -1.0f, posicoes442[i].second * -1.0f));
    }

    // printa o time aliado
    for (int i = 0; i < 10; i++) {
        timeAliado.push_back(Jogador(posicoes442[i].first, posicoes442[i].second));
    }

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

    // jogadores e bola vêm primeiro
    for (int x=0; x<timeAliado.size(); x++){
        timeRival[x].draw();
    }

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
    if (bola.statusPosse == 2 && key == ' ') {
        cliquesParaSoltar++;
    }
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

void Game::atualizarIARival(){
   if (bola.statusPosse == 2 && cliquesParaSoltar >= 10) {
    bola.statusPosse = 0; // bola fica livre
    bola.velY = 0.05f; // afasta a bola um pouquinho do rival
    bola.framesIntocavel = 30; 
    cliquesParaSoltar = 0; 
}

//  IA do time rival (botei 10 macacos pra controlar o time)

for (int i = 0; i < timeRival.size(); i++) {
    
    // se o rival está com a bola ele ataca
    if (bola.statusPosse == 2 && bola.idRival == i) {
        
        // ataca para o gol debaixo
        float dirX = 0.0f - timeRival[i].x;
        float dirY = -4.5f - timeRival[i].y;
        float distProGol = pitagoras(dirX, dirY);

        if (distProGol > 0) {
            // rival corre pro gol
            timeRival[i].x += (dirX / distProGol) * 0.002f; 
            timeRival[i].y += (dirY / distProGol) * 0.002f;
        }

        // A bola gruda no pé dele
        bola.x = timeRival[i].x;
        bola.y = timeRival[i].y - 0.4f; 

        // chegou na área, daqui é caixa
        if (distProGol < 1.5f) {
            bola.statusPosse = 0;
            bola.velY = -0.05f;
        }
    } 

    

    // se a bola está livre ou com o jogador eles tenteam roubar
    else if ((bola.statusPosse == 0 || bola.statusPosse == 1) && bola.framesIntocavel == 0) {
        float distPraBola = pitagoras(bola.x - timeRival[i].x, bola.y - timeRival[i].y);
        // chegou perto rouba a bola
        if (distPraBola < 0.4f) {
            bola.statusPosse = 2; 
            bola.idRival = i;
            cliquesParaSoltar = 0;
        } 
        // Se ainda tá longe, mas tá no campo de visão , persegue a bola
        else if (distPraBola < 2.0f) {
            float dirX = bola.x - timeRival[i].x;
            float dirY = bola.y - timeRival[i].y;
            timeRival[i].x += (dirX / distPraBola) * 0.0009f; 
            timeRival[i].y += (dirY / distPraBola) * 0.0009f;
        }
        else {
            float dirXBase = timeRival[i].baseX - timeRival[i].x;
            float dirYBase = timeRival[i].baseY - timeRival[i].y;
            float distPraBase = pitagoras(dirXBase, dirYBase);
            
            if (distPraBase > 0.05f) {
                timeRival[i].x += (dirXBase / distPraBase) * 0.004f; 
                timeRival[i].y += (dirYBase / distPraBase) * 0.004f;
            }
        }
    }

    else{
        float dirXBase = timeRival[i].baseX - timeRival[i].x;
        float dirYBase = timeRival[i].baseY - timeRival[i].y;
        float distPraBase = pitagoras(dirXBase, dirYBase);
        
        if (distPraBase > 0.05f) {
            timeRival[i].x += (dirXBase / distPraBase) * 0.004f; 
            timeRival[i].y += (dirYBase / distPraBase) * 0.004f;
        }
    }
}
}

// Atualiza a posição do jogador e da bola
void Game::updatePlayer() {
    float dirX = 0;
    float dirY = 0;

    // verifica a tecla pressionada e salva a ultima posição
    if (input.isWPressed) { 
        dirY += 1; 
        timeAliado[indiceJogador].lastDirection = 'W'; 
    }
    if (input.isSPressed) { 
        dirY -= 1; 
        timeAliado[indiceJogador].lastDirection = 'S'; 
    }
    if (input.isDPressed) { 
        dirX += 1; 
        timeAliado[indiceJogador].lastDirection = 'D'; 
    }
    if (input.isAPressed) { 
        dirX -= 1; 
        timeAliado[indiceJogador].lastDirection = 'A'; 
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

    // começa o jogo quando dá um clique
    if (isMoving) {
        jogoIniciado = true;
    }

    if (jogoIniciado == true) {
        atualizarIARival();
    }

    // grante que os outros jogadores do time que não estão 
    // sendo controlados parem a animação
    for (int x = 0; x < timeAliado.size(); x++) {
        timeAliado[x].setAndando(false);
    }
    
    // atualizao estado da animação do jogador atual com base no movimento
    timeAliado[indiceJogador].setAndando(isMoving);

    // faz o jogador correr
    float velocidadeJogador = 0.005f; 
    timeAliado[indiceJogador].x += dirX * velocidadeJogador;
    timeAliado[indiceJogador].y += dirY * velocidadeJogador;

    // pega a bola se chegar perto
    if (bola.statusPosse == 0) {
        float distance = pitagoras(
            bola.x - timeAliado[indiceJogador].x, 
            bola.y - timeAliado[indiceJogador].y
        );
        if (distance < 0.4 && bola.framesIntocavel == 0) {
            bola.velX = 0;
            bola.velY = 0;
            bola.statusPosse = 1;
        }
    }

    // isso mesmo garotinho joga la bola para o papai
    if (input.isJPressed && !input.wasJPressed && bola.statusPosse == 1) {
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
        bola.statusPosse = 0;
        indiceJogador = maisProx;
    }

    // dá o chutão 
    if (input.isKPressed && bola.statusPosse == 1) {
        switch (timeAliado[indiceJogador].lastDirection) {
            case 'W': bola.velY += 0.05f; break;
            case 'A': bola.velX -= 0.05f; break;
            case 'S': bola.velY -= 0.05f; break;
            case 'D': bola.velX += 0.05f; break;
        }
        bola.statusPosse = 0;
    }

    // troca de jogador
    if (input.isLPressed && !input.wasLPressed) {
        indiceJogador++;
        if (indiceJogador >= timeAliado.size()) {
            indiceJogador = 0;
        }
    }

    // o jogador sai driblando com a bola colada no pé
    if (bola.statusPosse == 1) {
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

    if (bola.statusPosse == 0) {
        // adiciona aceleração a bola
        bola.x += bola.velX;
        bola.y += bola.velY;

        // bola vai freiando na grama (papo de fisica)
        bola.velX *= 0.98f;
        bola.velY *= 0.98f;
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
        jogoIniciado = false; 
        bola.statusPosse = 0;
        for (int i = 0; i < timeRival.size(); i++) {
            timeRival[i].x = timeRival[i].baseX;
            timeRival[i].y = timeRival[i].baseY;
        }
        for (int i = 0; i < timeAliado.size(); i++) {
            timeAliado[i].x = timeAliado[i].baseX;
            timeAliado[i].y = timeAliado[i].baseY;
        }
        indiceJogador = 0;
    } else if (statusGol == 2) {
        // se o status for 2 pontua para a alemanha
        scoreboard.scoreRival();
        bola.x = 0.0f;
        bola.y = 0.0f;
        bola.velX = 0.0f;
        bola.velY = 0.0f;
        jogoIniciado = false;
        bola.statusPosse = 0;
        for (int i = 0; i < timeRival.size(); i++) {
            timeRival[i].x = timeRival[i].baseX;
            timeRival[i].y = timeRival[i].baseY;
        }
        for (int i = 0; i < timeAliado.size(); i++) {
            timeAliado[i].x = timeAliado[i].baseX;
            timeAliado[i].y = timeAliado[i].baseY;
        }
        indiceJogador = 0;
    }

    input.wasJPressed = input.isJPressed;
    input.wasLPressed = input.isLPressed;

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