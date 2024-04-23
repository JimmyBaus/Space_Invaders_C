#pragma once
#include "spaceship.hpp"
#include "obstacle.hpp"
#include "alien.hpp"

class Game{
    public:
        Game();
        ~Game();
        void Draw();
        void Update();
        void HandleInput();
    private:
        void DeleteInactiveLasers();
        std::vector<Obstacle> CreateObstacles();
        std::vector<Alien> CreateAliens();
        void MoveAliens();
        void MoveDownAliens(int distance);
        void AlienShootLaser();
        Spaceship spaceship;
        std::vector<Obstacle> obstacles;
        std::vector<Alien> aliens;
        int aliensDirection;
        std::vector<Laser> alien_laser;
};