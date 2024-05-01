#include <raylib.h>
#include "game.hpp"
#include <string>

std::string FormatWithLeadingZeros(int number,int width);

int main(){
    Color grey = {29, 29, 27, 255};
    Color yellow = {243, 216, 63, 255};
    int offset = 50;
    int windowWidth = 750;
    int windowHeight = 700;

    InitWindow(windowWidth + offset,windowHeight + 2*offset,"C++ Space Invaders");
    InitAudioDevice();
    Font font = LoadFontEx("Font/monogram.ttf",64,0,0);
    Texture2D SpaceshipImage = LoadTexture("Graphics/spaceship.png");
    SetTargetFPS(60);
    Game game;

    while (WindowShouldClose() == false)
    {
        UpdateMusicStream(game.music);
        game.HandleInput();
        game.Update();

        BeginDrawing();
        ClearBackground(grey);

        DrawRectangleRoundedLines({10, 10, 780, 780}, 0.18f, 20, 2, yellow);
        DrawLineEx({25, 730}, {775, 730}, 3, yellow);
        if(game.run == true)
            DrawTextEx(font, "LEVEL 01", {570, 740}, 34, 2, yellow);
        else
            DrawTextEx(font, "GAME OVER", {570, 740}, 34, 2, yellow);

        float x = 50.0;
        for(int i =0;i<game.lives;i++){
            DrawTextureV(SpaceshipImage,{x, 745}, WHITE);
            x += 50.0;
        }

        DrawTextEx(font, "SCORE", {50, 15}, 34, 2, yellow);
        std::string scoreText = FormatWithLeadingZeros(game.score, 5);
        
        DrawTextEx(font,scoreText.c_str(), {50, 40}, 34, 2, yellow); 
        DrawTextEx(font, "HIGH-SCORE", {570, 15}, 34, 2, yellow);
        std::string hieghscoretext = FormatWithLeadingZeros(game.Highscore,5);
        DrawTextEx(font, hieghscoretext.c_str(),{655, 40}, 34, 2, yellow);
        
        
        
        
        game.Draw();
        EndDrawing();
    }
    CloseWindow();
    CloseAudioDevice();
}

std::string FormatWithLeadingZeros(int number,int width){
    std::string numberText = std::to_string(number);
    int leadingzeros = 5 - numberText.length();
    return numberText = std::string(leadingzeros,'0') + numberText;
}