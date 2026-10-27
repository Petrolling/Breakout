#pragma once
#include <SFML/Audio.hpp>

class SoundManager
{
private:
    sf::SoundBuffer _hitBuffer;

    sf::Sound _hitSound;

public:
    SoundManager();
    ~SoundManager();

    void playHit();
};
