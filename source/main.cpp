/*
* Whack-A-Goon!
* An SDL video game developed by Francis Tayag (zyenapz)
* The source code for this game is available on my GitHub page
* and is licensed under the GPLv3 license.
* My GitHub Page: github.com/zyenapz
* My E-mail: zyenapz@gmail.com
*/

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "ZRect.h"
#include "Scenes.h"
#include "metadata.h"

void createRenderer(SDL_Window** window, SDL_Renderer** renderer);

int main(int argv, char** args){
    // Initialize
    SDL_Window* gWindow = SDL_CreateWindow(GAME_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* gRenderer = NULL;
    createRenderer(&gWindow, &gRenderer);
    IMG_Init(IMG_INIT_PNG);
    SceneContext gContext(&gRenderer);

    // - Game loop --------------
    SDL_Event e;

    while(gContext.isExited()) {

        // Run context methods
        gContext.HandleEvents(&e);
        gContext.Update();
        gContext.Draw();

    }

    // - Close SDL ---------------

    // Destroy window
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    // Destroy renderer
    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;

    // Quit SDL_image, and SDL
    IMG_Quit();
    SDL_Quit();

    return 0;
}

void createRenderer(SDL_Window** window, SDL_Renderer** renderer) {
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
}
