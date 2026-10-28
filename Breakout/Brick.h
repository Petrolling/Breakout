#pragma once
#include <SFML/Graphics.hpp>

class Brick {
public:
    Brick(float x, float y, float width, float height);
    void render(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;

    void setDestroyed(bool _b) { _isDestroyed = _b; }
    bool getDestroyed() { return _isDestroyed; }
private:
    sf::RectangleShape _shape;
    bool _isDestroyed;
};