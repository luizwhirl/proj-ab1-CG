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

    void drawText(int x, int y, const std::string& text);
    void drawSmallText(int x, int y, const std::string& text);
    
    void beginHUD(int winW, int winH);
    void endHUD();

public:
    Scoreboard();
    
    void draw(int winW, int winH);
    void scoreAliado();
    
    // pontua rival (depouis eu tiroo esse nome maldito da alemanha)
    void scoreRival(); 
};

#endif