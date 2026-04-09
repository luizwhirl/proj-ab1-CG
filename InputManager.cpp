#include "InputManager.h"

InputManager::InputManager() {
    isZoomed = false;
    mouseWorldX = 0.0f;
    mouseWorldY = 0.0f;

    isWPressed = false;
    isAPressed = false;
    isSPressed = false;
    isDPressed = false;
}

void InputManager::updateMouseWorldCoords(int x, int y, int winW, int winH) {
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