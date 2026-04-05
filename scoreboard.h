#ifndef SCOREBOARD_H
#define SCOREBOARD_H

// draw interface 
void drawScoreboard(int winW, int winH);
void drawTestButtons(int winW, int winH);

// check o clicque dos botoes 
bool checkButtonLeftClick(int x, int y, int winW, int winH);
bool checkButtonRightClick(int x, int y, int winW, int winH);

#endif  