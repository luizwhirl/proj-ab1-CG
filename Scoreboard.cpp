#include "Scoreboard.h"
#include <GL/glut.h>

Scoreboard::Scoreboard() {
    scoreLeft = 1;
    scoreRight = 7;
}

// funcao aux pra renderizar o texto
void Scoreboard::drawText(int x, int y, const std::string& text) {
    glRasterPos2i(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

// rendreriza texto menor
void Scoreboard::drawSmallText(int x, int y, const std::string& text) {
    glRasterPos2i(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }
}

// HUD do placar no bagui se liga mermao
void Scoreboard::beginHUD(int winW, int winH) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, winW, winH, 0, -1.0, 1.0); // (0,0) -> posicao no canto superior esquerdo
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
}

void Scoreboard::endHUD() {
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void Scoreboard::draw(int winW, int winH) {
    beginHUD(winW, winH);

    int startX = 20;
    int startY = 20;
    int h = 30;

    // brasil fundo
    glColor3f(0.9f, 0.9f, 0.9f);
    glBegin(GL_QUADS);
        glVertex2i(startX, startY);
        glVertex2i(startX + 85, startY);
        glVertex2i(startX + 85, startY + h);
        glVertex2i(startX, startY + h);
    glEnd();

    // """"""""""""""bandeira""""""""""""")
    glColor3f(0.0f, 0.6f, 0.0f);
    glBegin(GL_QUADS); glVertex2i(startX + 5, startY + 6); glVertex2i(startX + 28, startY + 6); glVertex2i(startX + 28, startY + 24); glVertex2i(startX + 5, startY + 24); glEnd();
    glColor3f(1.0f, 0.9f, 0.0f); 
    glBegin(GL_QUADS); glVertex2i(startX + 10, startY + 10); glVertex2i(startX + 23, startY + 10); glVertex2i(startX + 23, startY + 20); glVertex2i(startX + 10, startY + 20); glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    drawText(startX + 35, startY + 21, "BRA");

    // seção central do placa r - azul
    glColor3f(0.2f, 0.5f, 0.8f);
    glBegin(GL_QUADS);
        glVertex2i(startX + 85, startY);
        glVertex2i(startX + 145, startY);
        glVertex2i(startX + 145, startY + h);
        glVertex2i(startX + 85, startY + h);
    glEnd();

    // numeros do pracaar
    glColor3f(1.0f, 1.0f, 1.0f);
    std::string scoreStr = std::to_string(scoreLeft) + " - " + std::to_string(scoreRight);
    int textWidth = scoreStr.length() * 9; 
    drawText(startX + 115 - textWidth / 2, startY + 21, scoreStr);

    // alemanha
    glColor3f(0.9f, 0.9f, 0.9f);
    glBegin(GL_QUADS);
        glVertex2i(startX + 145, startY);
        glVertex2i(startX + 230, startY);
        glVertex2i(startX + 230, startY + h);
        glVertex2i(startX + 145, startY + h);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    drawText(startX + 155, startY + 21, "ALE");

    // """"""""""""bandeira""""""""""""
    int fX = startX + 198;
    int fY = startY + 6;
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS); glVertex2i(fX, fY); glVertex2i(fX + 23, fY); glVertex2i(fX + 23, fY + 6); glVertex2i(fX, fY + 6); glEnd();
    glColor3f(0.8f, 0.0f, 0.0f);
    glBegin(GL_QUADS); glVertex2i(fX, fY + 6); glVertex2i(fX + 23, fY + 6); glVertex2i(fX + 23, fY + 12); glVertex2i(fX, fY + 12); glEnd();
    glColor3f(1.0f, 0.8f, 0.0f);
    glBegin(GL_QUADS); glVertex2i(fX, fY + 12); glVertex2i(fX + 23, fY + 12); glVertex2i(fX + 23, fY + 18); glVertex2i(fX, fY + 18); glEnd();

    endHUD();
}

void Scoreboard::scoreBrazil() {
    scoreLeft++;
}

// vai se ferra aemanah
void Scoreboard::scoreGermany() {
    scoreRight++;
}