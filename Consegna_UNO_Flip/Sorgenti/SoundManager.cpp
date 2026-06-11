#include "SoundManager.h"
#include <iostream>

SoundManager::SoundManager() {
    // Caricamento dei suoni predefiniti
    loadSound("pesca", "pesca.wav");
    loadSound("gioca", "gioca.wav");
    loadSound("click", "click.wav");
    loadSound("uno", "uno.wav");
    loadSound("errore", "errore.wav");
    loadSound("vittoria", "vittoria.wav");
}

SoundManager::~SoundManager() {}

SoundManager& SoundManager::getInstance() {
    static SoundManager instance;
    return instance;
}

void SoundManager::loadSound(const std::string& name, const std::string& filepath) {
    if (!buffers[name].loadFromFile(filepath)) {
        std::cerr << "Attenzione: Impossibile caricare il file audio " << filepath << std::endl;
    } else {
        sounds.insert({name, sf::Sound(buffers[name])});
    }
}

void SoundManager::playSound(const std::string& name) {
    auto it = sounds.find(name);
    if (it != sounds.end()) {
        it->second.play();
    }
}

void SoundManager::setVolume(float volume) {
    for (auto& pair : sounds) {
        pair.second.setVolume(volume);
    }
}
