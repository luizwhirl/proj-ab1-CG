#ifndef GAME_H
#define GAME_H

#include <vector>
#include "Campo.h"
#include "Gol.h"
#include "Scoreboard.h"
#include "InputManager.h"
#include "Bola.h"
#include "Jogador.h"
#include "Goleiro.h"

// Scene / Game State e Window / Context Manager
class Game {
private:
    int winW, winH;
    int indiceJogador = 0;

    Campo campo;
    Gol gol;
    Scoreboard scoreboard;
    InputManager input;
    Bola bola;
    std::vector<Jogador> timeAliado;
    std::vector<Jogador> timeRival;
    Goleiro goleiro;
    Goleiro goleiroRival; // di cima
    Goleiro goleiroAliado;   // di baxo

    static Game* instance;

    void setupCamera();

public:
    Game();
    
    void init();
    void display();
    void reshape(int w, int h);
    void mouseClick(int button, int state, int x, int y);
    void mousePassiveMotion(int x, int y);
    void keyboardClick(unsigned char key, int x, int y);
    void keyboardUp(unsigned char key, int x, int y);
    void updatePlayer();
    float pitagoras(float, float);

    static Game* getInstance();

    // callbacks estáticos pro glut - isso aqui basicamente é a ponte de ligação para o OO
    static void displayCallback();
    static void reshapeCallback(int w, int h);
    static void mouseClickCallback(int button, int state, int x, int y);
    static void mousePassiveMotionCallback(int x, int y);
    static void keyboardClickCallback(unsigned char key, int x, int y);
    static void keyboardUpCallback(unsigned char key, int x, int y);
    static void idleCallback();
};

#endif