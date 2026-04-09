#ifndef GAME_H
#define GAME_H

#include "Campo.h"
#include "Scoreboard.h"
#include "InputManager.h"
#include "Bola.h"

// Scene / Game State e Window / Context Manager
class Game {
private:
    int winW, winH;
    
    Campo campo;
    Scoreboard scoreboard;
    InputManager input;
    Bola bola;

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
    void updateBall();

    static Game* getInstance();

    // callbacks estáticos pro glut - isso aqui basicamente é a ponte de ligação para o OO
    static void displayCallback();
    static void reshapeCallback(int w, int h);
    static void mouseClickCallback(int button, int state, int x, int y);
    static void mousePassiveMotionCallback(int x, int y);
    static void keyboardClickCallback(unsigned char key, int x, int y);
};

#endif