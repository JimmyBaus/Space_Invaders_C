#pragma once
#include "spaceship.hpp"
#include "obstacle.hpp"
#include "alien.hpp"
#include "mysteryship.hpp"

class Game{
    public:
        Game();
        ~Game();
        void Draw();
        void Update();
        void HandleInput();
        bool run;
        int lives;
        int score;
        int Highscore;
        Music music;
    private:
        void CheckForCollisions();
        void DeleteInactiveLasers();
        std::vector<Obstacle> CreateObstacles();
        std::vector<Alien> CreateAliens();
        void MoveAliens();
        void MoveDownAliens(int distance);
        void AlienShootLaser();
        void GameOver();
        void Reset();
        void InitGame();
        void CheckForHighScore();
        void saveHighScoreToFile(int highscore);
        int loadHighScoreToFile();
        Spaceship spaceship;
        std::vector<Obstacle> obstacles;
        std::vector<Alien> aliens;
        int aliensDirection;
        std::vector<Laser> alien_laser;
        constexpr static float alienLaserShootInterval = 0.35;
        float timeLastAlienFired;
        MysteryShip mysteryShip;
        float mysteryShipSpawnInterval;
        float timeLastSpawn;
        Sound exploisionSound;
};