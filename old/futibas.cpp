#include <GL/glut.h>
#include <cmath>
#include <iostream> 
#include <cstdlib> // rand() e srand()
#include <ctime>   // time()
#include "scoreboard.h"

constexpr float PI = 3.14159265358979323846f;

int winW = 600, winH = 800;
bool isZoomed = false;
float mouseWorldX = 0.0f, mouseWorldY = 0.0f;

// variavel do id da grama random
GLuint grassTexture;

// gerar nossa graminha randomizada
void createGrassTexture() {
    const int TEX_SIZE = 128; // 128x128 pixels 
    unsigned char data[TEX_SIZE * TEX_SIZE * 3];

    // paleta de cores no carai
    unsigned char palette[4][3] = {
        { 90, 145,  65},  // verde médio base
        {105, 160,  70},  // verde mais claro
        { 75, 120,  50},  // verde escuro
        {110, 130,  55}   // verde amarelado
    };

    for (int i = 0; i < TEX_SIZE * TEX_SIZE; ++i) {
        int colorIndex = rand() % 4;
        
        data[i * 3]     = palette[colorIndex][0];
        data[i * 3 + 1] = palette[colorIndex][1];
        data[i * 3 + 2] = palette[colorIndex][2];
    }

    glGenTextures(1, &grassTexture);
    glBindTexture(GL_TEXTURE_2D, grassTexture);
    
    // enviando dados da textura para o opengll
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TEX_SIZE, TEX_SIZE, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    //  - nao aplica blur ao escalajar
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // rete a a rexteura ao longo do campo
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void drawCircle(float cx, float cy, float r, int num_segments) {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < num_segments; i++) {
        float theta = 2.0f * PI * static_cast<float>(i) / static_cast<float>(num_segments);
        float x = r * std::cos(theta);
        float y = r * std::sin(theta);
        glVertex2f(cx + x, cy + y);
    }
    glEnd();
}

// desenhar as linhas do campo na vertical
void drawFieldLines() {
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2.0f);

    glBegin(GL_LINE_LOOP);
        glVertex2f(-3.0f, -5.0f);
        glVertex2f( 3.0f, -5.0f);
        glVertex2f( 3.0f,  5.0f);
        glVertex2f(-3.0f,  5.0f);
    glEnd();

    // meio do campo
    glBegin(GL_LINES);
        glVertex2f(-3.0f, 0.0f);
        glVertex2f( 3.0f, 0.0f);
    glEnd();

    // meio do meio do campo - o círculo
    drawCircle(0.0f, 0.0f, 1.0f, 36);

    // lado dibaxo - grande area
    glBegin(GL_LINE_STRIP);
        glVertex2f(-1.5f, -5.0f);
        glVertex2f(-1.5f, -3.5f);
        glVertex2f( 1.5f, -3.5f);
        glVertex2f( 1.5f, -5.0f);
    glEnd();

    // lado dibaxo 2 - pqeuna area
    glBegin(GL_LINE_STRIP);
        glVertex2f(-0.6f, -5.0f);
        glVertex2f(-0.6f, -4.2f);
        glVertex2f( 0.6f, -4.2f);
        glVertex2f( 0.6f, -5.0f);
    glEnd();

    // lado di cimah - grande area de novo wtf 
    glBegin(GL_LINE_STRIP);
        glVertex2f(-1.5f,  5.0f);
        glVertex2f(-1.5f,  3.5f);
        glVertex2f( 1.5f,  3.5f);
        glVertex2f( 1.5f,  5.0f);
    glEnd();

    // lado di xima 2 - pquenea area 
    glBegin(GL_LINE_STRIP);
        glVertex2f(-0.6f,  5.0f);
        glVertex2f(-0.6f,  4.2f);
        glVertex2f( 0.6f,  4.2f);
        glVertex2f( 0.6f,  5.0f);
    glEnd();
}

void setupCamera() {
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

    if (isZoomed) {
        float zoomFactor = 0.35f;
        float viewWidth = (right - left) * zoomFactor;
        float viewHeight = (top - bottom) * zoomFactor;

        left = mouseWorldX - viewWidth / 2.0f;
        right = mouseWorldX + viewWidth / 2.0f;
        bottom = mouseWorldY - viewHeight / 2.0f;
        top = mouseWorldY + viewHeight / 2.0f;
    }

    glOrtho(left, right, bottom, top, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    setupCamera();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, grassTexture);
    
    // esse repeats via definr quantas vezes a etxtura da grama vai se repetir
    float repeats = 15.0f; 

    // lógica de faixas
    int numFaixas = 11; // 11 faixas distribuidas ao longo (nisso bem distribuído de -5.5 a 5.5)
    float campoInicioY = -5.5f;
    float campoFimY = 5.5f;
    float alturaFaixa = (campoFimY - campoInicioY) / numFaixas;

    glBegin(GL_QUADS);
    for (int i = 0; i < numFaixas; ++i) {
        float yBase = campoInicioY + i * alturaFaixa;
        float yTopo = yBase + alturaFaixa;

        // altaernancia de cores de casa lado
        if (i % 2 == 0) {
            glColor3f(1.0f, 1.0f, 1.0f); // tim base da textura - claro
        } else {
            glColor3f(0.85f, 0.90f, 0.85f); // multiplika a textura por um tom mais escuro
        }

        // calkuloda coordenada UV da textura para que ela não quebre na divisa das faixas
        float vBase = (yBase - campoInicioY) / (campoFimY - campoInicioY) * repeats;
        float vTopo = (yTopo - campoInicioY) / (campoFimY - campoInicioY) * repeats;

        // faixa horizontal
        glTexCoord2f(0.0f, vBase);       glVertex2f(-3.5f, yBase);
        glTexCoord2f(repeats, vBase);    glVertex2f( 3.5f, yBase);
        glTexCoord2f(repeats, vTopo);    glVertex2f( 3.5f, yTopo);
        glTexCoord2f(0.0f, vTopo);       glVertex2f(-3.5f, yTopo);
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);

    drawFieldLines();

    drawScoreboard(winW, winH);

    // butaos de teste
    drawTestButtons(winW, winH);

    glutSwapBuffers();
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    winW = w;
    winH = h;
    glViewport(0, 0, w, h);
}

// convert a posição do mouse (em pixels) para o sistema de coordenadas do opengiló
void updateMouseWorldCoords(int x, int y) {
    float aspect = static_cast<float>(winW) / static_cast<float>(winH);
    float size = 6.0f;
    float left, right, bottom, top;

    // rekalkula o tamanho total para mapear corretamente (isso sem o zoom)
    if (winW <= winH) {
        left = -size; right = size;
        bottom = -size / aspect; top = size / aspect;
    } else {
        left = -size * aspect; right = size * aspect;
        bottom = -size; top = size;
    }
    
    // a janela vai de (0,0) no topo esquerdo ate winW, winG no fundo direitio
    // dai precisamo  converter isso para as coordenadas x e y do campo
    mouseWorldX = left + (static_cast<float>(x) / winW) * (right - left);
    mouseWorldY = top - (static_cast<float>(y) / winH) * (top - bottom); 
    // inverte Y porque o openg cresce para cima
}

void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        
        // butaos de teste
        if (checkButtonLeftClick(x, y, winW, winH) || checkButtonRightClick(x, y, winW, winH)) {
            glutPostRedisplay();
            return;
        }

        isZoomed = !isZoomed;
        
        if (isZoomed) {
            updateMouseWorldCoords(x, y);
        }
        glutPostRedisplay();
    }
}

// passivo igual lucas
void mousePassiveMotion(int x, int y) {
    if (isZoomed) {
        updateMouseWorldCoords(x, y);
        glutPostRedisplay(); // atualiza continuamente a tela ne pq zoom i tals
    }
}

void init() {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // inicializa a seed do rand()
    srand(static_cast<unsigned int>(time(NULL)));
    createGrassTexture();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE);
    glutInitWindowSize(600, 800);
    glutCreateWindow("Campo c Zum");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    
    glutMouseFunc(mouseClick);
    glutPassiveMotionFunc(mousePassiveMotion);

    glutMainLoop();
    return 0;
}