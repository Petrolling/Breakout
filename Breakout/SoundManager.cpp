#include "SoundManager.h"
#include <iostream>

SoundManager::SoundManager()
{
    if (!_hitBuffer.loadFromFile("Audio/pop.wav"))
        std::cout << "Failed to load hit sound\n";

    _hitSound.setBuffer(_hitBuffer);
}

SoundManager::~SoundManager()
{
}

void SoundManager::playHit()
{
    _hitSound.play();
}
