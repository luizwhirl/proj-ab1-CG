#include <GL/glut.h>
#include "Game.h"

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE);
    glutInitWindowSize(600, 800);
    glutCreateWindow("Campo c Zum POO");

    // Instancia o nosso contexto/manager
    Game* game = Game::getInstance();
    game->init();

    // Atribuindo os callbacks usando nossos métodos estáticos da classe Game
    glutDisplayFunc(Game::displayCallback);
    glutReshapeFunc(Game::reshapeCallback);
    glutMouseFunc(Game::mouseClickCallback);
    glutPassiveMotionFunc(Game::mousePassiveMotionCallback);

    glutMainLoop();
    return 0;
}