#include "SoundManager.h"
#include <cstdlib> // rand()
#include <iostream> // std::cout

#include <irrKlang.h> 

// pra n escrever irrklang:: toda hora
using namespace irrklang;

// o "motor" de som global -  so que sopra este arquivo
ISoundEngine* engine = nullptr;

// inicando o singleton
SoundManager* SoundManager::instance = nullptr;

SoundManager::SoundManager() {}

SoundManager* SoundManager::getInstance() {
    if (!instance) {
        instance = new SoundManager();
    }
    return instance;
}

void SoundManager::init() {
    // ligando a engine do irrklang
    engine = createIrrKlangDevice();
    
    if (!engine) {
        std::cout << "[Erro] Nao foi possivel iniciar o irrklang\n";
    } else {
        std::cout << "[SoundManager] irrklang iniciado com sucesso\n";
    }
}

void SoundManager::playSound(const std::string& filepath) {
    // se -> engine foi criada, toca o arquivo
    if (engine) {
        // false é pro som não vai ficar em loop eterno
        engine->play2D(filepath.c_str(), false);
    }
}

void SoundManager::playChute() {
    // randomicamente escolhe 0 - 1 para definir qual som de chute tocar
    int escolha = rand() % 2; 
    if (escolha == 0) {
        playSound("assets/sounds/chute1.mp3");
    } else {
        playSound("assets/sounds/chute2.mp3");
    }
}

void SoundManager::playGol() {
    playSound("assets/sounds/comemoracao1.mp3");
}

void SoundManager::playApito() {
    playSound("assets/sounds/arbitro-apito.mp3");
}