#include "game.hpp"
#include <iostream>
#include <raylib.h>
#include <fstream>

Game::Game()
{   
    music = LoadMusicStream("Sounds/music.ogg");
    exploisionSound = LoadSound("Sounds/explosion.ogg");
    PlayMusicStream(music);
    InitGame();
}

Game::~Game()
{
    Alien::UnloadImages();
    UnloadMusicStream(music);
    UnloadSound(exploisionSound);
}

void Game::Update()
{   
    if(run){
        double currentTime = GetTime();
        if(currentTime-timeLastSpawn > mysteryShipSpawnInterval){
            mysteryShip.Spawn();
            timeLastSpawn = GetTime();
            mysteryShipSpawnInterval = GetRandomValue(10,20);
        }

        for(auto& laser: spaceship.lasers){
            laser.Update();
        }
        MoveAliens();
        AlienShootLaser();
        for(auto& laser:alien_laser){
            laser.Update();
        }
        mysteryShip.Update();
        DeleteInactiveLasers();
        CheckForCollisions();
    }
    else{
        if(IsKeyDown(KEY_ENTER)){
            Reset();
            InitGame();
        }
    }
}

void Game::Draw()
{
    spaceship.Draw();
    for(auto& laser: spaceship.lasers){
        laser.Draw();
    }
    for(auto& obstacle: obstacles){
        obstacle.Draw();
    }
    for(auto& alien: aliens){
        alien.Draw();
    }
    for(auto& laser:alien_laser){
        laser.Draw();
    }
    mysteryShip.Draw();
}

void Game::HandleInput(){
    if(run){
        if(IsKeyDown(KEY_LEFT)){
            spaceship.MoveLeft();
        }
        else if(IsKeyDown(KEY_RIGHT)){
            spaceship.MoveRight();
        }
        else if(IsKeyDown(KEY_SPACE)){
            spaceship.FireLaser();
        }
    }
}

void Game::DeleteInactiveLasers()
{
    for(auto it = spaceship.lasers.begin();it != spaceship.lasers.end();){
        if(!it -> active){
            it = spaceship.lasers.erase(it);
        }
        else{
            ++it;
        }
    }
    for(auto it = alien_laser.begin();it != alien_laser.end();){
        if(!it -> active){
            it = alien_laser.erase(it);
        }
        else{
            ++it;
        }
    }
}
std::vector<Obstacle> Game::CreateObstacles(){
    int obstacleWidth = Obstacle::grid[0].size() * 3;
    float gap = (GetScreenWidth() - (4 * obstacleWidth)) / 5;
    for(int i =0;i<4;i++){
        float offsetX = (i + 1) * gap + i * obstacleWidth;
        obstacles.push_back(Obstacle({offsetX,float(GetScreenHeight() - 200)}));
    }
    return obstacles;
}

std::vector<Alien> Game::CreateAliens()
{
    std::vector<Alien> aliens;
    for(int row = 0; row < 5 ; row++){
        for(int col = 0; col < 11;col++){
            int alien_type;
            if(row == 0)
                alien_type = 3;
            else if(row ==1 || row == 2)
                alien_type = 2;
            else 
                alien_type = 1;
            float x = 75 + col * 55;
            float y = 110 + row * 55;
            aliens.push_back(Alien(alien_type,{x, y}));
        }
    }
    return aliens;
}

void Game::MoveAliens(){
    for(auto& alien:aliens){
        if(alien.position.x + alien.alienImages[alien.type-1].width > GetScreenWidth()-25){
            aliensDirection = -1;
            MoveDownAliens(4);
        }
        if(alien.position.x < 25){
            aliensDirection = 1;
            MoveDownAliens(4);
        }

        alien.Update(aliensDirection);
    }
}

void Game::MoveDownAliens(int distance)
{
    for(auto& alien:aliens){
        alien.position.y += distance;
    }
}

void Game::AlienShootLaser()
{   
    double currentTime = GetTime();
    if(currentTime - timeLastAlienFired >= alienLaserShootInterval && !aliens.empty()){
        int randomIndex = GetRandomValue(0,aliens.size() - 1);
        Alien& alien = aliens[randomIndex];
        alien_laser.push_back(Laser({alien.position.x + alien.alienImages[alien.type-1].width / 2,
        alien.position.y + alien.alienImages[alien.type - 1].height},-6));
        timeLastAlienFired = GetTime();
    }
    

}

void Game::CheckForCollisions()
{   
    //SpaceShip lasers

    for(auto& laser: spaceship.lasers){
        auto it = aliens.begin();
        while(it != aliens.end()){
            if(CheckCollisionRecs(it -> getRect(),laser.getRect())){
                PlaySound(exploisionSound);
                if(it -> type == 1){
                    score+=100;
                }
                else if(it -> type == 2){
                    score += 200;
                }
                else if( it -> type == 3){
                    score += 300;
                }
                it=aliens.erase(it);
                laser.active = false;
                CheckForHighScore();
            }
            else{
                it++;
            }
        }
        for(auto& obstacle:obstacles){
            auto it = obstacle.blocks.begin();
            while(it != obstacle.blocks.end()){
                if(CheckCollisionRecs(it -> getRect(),laser.getRect())){
                    it=obstacle.blocks.erase(it);
                    laser.active = false;
                }
                else{
                    it++;
                }
            }
        }
        if(CheckCollisionRecs(mysteryShip.getRect(),laser.getRect())){
            PlaySound(exploisionSound);
            mysteryShip.alive = false;
            laser.active = false;
            score += 500;
            CheckForHighScore();
        }
    }
    // Alien lasers
    for(auto& laser: alien_laser){
        if(CheckCollisionRecs(laser.getRect(),spaceship.getRect())){
            laser.active = false;
            lives--;
            if(lives == 0){
                GameOver();
            }
        }
       for(auto& obstacle:obstacles){
            auto it = obstacle.blocks.begin();
            while(it != obstacle.blocks.end()){
                if(CheckCollisionRecs(it -> getRect(),laser.getRect())){
                    it=obstacle.blocks.erase(it);
                    laser.active = false;
                }
                else{
                    it++;
                }
            }
        }
    }
    // Alien Collision with Obstacle
    for(auto& alien:aliens){
        for(auto& obstacle: obstacles){
            auto it = obstacle.blocks.begin();
            while(it != obstacle.blocks.end()){
                if(CheckCollisionRecs(it -> getRect(),alien.getRect())){
                    it = obstacle.blocks.erase(it);
                }
                else{
                    it++;
                }
            }
        }
        if(CheckCollisionRecs(alien.getRect(),spaceship.getRect())){
            GameOver();
        }
    }
}

void Game::GameOver(){
    run = false;
}

void Game::InitGame()
{
    obstacles = CreateObstacles();
    aliens = CreateAliens();
    aliensDirection = 1;
    timeLastAlienFired = 0.0;
    mysteryShip.Spawn();
    timeLastSpawn =0.0;
    mysteryShipSpawnInterval = GetRandomValue(10,20);
    lives = 3;
    run = true;
    score = 0;
    Highscore = loadHighScoreToFile();
}

void Game::CheckForHighScore()
{
    if(score > Highscore){
        Highscore = score;
        saveHighScoreToFile(Highscore);
    }
}

void Game::saveHighScoreToFile(int highscore)
{
    std::ofstream highScoreFile("highscore.txt");
    if(highScoreFile.is_open()){
        highScoreFile<<highscore;
        highScoreFile.close();
    }
    else{
        std::cerr<<"Failed to save highscore to file"<<std::endl;
    }
}

int Game::loadHighScoreToFile()
{
    int loadedHighScore = 0;
    std::ifstream highScoreFile("highscore.txt");
    if(highScoreFile.is_open()){
        highScoreFile >> loadedHighScore;
        highScoreFile.close();
    }
    else{
        std::cerr<<"Failed to load highscore"<<std::endl;
    }
    return loadedHighScore;
}

void Game::Reset()
{
    spaceship.Reset();
    aliens.clear();
    alien_laser.clear();
    obstacles.clear();
}