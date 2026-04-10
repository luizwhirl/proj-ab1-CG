#include <GL/glut.h>
#include "Game.h"

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE);
    glutInitWindowSize(600, 800);
    glutCreateWindow("Campo c Zum POO");

    // instancia o contexto/manager
    Game* game = Game::getInstance();
    game->init();

    // atribuindo callbacks usando os métodos estáticos da classe Game
    glutDisplayFunc(Game::displayCallback);
    glutReshapeFunc(Game::reshapeCallback);
    glutMouseFunc(Game::mouseClickCallback);
    glutPassiveMotionFunc(Game::mousePassiveMotionCallback);
    glutKeyboardFunc(Game::keyboardClickCallback);
    glutKeyboardUpFunc(Game::keyboardUpCallback);
    glutIdleFunc(Game::idleCallback);

    glutMainLoop();
    return 0;
}