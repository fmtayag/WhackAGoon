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
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "g_data.h"
#include "scenes.h"
#include "helpers.h"

SDL_Window *gameWindow;
SDL_Renderer *gameRenderer;
SceneContext *gameContext;

// Textures
GTexture uiTexture;

void initializeGame();
void runGame();
void destroyGame();

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

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
        winMetadata.WIDTH, winMetadata.HEIGHT,
        SDL_WINDOW_SHOWN);

    SDL_Surface *iconSurf = IMG_Load("assets/images/icon.png");
    SDL_SetWindowIcon(gameWindow, iconSurf);
    SDL_FreeSurface(iconSurf);

    gameRenderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

    gameContext = new SceneContext(new MenuScene());

    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

    TTF_Init();

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
}
void runGame()
{
    WindowMetadata winData;
    SDL_Event event;

    GTimer fpsTmr;
    GTimer capTmr;

    int countedFrames = 0;
    fpsTmr.start();

    while (gameContext->fetchQuitFlag() != true)
    {
        capTmr.start();

        // Calc and correct FPS
        float avgFPS = countedFrames / (fpsTmr.getTicks() / 1000.0f);
        if (avgFPS > 2000000.0f)
        {
            avgFPS = 0;
        }

        gameContext->handleEvents(&event);
        gameContext->update();
        gameContext->draw();
        gameContext->checkTransition();

        // Advance frame
        countedFrames++;

        // If frame finished early
        int frameTicks = capTmr.getTicks();
        if (frameTicks < winData.TICKS_PF)
        {
            SDL_Delay(winData.TICKS_PF - frameTicks);
        }
    }
}
void destroyGame()
{
    SDL_DestroyWindow(gameWindow);
    SDL_DestroyRenderer(gameRenderer);
}