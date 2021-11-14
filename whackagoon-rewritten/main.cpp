/*
 * Whack-A-Goon!
 * Author: zyenapz
 * The source code for this game is available on my GitHub page
 * and is licensed under the GPLv3 license.
 * My GitHub Page: github.com/zyenapz
 * My E-mail: zyenapz@gmail.com
 */

#include <iostream>
#include <cassert>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "g_data.h"
#include "scenes.h"
#include "helpers.h"
#include "g_texture.h"

SDL_Window *gameWindow;
SDL_Renderer *gameRenderer;
SceneContext *gameContext;

// Textures
GTexture uiTexture;

void initializeGame();
void runGame();
void destroyGame();

int main(int argv, char **args)
{
    initializeGame();
    runGame();
    destroyGame();

    return 0;
}

void initializeGame()
{
    WindowMetadata winMetadata;

    SDL_Init(SDL_INIT_EVERYTHING);

    gameWindow = SDL_CreateWindow(
        winMetadata.TITLE.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        winMetadata.WINDOW_WIDTH, winMetadata.WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN);

    gameRenderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

    gameContext = new SceneContext(new MenuScene());

    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

    TTF_Init();
}
void runGame()
{
    WindowMetadata winMetadata;
    SDL_Event event;

    while (gameContext->fetchQuitFlag() != true)
    {
        SDL_Delay(winMetadata.FPS);

        gameContext->handleEvents(&event);
        gameContext->update();
        gameContext->draw();
    }
}
void destroyGame()
{
    SDL_DestroyWindow(gameWindow);
    SDL_DestroyRenderer(gameRenderer);
}