#include "Game.h"
#include <GL/glut.h>
#include <cstdlib> // rand() e srand()
#include <ctime>   // time()
#include <stack>
#include <iostream>
#include <cmath>
#include <algorithm>  

Game* Game::instance = nullptr;

namespace {
// limita um valor para evitar exageros no deslocamento e nas velocidades.
float clampValor(float valor, float minimo, float maximo) {
    return std::max(minimo, std::min(valor, maximo));
}

// move o jogador até um alvo sem oscilar quando ele já chegou perto.
void moverJogadorPara(Jogador& jogador, float alvoX, float alvoY, float velocidade, float tolerancia = 0.05f) {
    float dirX = alvoX - jogador.x;
    float dirY = alvoY - jogador.y;
    float distancia = std::sqrt((dirX * dirX) + (dirY * dirY));

    if (distancia <= tolerancia || distancia <= 0.0001f) {
        return;
    }

    float passo = std::min(velocidade, distancia - tolerancia);
    jogador.x += (dirX / distancia) * passo;
    jogador.y += (dirY / distancia) * passo;
}

// atualiza direção e animação usando o deslocamento real do frame
void atualizarDirecaoEAnimacao(Jogador& jogador, float oldX, float oldY) {
    float diffX = jogador.x - oldX;
    float diffY = jogador.y - oldY;

    if (std::abs(diffX) > 0.0001f || std::abs(diffY) > 0.0001f) {
        jogador.setAndando(true);
        if (std::abs(diffX) > std::abs(diffY)) {
            jogador.lastDirection = (diffX > 0.0f) ? 'D' : 'A';
        } else {
            jogador.lastDirection = (diffY > 0.0f) ? 'W' : 'S';
        }
    } else {
        jogador.setAndando(false);
    }
}

// Mede o quão marcado um jogador está pelo time adversário.
float distanciaMaisProxima(const Jogador& referencia, const std::vector<Jogador>& adversarios) {
    float menorDistancia = 1000000.0f;

    for (const Jogador& adversario : adversarios) {
        float dx = referencia.x - adversario.x;
        float dy = referencia.y - adversario.y;
        float distancia = std::sqrt((dx * dx) + (dy * dy));
        menorDistancia = std::min(menorDistancia, distancia);
    }

    return menorDistancia;
}

// Escolhe um companheiro livre e mais adiantado para receber o passe rival.
int escolherMelhorPasseRival(const std::vector<Jogador>& rivais, const std::vector<Jogador>& aliados, int portador) {
    int melhorOpcao = -1;
    float melhorScore = -1000000.0f;

    // procura seus amiguinhos
    for (int i = 0; i < static_cast<int>(rivais.size()); i++) {
        if (i == portador) {
            continue;
        }

        // só considera companheiros numa faixa razoável de passe
        float dx = rivais[i].x - rivais[portador].x;
        float dy = rivais[i].y - rivais[portador].y;
        float distancia = std::sqrt((dx * dx) + (dy * dy));

        
        if (distancia < 0.8f || distancia > 3.2f) {
            continue;
        }

        if (rivais[i].y >= rivais[portador].y - 0.15f) {
            continue;
        }

        // quanto mais livre e mais perto do gol, maior a nota do passe
        float coberturaAliada = distanciaMaisProxima(rivais[i], aliados);
        float progressoAoGol = rivais[portador].y - rivais[i].y;
        float score = (progressoAoGol * 1.8f) + (coberturaAliada * 0.9f) - (distancia * 0.35f) - (std::abs(rivais[i].x) * 0.15f);

        if (score > melhorScore) {
            melhorScore = score;
            melhorOpcao = i;
        }
    }

    return melhorOpcao;
}

// seleciona os dois jogadores que realmente vão pressionar a bola.
void selecionarPerseguidores(const std::vector<Jogador>& time, const Bola& bola, int ignorarIndex, int& maisProximo, int& segundoMaisProximo) {
    float menorDistancia = 1000000.0f;
    float segundaMenorDistancia = 1000000.0f;
    maisProximo = -1;
    segundoMaisProximo = -1;

    // ignora quem já está com a posse para evitar perseguição falsa.
    for (int i = 0; i < static_cast<int>(time.size()); i++) {
        if (i == ignorarIndex) {
            continue;
        }

        float dx = bola.x - time[i].x;
        float dy = bola.y - time[i].y;
        float distancia = std::sqrt((dx * dx) + (dy * dy));

        // salva os mais proximos
        if (distancia < menorDistancia) {
            segundaMenorDistancia = menorDistancia;
            segundoMaisProximo = maisProximo;
            menorDistancia = distancia;
            maisProximo = i;
        } else if (distancia < segundaMenorDistancia) {
            segundaMenorDistancia = distancia;
            segundoMaisProximo = i;
        }
    }
}
}

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
        // força os rivais a nascerem olhando para o sul e estaticos
        timeRival[i].lastDirection = 'S'; 
        timeRival[i].setAndando(false);
    }

    // printa o time aliado
    for (int i = 0; i < 10; i++) {
        timeAliado.push_back(Jogador(posicoes442[i].first, posicoes442[i].second));
        // garante que os aliados também iniciem de forma estática
        timeAliado[i].setAndando(false);
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
    campo.loadArquibancadaTextures("assets/sprites/arquibancada/arquibancadaA.png", 
                                    "assets/sprites/arquibancada/ArquibancadaB.png", 
                                    "assets/sprites/arquibancada/ArquibancadaC.png", 
                                    "assets/sprites/arquibancada/ArquibancadaD.png");
    
    // carrega os sprites de animacao da bola junto com os carregamentos do jogo
    bola.loadTextures();

    // chamada para carregar as novas texturas da bota e escudo do PowerUp
    powerUp.loadTextures();

    for (int x=0; x<timeAliado.size(); x++){
        // classe AnimacaoJogador carrega todos os sprites do jogador internamente
        // o loadtexture vai chamar ela dispois
        // seta "false" indicando ser aliado
        timeAliado[x].loadTexture(false);
    }
    
    // o for para grarante o carregamento das texturas do time rival com a flag true
    for (int x=0; x<timeRival.size(); x++){
        timeRival[x].loadTexture(true); 
    }
    goleiroRival.loadTexture();
    goleiroAliado.loadTexture();
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

    // pensando em alterar esse zoom aqui hein
    if (input.isZoomed) {
        float zoomFactor = 0.35f;
        float viewWidth = (right - left) * zoomFactor;
        float viewHeight = (top - bottom) * zoomFactor;

        // variáveis para definir o alvo da câmera
        float alvoX, alvoY;

        // lógica para a câmera seguir o jogador COM a bola ou a BOLA SOLTAAA
        if (bola.statusPosse == 1) { // 1 = time aliado
            // if -> alguém pegou a bola: a câmera foca nas coordenadas do jogador atual aliado
            alvoX = timeAliado[indiceJogador].x;
            alvoY = timeAliado[indiceJogador].y;
        } else if (bola.statusPosse == 2 && bola.idRival >= 0 && bola.idRival < timeRival.size()) { // 2 = time rival
            // a câmera também sabe seguir o rival caso ele esteja conduzindo a bola
            alvoX = timeRival[bola.idRival].x;
            alvoY = timeRival[bola.idRival].y;
        } else {
            // if-> a bola tá solta (statusPosse == 0): foca na bola
            alvoX = bola.x;
            alvoY = bola.y;
        }

        left = alvoX - viewWidth / 2.0f;
        right = alvoX + viewWidth / 2.0f;
        bottom = alvoY - viewHeight / 2.0f;
        top = alvoY + viewHeight / 2.0f;
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

    // chama o desenho do item caso esteja em campo
    powerUp.draw(); 

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
        
        glutPostRedisplay();
    }
}

// passivo igual lucas (????????)

// verifica se a tecla foi clicada
void Game::keyboardClick(unsigned char key, int x, int y) {
    if (key == 'w' || key == 'W') input.isWPressed = true;
    if (key == 'a' || key == 'A') input.isAPressed = true;
    if (key == 's' || key == 'S') input.isSPressed = true;
    if (key == 'd' || key == 'D') input.isDPressed = true;
    if (key == 'j' || key == 'J') input.isJPressed = true;
    if (key == 'k' || key == 'K') input.isKPressed = true;
    if (key == 'l' || key == 'L') input.isLPressed = true;
    // regra antiga> 5 toques em K fazem o rival largar a bola
    // suporte para 'espaço' tbm soltar a bola
    if (bola.statusPosse == 2 && (key == 'k' || key == 'K' || key == ' ')) {
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

// immplementa  sistema de colisão para jogadores
void Game::resolverColisoesJogadores() {
    float raioJogador = 0.2f; // raio considerado para cada jogador
    float distanciaMinima = raioJogador * 2.0f; // distância mínima entre os centros (0.4f)

    // agrupa todos os jogadores em um vetor de ponteiros para testar todos contra todos
    std::vector<Jogador*> todosJogadores;
    for (int i = 0; i < timeAliado.size(); i++) {
        todosJogadores.push_back(&timeAliado[i]);
    }
    for (int i = 0; i < timeRival.size(); i++) {
        todosJogadores.push_back(&timeRival[i]);
    }

    // compara cada jogador com todos os outros para evitar entrar um no outro
    for (size_t i = 0; i < todosJogadores.size(); i++) {
        for (size_t j = i + 1; j < todosJogadores.size(); j++) {
            Jogador* j1 = todosJogadores[i];
            Jogador* j2 = todosJogadores[j];

            float dx = j2->x - j1->x;
            float dy = j2->y - j1->y;
            float dist = pitagoras(dx, dy);

            // verifica se estão se sobrepondo (com tolerância pequena para evitar divisões por zero)
            if (dist < distanciaMinima && dist > 0.0001f) {
                float sobreposicao = distanciaMinima - dist;
                
                // normalizando a direção
                float nx = dx / dist;
                float ny = dy / dist;

                // empurra metade da sobreposição para cada lado, separando fisicamente os corpos
                j1->x -= nx * (sobreposicao / 2.0f);
                j1->y -= ny * (sobreposicao / 2.0f);
                j2->x += nx * (sobreposicao / 2.0f);
                j2->y += ny * (sobreposicao / 2.0f);
            }
        }
    }
}


void Game::atualizarIARival(){
    // força a perda da posse rival depois dos 5 cliques em K.
    if (bola.statusPosse == 2 && cliquesParaSoltar >= 5) {
        bola.statusPosse = 0;
        bola.idRival = -1;
        bola.velX = 0.0f;
        bola.velY = 0.05f;
        bola.framesIntocavel = 30;
        cooldownPasseRival = 20;
        cliquesParaSoltar = 0;
    }

    // IA do rival (botei 10 macacos pra controlar o time)
    int rivalMaisProximo = -1;
    int segundoRivalMaisProximo = -1;
    selecionarPerseguidores(timeRival, bola, -1, rivalMaisProximo, segundoRivalMaisProximo);

    for (int i = 0; i < static_cast<int>(timeRival.size()); i++) {
        float oldX = timeRival[i].x;
        float oldY = timeRival[i].y;

        // se a bola está com o time rival
        if (bola.statusPosse == 2 && bola.idRival == i) {
            float dirGolX = 0.0f - timeRival[i].x;
            float dirGolY = -4.5f - timeRival[i].y;
            float distProGol = pitagoras(dirGolX, dirGolY);

            // se estiver pressionado, tenta antes o passe e só depois a corrida.
            if (cooldownPasseRival == 0) {
                int melhorPasse = escolherMelhorPasseRival(timeRival, timeAliado, i);
                float pressaoMaisProxima = distanciaMaisProxima(timeRival[i], timeAliado);
                bool estaPressionado = pressaoMaisProxima < 0.9f;
                bool travadoNaLateral = std::abs(timeRival[i].x) > 2.2f;

                if (melhorPasse != -1 && (estaPressionado || travadoNaLateral)) {
                    float passeX = timeRival[melhorPasse].x - timeRival[i].x;
                    float passeY = timeRival[melhorPasse].y - timeRival[i].y;
                    float distPasse = pitagoras(passeX, passeY);

                    if (distPasse > 0.0001f) {
                        float vetorPasseX = passeX / distPasse;
                        float vetorPasseY = passeY / distPasse;
                        float velPasse = clampValor(distPasse * 0.018f, 0.035f, 0.06f);

                        bola.statusPosse = 0;
                        bola.idRival = -1;
                        bola.framesIntocavel = 18;
                        bola.x = timeRival[i].x + vetorPasseX * 0.35f;
                        bola.y = timeRival[i].y + vetorPasseY * 0.35f;
                        bola.velX = vetorPasseX * velPasse;
                        bola.velY = vetorPasseY * velPasse;
                        cooldownPasseRival = 45;
                    }
                }
            }

            // sem passe, o portador tenta conduzir até a zona de finalização.
            if (bola.statusPosse == 2 && bola.idRival == i) {
                if (distProGol > 1.35f) {
                    moverJogadorPara(timeRival[i], 0.0f, -4.5f, 0.0028f, 0.15f);
                    bola.x = timeRival[i].x;
                    bola.y = timeRival[i].y - 0.4f;
                } else {
                    // perto do gol, finaliza em vez de continuar carregando.
                    bola.statusPosse = 0;
                    bola.idRival = -1;
                    bola.framesIntocavel = 12;
                    bola.x = timeRival[i].x;
                    bola.y = timeRival[i].y - 0.4f;
                    bola.velX = clampValor(-timeRival[i].x * 0.01f, -0.02f, 0.02f);
                    bola.velY = -0.05f;
                    cooldownPasseRival = 20;
                }
            }
        } else if (bola.statusPosse == 2 && bola.idRival >= 0 && bola.idRival < static_cast<int>(timeRival.size())) {
            // quem não está com a bola acompanha a jogada sem abandonar a formação.
            float avancoTime = clampValor((timeRival[bola.idRival].baseY - timeRival[bola.idRival].y) * 0.45f, 0.0f, 1.2f);
            float alvoX = timeRival[i].baseX + clampValor((timeRival[bola.idRival].x - timeRival[i].baseX) * 0.35f, -0.9f, 0.9f);
            float alvoY = timeRival[i].baseY - avancoTime;
            moverJogadorPara(timeRival[i], alvoX, alvoY, 0.0022f);
            
        } else if ((bola.statusPosse == 0 || bola.statusPosse == 1) && bola.framesIntocavel == 0) {
            float distPraBola = pitagoras(bola.x - timeRival[i].x, bola.y - timeRival[i].y);
            // só os dois mais próximos dão o bote, o resto fecha espaço.
            bool devePressionar = (i == rivalMaisProximo || i == segundoRivalMaisProximo) && distPraBola < 3.5f;

            if (distPraBola < 0.4f) {
                // verifica invencibilidade antes de roubar
                if (tempoInvincibilidade > 0 && bola.statusPosse == 1) {
                    // não rouba, a invencibilidade não permite
                } else {
                    bola.statusPosse = 2;
                    bola.idRival = i;
                    bola.velX = 0.0f;
                    bola.velY = 0.0f;
                    cliquesParaSoltar = 0;
                    cooldownPasseRival = 20;
                }
            } else if (devePressionar || distPraBola < 1.2f) {
                moverJogadorPara(timeRival[i], bola.x, bola.y, 0.0030f, 0.08f);
            } else {
                float alvoX = timeRival[i].baseX + clampValor((bola.x - timeRival[i].baseX) * 0.18f, -0.5f, 0.5f);
                float alvoY = timeRival[i].baseY + clampValor((bola.y - timeRival[i].baseY) * 0.08f, -0.35f, 0.2f);
                moverJogadorPara(timeRival[i], alvoX, alvoY, 0.0025f);
            }
        } else {
            moverJogadorPara(timeRival[i], timeRival[i].baseX, timeRival[i].baseY, 0.0025f);
        }

        atualizarDirecaoEAnimacao(timeRival[i], oldX, oldY);
    }

    if (cooldownPasseRival > 0) {
        cooldownPasseRival--;
    }
}

void Game::atualizarIATime(){
    int aliadoMaisProximo = -1;
    int segundoAliadoMaisProximo = -1;
    selecionarPerseguidores(timeAliado, bola, indiceJogador, aliadoMaisProximo, segundoAliadoMaisProximo);


    // botei mais 10 macacos aqui
    for (int i = 0; i < static_cast<int>(timeAliado.size()); i++) {
        if (i == indiceJogador) {
            continue;
        }

        float oldX = timeAliado[i].x;
        float oldY = timeAliado[i].y;

        if (bola.statusPosse == 1) {
            // os aliados acompanham o ataque, mas sem disparar todos para o gol.
            float avancoTime = clampValor((timeAliado[indiceJogador].y - timeAliado[indiceJogador].baseY) * 0.45f, 0.0f, 1.2f);
            float alvoX = timeAliado[i].baseX + clampValor((timeAliado[indiceJogador].x - timeAliado[i].baseX) * 0.35f, -0.9f, 0.9f);
            float alvoY = timeAliado[i].baseY + avancoTime;
            moverJogadorPara(timeAliado[i], alvoX, alvoY, 0.0025f);
        } else if ((bola.statusPosse == 0 || bola.statusPosse == 2) && bola.framesIntocavel == 0) {
            float distPraBola = pitagoras(bola.x - timeAliado[i].x, bola.y - timeAliado[i].y);
            // só dois aliados pressionam forte para evitar desorganizar o time
            bool devePressionar = (i == aliadoMaisProximo || i == segundoAliadoMaisProximo) && distPraBola < 3.5f;

            if (distPraBola < 0.4f) {
                bola.velX = 0.0f;
                bola.velY = 0.0f;
                bola.statusPosse = 1;
                bola.idRival = -1;
                indiceJogador = i;
            } else if (devePressionar || distPraBola < 1.2f) {
                moverJogadorPara(timeAliado[i], bola.x, bola.y, 0.0032f, 0.08f);
            } else {
                float alvoX = timeAliado[i].baseX + clampValor((bola.x - timeAliado[i].baseX) * 0.18f, -0.5f, 0.5f);
                float alvoY = timeAliado[i].baseY + clampValor((bola.y - timeAliado[i].baseY) * 0.10f, -0.2f, 0.35f);
                moverJogadorPara(timeAliado[i], alvoX, alvoY, 0.0025f);
            }
        } else {
            moverJogadorPara(timeAliado[i], timeAliado[i].baseX, timeAliado[i].baseY, 0.0025f);
        }

        atualizarDirecaoEAnimacao(timeAliado[i], oldX, oldY);
    }
}



// Atualiza a posição do jogador e da bola
void Game::updatePlayer() {
    // contagem e respawn do power up
    if (tempoSpeedBoost > 0) tempoSpeedBoost--;
    if (tempoInvincibilidade > 0) tempoInvincibilidade--;

    if (!powerUp.active) {
        spawnTimer++;
        if (spawnTimer >= 300) { // depois de aprox 5s spawna oto
            powerUp.spawn();
            spawnTimer = 0;
        }
    } else {
        // Checa se o aliado controlado colidiu com o powerup
        if (powerUp.checkCollision(timeAliado[indiceJogador].x, timeAliado[indiceJogador].y)) {
            if (powerUp.type == 1) {
                tempoSpeedBoost = 2400; // 10 segundos a 60 fps (40s - 2400)
            } else if (powerUp.type == 2) {
                tempoInvincibilidade = 2400; // 10 segundos a 60 fps (tambem)
            }
            powerUp.active = false;
        }
    }

    float dirX = 0;
    float dirY = 0;

    // verifica a tecla pressionada e salva a ultima posição
    // o jogador só muda a direção pra qual está olhando se tiver a bola
    if (input.isWPressed) { 
        dirY += 1; 
        if (bola.statusPosse == 1) timeAliado[indiceJogador].lastDirection = 'W'; 
    }
    if (input.isSPressed) { 
        dirY -= 1; 
        if (bola.statusPosse == 1) timeAliado[indiceJogador].lastDirection = 'S'; 
    }
    if (input.isDPressed) { 
        dirX += 1; 
        if (bola.statusPosse == 1) timeAliado[indiceJogador].lastDirection = 'D'; 
    }
    if (input.isAPressed) { 
        dirX -= 1; 
        if (bola.statusPosse == 1) timeAliado[indiceJogador].lastDirection = 'A'; 
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
        atualizarIATime();
    }

    // grante que os outros jogadores do time que não estão 
    // sendo controlados parem a animação
    timeAliado[indiceJogador].setAndando(false);
    
    // o jogador so anda e atualiza sua animação de movimento se estiver com a bola
    if (bola.statusPosse == 1) {
        // atualizao estado da animação do jogador atual com base no movimento
        timeAliado[indiceJogador].setAndando(isMoving);

        // faz o jogador correr
        // aplica a nova velocidade drobada caso o speed boost esteja on
        float velocidadeJogador = (tempoSpeedBoost > 0) ? 0.02f : 0.01f; 
        timeAliado[indiceJogador].x += dirX * velocidadeJogador;
        timeAliado[indiceJogador].y += dirY * velocidadeJogador;
    }

    // pega a bola se chegar perto
    if (bola.statusPosse == 0) {
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
                bola.statusPosse = 1;
                bola.idRival = -1;
                indiceJogador = i; 
                break; // achou um: não precisa continuar checando
            }
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

        // O passe nasce já apontado para o companheiro escolhido.
        float vectorX = (timeAliado[maisProx].x - timeAliado[indiceJogador].x)/menorDist;
        float vectorY = (timeAliado[maisProx].y - timeAliado[indiceJogador].y)/menorDist;

        // calcula a velocidade da bola
        float velBola = menorDist * 0.02f;

        if(velBola < 0.04f){
            velBola = 0.04f;
        }
        // A bola sai um pouco à frente para não ficar presa no pé de origem.
        bola.x = timeAliado[indiceJogador].x + vectorX * 0.35f;
        bola.y = timeAliado[indiceJogador].y + vectorY * 0.35f;
        bola.velX = vectorX * (velBola);
        bola.velY = vectorY * (velBola);
        bola.statusPosse = 0;
        bola.idRival = -1;
        indiceJogador = maisProx;
    }

    // dá o chutão 
    // verifica !input.wasKPressed para o jogador que receber a bola não chutar imediatamente de volta neh pai
    if (input.isKPressed && !input.wasKPressed && bola.statusPosse == 1) {
        bola.velX = 0.0f;
        bola.velY = 0.0f;
        switch (timeAliado[indiceJogador].lastDirection) {
            case 'W': bola.velY += 0.05f; break;
            case 'A': bola.velX -= 0.05f; break;
            case 'S': bola.velY -= 0.05f; break;
            case 'D': bola.velX += 0.05f; break;
        }
        bola.statusPosse = 0;
        bola.idRival = -1;
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

        // Mantém a mecânica original de empurrar a bola livre com WASD.
        float velocidadeBolaViva = 0.01f;
        bola.x += dirX * velocidadeBolaViva;
        bola.y += dirY * velocidadeBolaViva;
    }

    // resolvendo as colisões DEPOIS do update de teclas
    // primeiro os limites do campo para não deixar ngm sair
    for (int i = 0; i < static_cast<int>(timeAliado.size()); i++) {
        campo.resolverColisaoLimites(timeAliado[i].x, timeAliado[i].y, 0.2f);
    }
    for (int i = 0; i < static_cast<int>(timeRival.size()); i++) {
        campo.resolverColisaoLimites(timeRival[i].x, timeRival[i].y, 0.2f);
    }
    campo.resolverColisaoLimites(bola.x, bola.y, 0.1f);

    // afasta os jogadores que estejam esbarrando ou se sobrepondo em campo
    resolverColisoesJogadores();

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
        // limpa os power up no reset do gol
        powerUp.active = false;
        spawnTimer = 0;
        tempoSpeedBoost = 0;
        tempoInvincibilidade = 0;

        scoreboard.scoreAliado();
        bola.x = 0.0f;
        bola.y = 0.0f;
        bola.velX = 0.0f; 
        bola.velY = 0.0f;
        jogoIniciado = false; 
        bola.statusPosse = 0;
        bola.idRival = -1;
        cooldownPasseRival = 0;
        cliquesParaSoltar = 0;
        for (int i = 0; i < timeRival.size(); i++) {
            timeRival[i].x = timeRival[i].baseX;
            timeRival[i].y = timeRival[i].baseY;
            // reseta a direção dos rivais para o sul e os deixa estáticos
            timeRival[i].lastDirection = 'S';
            timeRival[i].setAndando(false);
        }
        for (int i = 0; i < timeAliado.size(); i++) {
            timeAliado[i].x = timeAliado[i].baseX;
            timeAliado[i].y = timeAliado[i].baseY;
            // reseta aliados para o norte e tira a animação 
            timeAliado[i].lastDirection = 'W';
            timeAliado[i].setAndando(false);
        }
        indiceJogador = 0;
    } else if (statusGol == 2) {
        // limpa os power up no reset do gol
        powerUp.active = false;
        spawnTimer = 0;
        tempoSpeedBoost = 0;
        tempoInvincibilidade = 0;

        // se o status for 2 pontua para a alemanha
        scoreboard.scoreRival();
        bola.x = 0.0f;
        bola.y = 0.0f;
        bola.velX = 0.0f;
        bola.velY = 0.0f;
        jogoIniciado = false;
        bola.statusPosse = 0;
        bola.idRival = -1;
        cooldownPasseRival = 0;
        cliquesParaSoltar = 0;
        for (int i = 0; i < timeRival.size(); i++) {
            timeRival[i].x = timeRival[i].baseX;
            timeRival[i].y = timeRival[i].baseY;
            // reseta a direção dos rivais para o sul e os deixa estáticos
            timeRival[i].lastDirection = 'S';
            timeRival[i].setAndando(false);
        }
        for (int i = 0; i < timeAliado.size(); i++) {
            timeAliado[i].x = timeAliado[i].baseX;
            timeAliado[i].y = timeAliado[i].baseY;
            // reseta aliados para o nortee tira a animação
            timeAliado[i].lastDirection = 'W';
            timeAliado[i].setAndando(false);
        }
        indiceJogador = 0;
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
void Game::keyboardClickCallback(unsigned char key, int x, int y) { Game::getInstance()->keyboardClick(key, x, y); }
void Game::keyboardUpCallback(unsigned char key, int x, int y) { Game::getInstance()->keyboardUp(key, x, y); }
void Game::idleCallback() { Game::getInstance()->updatePlayer(); }