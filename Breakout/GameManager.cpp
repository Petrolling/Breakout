#include "GameManager.h"
#include "Ball.h"
#include "PowerupManager.h"
#include <iostream>

GameManager::GameManager(sf::RenderWindow* window)
    : _window(window), _paddle(nullptr), _brickManager(nullptr), _powerupManager(nullptr),
    _messagingSystem(nullptr), _ui(nullptr), _vfx(nullptr), _soundManager(nullptr), _pause(false), _time(0.f), _lives(3), _pauseHold(0.f), _levelComplete(false),
    _powerupInEffect({ none,0.f }), _timeLastPowerupSpawned(0.f)
{
    _font.loadFromFile("font/montS.ttf");
    _masterText.setFont(_font);
    _masterText.setPosition(50, 400);
    _masterText.setCharacterSize(48);
    _masterText.setFillColor(sf::Color::Yellow);
}

void GameManager::initialize()
{
    _paddle = new Paddle(_window);
    _brickManager = new BrickManager(_window, this);
    _messagingSystem = new MessagingSystem(_window);
    Ball* ball = new Ball(_window, 400.0f, this);
	_balls.push_back(ball);
    _powerupManager = new PowerupManager(_window, _paddle, _balls);
    _ui = new UI(_window, _lives, this);
    _soundManager = new SoundManager();


    // Create bricks
    _brickManager->createBricks(5, 10, 80.0f, 30.0f, 5.0f);

    _camera = _window->getDefaultView();
    _window->setView(_camera);
    _vfx = new VFX(&_camera);



}

void GameManager::reset()
{
    //clean up
    for (Ball* ball : _balls) {
        delete ball;
    }
    _balls.clear();

    delete _paddle;
    delete _brickManager;
    delete _powerupManager;
    delete _ui;

    //reset vars
    _lives = 3;
    _time = 0.f;
    _timeLastPowerupSpawned = 0.f;
    _levelComplete = false;
    _pause = false;
    _masterText.setString("");
    _masterText.setPosition(50, 400);

    initialize();
}

void GameManager::update(float dt)
{
    _powerupInEffect = _powerupManager->getPowerupInEffect();
    _ui->updatePowerupText(_powerupInEffect);
    _powerupInEffect.second -= dt;
    
    if (_lives <= 0)
    {
        _masterText.setString("Game over. Press 'R' to restart.");
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
        {
            reset();
        }
        return;
    }

    if (_levelComplete)
    {
        _masterText.setString("Level completed. Press 'R' to restart.");
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
        {
            reset();
        }
        return;
    }

    // pause and pause handling
    if (_pauseHold > 0.f) _pauseHold -= dt;
    if (_settingsHold > 0.f) _settingsHold -= dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
    {
        if (!_pause && _pauseHold <= 0.f)
        {
            _masterText.setPosition(50, 50);
            _pause = true;
            _masterText.setString("paused.");
            _pauseHold = PAUSE_TIME_BUFFER;
        }
        if (_pause && _pauseHold <= 0.f)
        {
            _masterText.setPosition(50, 400);
            _pause = false;
            _masterText.setString("");
            _pauseHold = PAUSE_TIME_BUFFER;
        }
    }
    if (_pause)
    {
        if (_settingsHold <= 0.f)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                _selectedSetting = (_selectedSetting == 0) ? 1 : 0;
                _settingsHold = 0.3f;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                _selectedSetting = (_selectedSetting == 0) ? 1 : 0;
                _settingsHold = 0.3f;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                if (_selectedSetting == 0)
                    _powerupFrequency -= 1.0f;
                else
                    _powerupSpawnChance -= 50;

                _settingsHold = 0.3f;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                if (_selectedSetting == 0)
                    _powerupFrequency += 1.0f;
                else
                    _powerupSpawnChance += 50;

                _settingsHold = 0.3f;
            }
        }

        //cap
        if (_powerupFrequency < 1.0f)
            _powerupFrequency = 1.0f;

        if (_powerupSpawnChance < 50)
            _powerupSpawnChance = 50;

        // display pause menu
        std::string menuText = "PAUSED\n\n";

        menuText += (_selectedSetting == 0 ? "> " : "  ");
        menuText += "Powerup Frequency: " + std::to_string(static_cast<int>(_powerupFrequency)) + " seconds\n";

        menuText += (_selectedSetting == 1 ? "> " : "  ");
        menuText += "Spawn Chance: 1 in " + std::to_string(_powerupSpawnChance) + "\n\n";

        menuText += "UP/DOWN: Select setting\n";
        menuText += "LEFT/RIGHT: Change value\n";
        menuText += "P: Unpause";

        _masterText.setString(menuText);

        return;
    }

    // timer.
    _time += dt;


    if (_time > _timeLastPowerupSpawned + _powerupFrequency && rand() % _powerupSpawnChance == 0)
    {
        _powerupManager->spawnPowerup();
        _timeLastPowerupSpawned = _time;
    }

    // move paddle
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) _paddle->moveRight(dt);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) _paddle->moveLeft(dt);

    sf::Vector2i position = sf::Mouse::getPosition(*_window);
	_paddle->setPosition(static_cast<float>(position.x) - _paddle->getBounds().width / 2.0f);

    _vfx->Update(dt);

    // update everything 
    _paddle->update(dt);
    for (Ball* _b : _balls)
    {
        _b->update(dt);
    }
    //_ball->update(dt);

    //remove dead balls, respawn if none left
    for (auto it = _balls.begin(); it != _balls.end(); )
    {
        if (!(*it)->isAlive())
        {
            delete *it;
            it = _balls.erase(it);
        }
        else
        {
            ++it;
        }
    }
    if (_balls.empty())
    {
        loseLife();
        if (_lives > 0)
        {
            Ball* newBall = new Ball(_window, 400.0f, this);
            _balls.push_back(newBall);
        }
    }

    _powerupManager->update(dt);
}

void GameManager::loseLife()
{
    _lives--;
    _ui->lifeLost(_lives);

	_vfx->Start(0.5f, 5.0f);
}

void GameManager::render()
{
    _window->setView(_camera);

    _paddle->render();
    for (Ball* _b : _balls)
    {
        _b->render();
    }
    //_ball->render();
    _brickManager->render();
    _powerupManager->render();
    _window->draw(_masterText);
    _ui->render();
}

void GameManager::levelComplete()
{
    _levelComplete = true;
}

SoundManager* GameManager::getSoundManager() const { return _soundManager; }
sf::RenderWindow* GameManager::getWindow() const { return _window; }
UI* GameManager::getUI() const { return _ui; }
Paddle* GameManager::getPaddle() const { return _paddle; }
BrickManager* GameManager::getBrickManager() const { return _brickManager; }
PowerupManager* GameManager::getPowerupManager() const { return _powerupManager; }
