#include "PowerupFireBall.h"


PowerupFireBall::PowerupFireBall(sf::RenderWindow* window, Paddle* paddle, std::vector<Ball*>& balls, float x_pos)
    : PowerupBase(window, paddle, balls, x_pos)
{
    _sprite.setFillColor(ballEffectsColour);
}

PowerupFireBall::~PowerupFireBall()
{

    int* pInteger = new int();

    if (pInteger != nullptr)
    {
        // use my pointer to do things.
    }


    
}

std::pair<POWERUPS, float> PowerupFireBall::applyEffect()
{
    for (Ball* ball : _balls)
        ball->setFireBall(5.0f);
    return { fireBall, 5.0f };
}