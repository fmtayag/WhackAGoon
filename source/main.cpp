/*
 * Whack-A-Goon!
 * An SDL video game developed by zyenapz
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

SDL_Window* gWindow;
SDL_Renderer* gRenderer;
SDL_Texture* fooTexture;

bool initialize();
bool loadAssets();
void cleanUp();

int main(int argv, char** args){
    // Initialize and load assets
    bool isInitialized = initialize();
    bool hasLoadedAssets = loadAssets();

    // Game loop
    if(isInitialized & hasLoadedAssets) {
        SceneContext gContext;
        SDL_Event e;
        bool isRunning = true;

        while(isRunning) {

            // Run context methods
            gContext.HandleEvents(&e, isRunning);
            gContext.Update();
            gContext.Draw(gRenderer);

        }
    }

    // Clean up
    cleanUp();

    return 0;
}

bool initialize() {
    bool isSuccessful = true;

    // Initialize SDL subsystems
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("SDL failed to initialize. Error: %s.\n", SDL_GetError());
        isSuccessful = false;
    }
    else {
        // Create window
        gWindow = SDL_CreateWindow(GAME_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
        if(gWindow == NULL) {
            printf("SDL failed to create window. Error: %s.\n", SDL_GetError());
            isSuccessful = false;
        }
        else {
            // Create renderer
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
            if(gRenderer == NULL) {
                printf("SDL failed to create renderer. Error: %s.\n", SDL_GetError());
                isSuccessful = false;
            }
            else {
                // Initialize SDL_image
                int flags = IMG_INIT_JPG | IMG_INIT_PNG;
                if( (IMG_Init(flags) & flags) != flags ) {
                    printf("SDL_image failed to initialize. Error: %s.\n", IMG_GetError());
                    isSuccessful = false;
;               }
            }
        }
    }

    // Return isSuccessful
    return isSuccessful;
}

bool loadAssets() {
    bool isSuccessful = true;

    fooTexture = loadTextureFromFile(gRenderer, "data/images/foo.png");
    if(fooTexture == NULL) {
        printf("Failed to load fooTexture.\n");
        isSuccessful = false;
    }

    return isSuccessful;
}

void cleanUp() {
    // Destroy textures
    SDL_DestroyTexture(fooTexture);
    fooTexture = NULL;

    // Destroy window
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    // Destroy renderer
    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;

    // Quit SDL_image, and SDL
    IMG_Quit();
    SDL_Quit();
}
