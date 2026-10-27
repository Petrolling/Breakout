#include "PowerupSplitBall.h"
#include <iostream>


PowerupSplitBall::PowerupSplitBall(sf::RenderWindow* window, Paddle* paddle, std::vector<Ball*>& balls, float x_pos)
    : PowerupBase(window, paddle, balls, x_pos)
{
    _sprite.setFillColor(ballEffectsColour); // Same colour as SmallPaddle
}

PowerupSplitBall::~PowerupSplitBall()
{
}

std::pair<POWERUPS, float> PowerupSplitBall::applyEffect()
{

    size_t ballCount = _balls.size();

    for (size_t i = 0; i < ballCount; ++i)
    {
        Ball* currentBall = _balls[i];

        for (int j = 0; j < 2; ++j)
        {
            Ball* newBall = new Ball(
                _window,
                currentBall->getVelocity(),
                currentBall->getGameManager()
            );

            newBall->setPosition(
                currentBall->getPosition().x,
                currentBall->getPosition().y
            );

            sf::Vector2f dir = currentBall->getDirection();

            if (j == 0)
                dir.x -= 0.5f;
            else
                dir.x += 0.5f;

            float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);

            if (length > 0.f)
                dir /= length;

            newBall->setDirection(dir);

            _balls.push_back(newBall);
        }
    }

    return { splitBall, 0.f };
}
