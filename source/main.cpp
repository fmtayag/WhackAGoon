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

// Constants
const std::string GAME_TITLE = "Whack-A-Goon!";
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

// States
enum STATES {
    ST_MENU,
    ST_PLAYING
};

// Function prototypes
bool initialize(SDL_Window* window, SDL_Renderer* renderer);
bool loadAssets();
void close(SDL_Window* window, SDL_Renderer* renderer);

// State function prototypes
void STMenu_GetInput(SDL_Event* e);
void STMenu_Update();
void STMenu_Draw(SDL_Window* window, SDL_Renderer* renderer);

void STPlay_GetInput(SDL_Event* e);
void STPlay_Update();
void STPlay_Draw(SDL_Window* window, SDL_Renderer* renderer);

int main(int argv, char** args){
    // Declare window and renderer pointers
    SDL_Window* gWindow = NULL;
    SDL_Renderer* gRenderer = NULL;

    ZRect myRect (5, 10, 50, 50);
    printf("%d", myRect.getTopRight().x);

    // Initialize SDL, window, renderer, and SDL_image
    if(!initialize(gWindow, gRenderer)) {
        printf("Game failed to initialize. :(\n");
    }
    else {
        // Load textures, fonts, and music
        if(!loadAssets()) {
            printf("Game failed to load assets. :(\n");
        }
        else {
            // Game loop
            bool isRunning = true;
            int gState = ST_MENU;
            SDL_Event e;

            while(isRunning) {

                // Poll events
                while(SDL_PollEvent(&e)) {
                    if(e.type == SDL_QUIT) {
                        isRunning = false;
                    }
                }

                if(gState == ST_MENU) {
                    // Update

                    // Draw
                    SDL_RenderClear(gRenderer);
                    SDL_RenderPresent(gRenderer);

                }
                else if(gState == ST_PLAYING) {
                    // Update

                    // Draw
                    SDL_RenderClear(gRenderer);
                    SDL_RenderPresent(gRenderer);

                }
            }
        }
    }

    // Close SDL
    close(gWindow, gRenderer);

    return 0;
}

bool initialize(SDL_Window* window, SDL_Renderer* renderer) {
    // - Initializes SDL, window, renderer, and SDL_image
    bool isSuccessful = true;

    // Initialize SDL2
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("SDL failed to initialize. SDL Error: %s\n", SDL_GetError());
        isSuccessful = false;
    }
    else {
        // Initialize Window
        window = SDL_CreateWindow(GAME_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
        if(window == NULL) {
            printf("SDL could not initialize window. SDL Error: %s\n", SDL_GetError());
            isSuccessful = false;
        }
        else {
            // Initialize Renderer
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if(renderer == NULL) {
                printf("SDL could not initialize renderer. SDL Error: %s\n", SDL_GetError());
                isSuccessful = false;
            }
            else {
                // Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if(!(IMG_Init(imgFlags) & imgFlags)) {
                    printf("SDL_image could not initialize. SDL_image Error: %s\n", IMG_GetError());
                    isSuccessful = false;
                }
            }
        }
    }

    return isSuccessful;
}

bool loadAssets() {
    // - Loads textures, fonts, and music
    bool isSuccessful = true;

    return isSuccessful;
}

void close(SDL_Window* window, SDL_Renderer* renderer) {
    // - Closes SDL

    // Destroy window
    SDL_DestroyWindow(window);
    window = NULL;

    // Destroy renderer
    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    // Quit SDL_image, and SDL
    IMG_Quit();
    SDL_Quit();
}
