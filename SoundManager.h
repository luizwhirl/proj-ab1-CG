#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <string>

// arquivotodinho pra gerenciar os sons do jogo 
class SoundManager {
private:
    static SoundManager* instance;
    SoundManager(); 
    // construtor privado (é singleton ne papis)

public:
    static SoundManager* getInstance();

    // inicializa o irrKnojang
    void init(); 
    
    // func~oes dos eventos
    void playChute();
    void playGol();
    void playApito();

    // func generica -> toca o arquivo
    void playSound(const std::string& filepath);
};

#endif