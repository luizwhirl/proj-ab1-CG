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

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(-0.2f, -0.2f);
        glVertex2f( 0.2f, -0.2f);
        glVertex2f( 0.2f,  0.2f);
        glVertex2f(-0.2f,  0.2f); 
    glEnd();

    glPopMatrix();
}