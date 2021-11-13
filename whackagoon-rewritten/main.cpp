/*
 * Whack-A-Goon!
 * Author: zyenapz
 * The source code for this game is available on my GitHub page
 * and is licensed under the GPLv3 license.
 * My GitHub Page: github.com/zyenapz
 * My E-mail: zyenapz@gmail.com
 */

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "g_data.h"
#include "scenes.h"

SDL_Window *gameWindow;
SDL_Renderer *gameRenderer;
SceneContext *gameContext;
GameAssets gameAssets;
WindowMetadata winMetadata;

SDL_Texture *loadTextureFromFile(SDL_Renderer *renderer, std::string path)
{
    SDL_Surface *tempSurface = NULL;
    SDL_Texture *texture = NULL;

    tempSurface = IMG_Load(path.c_str());

    if (tempSurface == NULL)
        printf("SDL_image could not load image. Error: %s.\n", IMG_GetError());
    else
    {
        texture = SDL_CreateTextureFromSurface(renderer, tempSurface);

        if (texture == NULL)
            printf("SDL could not create texture from surface. Error: %s.\n", SDL_GetError());
    }

    SDL_FreeSurface(tempSurface);

    return texture;
}

void initializeGame();
void loadAssets();
void runGame();
void destroyGame();

int main(int argv, char **args)
{
    initializeGame();
    loadAssets();
    runGame();
    destroyGame();

    return 0;
}

void initializeGame()
{
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
    SDL_Event event;

    while (gameContext->fetchQuitFlag() != true)
    {
        SDL_Delay(winMetadata.FPS);

        gameContext->handleEvents(&event);
        gameContext->update();
        gameContext->draw(gameRenderer);
    }
}

void loadAssets()
{
    gameAssets.brickBGTexture = loadTextureFromFile(gameRenderer, "assets/images/menu_bg.png");
    if (gameAssets.brickBGTexture == NULL)
        printf("Failed to load brickBGTexture. SDL_Error: %s.\n");

    gameAssets.cityBGTexture = loadTextureFromFile(gameRenderer, "assets/images/city_bg.png");
    if (gameAssets.cityBGTexture == NULL)
        printf("Failed to load cityBGTexture. SDL_Error: %s.\n");

    gameContext->bindGameAssets(&gameAssets);
}
void destroyGame()
{
    SDL_DestroyWindow(gameWindow);
    SDL_DestroyRenderer(gameRenderer);
    SDL_DestroyTexture(gameAssets.brickBGTexture);
    SDL_DestroyTexture(gameAssets.cityBGTexture);
}