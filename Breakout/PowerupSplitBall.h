#pragma once
#include "PowerupBase.h"
class PowerupSplitBall : public PowerupBase
{
public:
    PowerupSplitBall(sf::RenderWindow* window, Paddle* paddle, std::vector<Ball*>& balls, float x_pos);
    ~PowerupSplitBall();

    std::pair<POWERUPS, float> applyEffect() override;
};

