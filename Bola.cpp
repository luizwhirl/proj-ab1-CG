#include "Bola.h"
#include <GL/glut.h>
#include <utility>

Bola::Bola() {
    x = 0.0f;
    y = 0.0f;
}

void Bola::draw() {
    glPushMatrix(); 
    
    glTranslatef(x, y, 0.0f); 

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
        glVertex2f(-0.1f, -0.1f);
        glVertex2f( 0.1f, -0.1f);
        glVertex2f( 0.1f,  0.1f);
        glVertex2f(-0.1f,  0.1f); 
    glEnd();

    glPopMatrix();
}

// mover a bola por ai
void Bola::update(char direction) {
    switch (direction) {
    case 'R':
        x+= 0.1f;
        break;
    case 'L':
        x-= 0.1f;
        break;
    case 'U':
        y+= 0.1f;
        break;
    case 'D':
        y-= 0.1f;
        break;
    default:
        break;
    }
}