#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <string>

class Scoreboard {
private:
    // NESTA VILA ESTÃO
    // PROIBIDOS ANIMAIS E
    // CRIANÇAS PEQUENAS
    int scoreLeft;
    int scoreRight;

    // btn -> button
    const int btnW = 50;
    const int btnH = 50;
    const int margin = 20;

    void drawText(int x, int y, const std::string& text);
    void drawSmallText(int x, int y, const std::string& text);
    
    void beginHUD(int winW, int winH);
    void endHUD();

public:
    Scoreboard();
    
    void draw(int winW, int winH);
    void drawTestButtons(int winW, int winH);

    bool checkButtonLeftClick(int x, int y, int winW, int winH);
    bool checkButtonRightClick(int x, int y, int winW, int winH);
};

#endif