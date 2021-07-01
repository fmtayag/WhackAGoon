/*
 * Whack-A-Goon!
 * An SDL video game developed by Francis Tayag (zyenapz)
 * The source code for this game is available on my GitHub page
 * and is licensed under the GPLv3 license.
 * My GitHub Page: github.com/zyenapz
 * My E-mail: zyenapz@gmail.com
 */

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "ZRect.h"
#include "scenes.h"
#include "metadata.h"
#include "game_textures.h"
#include "utils.h"

SDL_Texture* fooTexture;

int main(int argv, char** args){
    // Initialize ---------------
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* gWindow = SDL_CreateWindow(GAME_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    IMG_Init(IMG_INIT_PNG);
    SceneContext gContext;

    // Load assets --------------
    fooTexture = loadTextureFromFile(gRenderer, "foo.png");

    // - Game loop --------------
    SDL_Event e;

    while(gContext.isExited()) {

        // Run context methods
        gContext.HandleEvents(&e);
        gContext.Update();
        gContext.Draw(gRenderer);

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
