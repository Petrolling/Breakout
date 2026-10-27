#pragma once
#include <SFML/Graphics.hpp>
#include <cstdlib>

class VFX
{
private:
    float shakeDuration = 0.0f;
    float shakeIntensity = 0.0f;

    sf::Vector2f originalPosition;
    sf::View* _view;

public:
    VFX(sf::View* view);
    ~VFX();

    void Start(float duration, float intensity)
    {
        if (shakeDuration <= 0.0f)
        {
            originalPosition = _view->getCenter();
        }

        shakeDuration = duration;
        shakeIntensity = intensity;
    }

    void Update(float deltaTime)
    {
        if (shakeDuration <= 0.0f)
        {
            return;
        }

        shakeDuration -= deltaTime;

        if (shakeDuration <= 0.0f)
        {
            _view->setCenter(originalPosition);
            return;
        }

        float offsetX = (rand() % 200 - 100) / 100.0f;
        float offsetY = (rand() % 200 - 100) / 100.0f;

        _view->setCenter(originalPosition.x + offsetX * shakeIntensity, originalPosition.y + offsetY * shakeIntensity);
    }
};