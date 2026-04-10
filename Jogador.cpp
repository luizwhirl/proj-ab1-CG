#include "Jogador.h"
#include <GL/glut.h>
#include <utility>

Jogador::Jogador() {
    x = 1.0f;
    y = 1.0f;
}

void Jogador::draw() {
    glPushMatrix(); 
    
    glTranslatef(x, y, 0.0f); 

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
        glVertex2f(-0.1f, -0.1f);
        glVertex2f( 0.1f, -0.1f);
        glVertex2f( 0.1f,  0.1f);
        glVertex2f(-0.1f,  0.1f); 
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(-0.2f, -0.2f);
        glVertex2f( 0.2f, -0.2f);
        glVertex2f( 0.2f,  0.2f);
        glVertex2f(-0.2f,  0.2f); 
    glEnd();


    glPopMatrix();
}

// Jogador sai andando por ai
void Jogador::update(char direction) {
    switch (direction) {
    case 'D':
        x+= 0.015f;
        break;
    case 'A':
        x-= 0.015f;
        break;
    case 'W':
        y+= 0.015f;
        break;
    case 'S':
        y-= 0.015f;
        break;
    default:
        break;
    }
}