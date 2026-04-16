#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

class InputManager {
public:
    bool isZoomed;
    float mouseWorldX;
    float mouseWorldY;

    bool isWPressed;
    bool isAPressed;
    bool isSPressed;
    bool isDPressed;
    bool isJPressed;
    bool isKPressed;
    bool isLPressed;
    bool wasLPressed;
    bool wasJPressed;
    bool wasKPressed;

    InputManager();
    
    // convert a posição do mouse (em pixels) para o sistema de coordenadas do opengiló
    void updateMouseWorldCoords(int x, int y, int winW, int winH);
};

#endif