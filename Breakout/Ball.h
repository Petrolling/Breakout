#pragma once
#include <SFML/Graphics.hpp>

struct TrailPoint {
    sf::Vector2f position;
    float lifetime;
};

class GameManager;  // forward declaration



class Ball {
public:
    Ball(sf::RenderWindow* window, float velocity, GameManager* gameManager);
    ~Ball();
    void update(float dt);
    void render();
    void setVelocity(float coeff, float duration);
    void setDirection(const sf::Vector2f& direction) { _direction = direction; }
    void setFireBall(float duration);
	void setPosition(float x, float y) { _sprite.setPosition(x, y); }

	sf::Vector2f getDirection() const { return _direction; }
	float getVelocity() const { return _velocity; }
    GameManager* getGameManager() const { return _gameManager; }
	sf::Vector2f getPosition() const { return _sprite.getPosition(); }
	bool isAlive() const { return _isAlive; }
private:
    sf::CircleShape _sprite;
    sf::Vector2f _direction;
    sf::RenderWindow* _window;
    float _velocity;
    bool _isAlive;
    bool _isFireBall;
    float _timeWithPowerupEffect;

    GameManager* _gameManager;  // Reference to the GameManager

    std::vector<TrailPoint> _trail;
    const float _maxTrailLifetime = 0.3f;

    static constexpr float RADIUS = 10.0f;      
    static constexpr float VELOCITY = 350.0f;   // for reference.
};

