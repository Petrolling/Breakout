#include "Ball.h"
#include "GameManager.h" // avoid cicular dependencies
#include <iostream>

Ball::Ball(sf::RenderWindow* window, float velocity, GameManager* gameManager)
    : _window(window), _velocity(velocity), _gameManager(gameManager),
    _timeWithPowerupEffect(0.f), _isFireBall(false), _isAlive(true), _direction({1,1})
{
    _sprite.setRadius(RADIUS);
    _sprite.setOrigin(RADIUS, RADIUS);
    _sprite.setFillColor(sf::Color::Cyan);
    _sprite.setPosition(0, 300 + RADIUS);
}

Ball::~Ball()
{
}

void Ball::update(float dt)
{
    // check for powerup, tick down or correct
    if (_timeWithPowerupEffect > 0.f)
    {
        _timeWithPowerupEffect -= dt;
    }
    else
    {
        if (_velocity != VELOCITY)
            _velocity = VELOCITY;   // reset speed.
        else
        {
            setFireBall(0);    // disable fireball
            _sprite.setFillColor(sf::Color::Cyan);  // back to normal colour.
        }        
    }

    // Fireball effect
    if (_isFireBall)
    {
        // Flickering effect
        int flicker = rand() % 50 + 205; // Random value between 205 and 255
        _sprite.setFillColor(sf::Color(flicker, flicker / 2, 0)); // Orange flickering color
    }

    // Update position with a subtle floating-point error
    _sprite.move(_direction * _velocity * dt);

    // check bounds and bounce
    sf::Vector2f position = _sprite.getPosition();
    sf::Vector2u windowDimensions = _window->getSize();

    // bounce on walls
    if ((position.x >= windowDimensions.x - RADIUS && _direction.x > 0) || (position.x <= RADIUS && _direction.x < 0))
    {
        _direction.x *= -1;
    }

    // bounce on ceiling
    if (position.y <= RADIUS && _direction.y < 0)
    {
        _direction.y *= -1;
    }

    // ball dead GameManager will handle ti
    if (position.y > windowDimensions.y)
    {
        _isAlive = false;
    }

    // collision with paddle
    if (_sprite.getGlobalBounds().intersects(_gameManager->getPaddle()->getBounds()))
    {
        _direction.y *= -1; // Bounce vertically

        float paddlePositionProportion = (_sprite.getPosition().x - _gameManager->getPaddle()->getBounds().left) / _gameManager->getPaddle()->getBounds().width;
        _direction.x = paddlePositionProportion * 2.0f - 1.0f;

        // Adjust position to avoid getting stuck inside the paddle
        _sprite.setPosition(_sprite.getPosition().x, _gameManager->getPaddle()->getBounds().top - RADIUS);
    }

    // collision with bricks
    int collisionResponse = _gameManager->getBrickManager()->checkCollision(_sprite, _direction);

    //std::cout << _trail.size() << std::endl;
    _trail.push_back({ _sprite.getPosition(), _maxTrailLifetime });
    for (auto it = _trail.begin(); it != _trail.end();)
    {
        it->lifetime -= dt;
        if (it->lifetime <= 0.f) {
            it = _trail.erase(it);
        }
        else {
            ++it;
        }
    }

    if (_isFireBall) return; // no collisisons when in fireBall mode.
    if (collisionResponse == 1)
    {
        _direction.x *= -1; // Bounce horizontally
    }
    else if (collisionResponse == 2)
    {
        _direction.y *= -1; // Bounce vertically
    }
}

void Ball::render()
{
    if (_trail.size() > 1)
    {
        sf::VertexArray trailMesh(sf::PrimitiveType::TriangleStrip);
        float trailWidth = RADIUS * 1.5f;

        for (size_t i = 0; i < _trail.size(); ++i)
        {
            sf::Vector2f direction(0.f, 0.f);
            if (i < _trail.size() - 1) {
                direction = _trail[i + 1].position - _trail[i].position;
            }
            else if (i > 0) {
                direction = _trail[i].position - _trail[i - 1].position;
            }

            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            sf::Vector2f normal(0.f, 0.f);
            if (length != 0.f) {
                normal = sf::Vector2f(-direction.y / length, direction.x / length);
            }

            float progress = _trail[i].lifetime / _maxTrailLifetime;
            float currHalfWidth = (trailWidth * progress) / 2.0f;
            uint8_t alpha = (uint8_t)(255 * progress);

            sf::Color trailColor;
            if (_isFireBall) {
                trailColor = sf::Color(255, 120, 0, alpha);
            }
            else {
                trailColor = sf::Color(0, 255, 255, alpha);
            }

            //shrink on lifetime
            sf::Vector2f leftPoint = _trail[i].position + normal * currHalfWidth;
            sf::Vector2f rightPoint = _trail[i].position - normal * currHalfWidth;

            trailMesh.append(sf::Vertex(leftPoint, trailColor));
            trailMesh.append(sf::Vertex(rightPoint, trailColor));
        }

        _window->draw(trailMesh);
    }

    _window->draw(_sprite);
}

void Ball::setVelocity(float coeff, float duration)
{
    _velocity = coeff * VELOCITY;
    _timeWithPowerupEffect = duration;
}

void Ball::setFireBall(float duration)
{
    if (duration) 
    {
        _isFireBall = true;
        _timeWithPowerupEffect = duration;        
        return;
    }
    _isFireBall = false;
    _timeWithPowerupEffect = 0.f;    
}
