#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <SFML/Audio.hpp>
#include <string>
#include <map>

class SoundManager {
private:
    std::map<std::string, sf::SoundBuffer> buffers;
    std::map<std::string, sf::Sound> sounds;
    
    // Rende la classe un Singleton
    SoundManager();
    ~SoundManager();

public:
    // Elimina costruttore di copia e assegnamento
    SoundManager(const SoundManager&) = delete;
    void operator=(const SoundManager&) = delete;

    static SoundManager& getInstance();

    void loadSound(const std::string& name, const std::string& filepath);
    void playSound(const std::string& name);
    void setVolume(float volume);
};

#endif
