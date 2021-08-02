#include <iostream>
#include <iterator>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "scenes.h"
#include "metadata.h"
#include "assets.h"
#include "entities.h" // replace with sprites.h soon
#include "sprites.h"
#include "utils.h"

/* -------------------------------------------------
 * Scene Context def
 * -------------------------------------------------
 */
SceneContext::SceneContext() {
    mScene = new PlayScene(this);
}

void SceneContext::changeScene(AbstractScene* scene) {
    delete this->mScene;
    mScene = scene;
}

void SceneContext::HandleEvents(SDL_Event* e, bool& isRunning) {
    mScene->handleEvents(e, isRunning);
}

void SceneContext::Update() {
    mScene->update();
}

void SceneContext::Draw(SDL_Renderer* renderer) {
    mScene->draw(renderer);
}

/* -------------------------------------------------
 * Play Scene def
 * -------------------------------------------------
 */
PlayScene::PlayScene(SceneContext* context) {
    mContext = context;

    gameTimer = SDL_GetTicks();
    gameDuration = 30000; // 30 secs
    score = 0;
    towniesHit = 0;
    isGameOver = false;
    gameOverMessage = "GAME OVER MESSAGE";

    hSprite = new HoleSprite(spritesTexture, 0, 0);

}

PlayScene::~PlayScene() {

}

void PlayScene::handleEvents(SDL_Event* e, bool& isRunning) {
    // Handle events --------------------------------------
    while(SDL_PollEvent(e)) {
        if(e->type == SDL_QUIT) {
            isRunning = false;
        }
        if(e->type == SDL_MOUSEBUTTONDOWN) {
            mMouseClicked = true;
        }
        if(e->type == SDL_MOUSEBUTTONUP) {
            mMouseClicked = false;
        }

        // Debug
        if(e->type == SDL_KEYDOWN) {
            switch(e->key.keysym.sym) {
            case SDLK_F1:
                hSprite->awake();
                break;
            case SDLK_F2:
                hSprite->whack();
                break;
            default:
                break;
            }
        }
    }

    // End game check
    if(isGameOver) {
        isRunning = false;
        printf("Game over: %s!\n", gameOverMessage.c_str());
    }

    // Update mouse position
    SDL_GetMouseState(&mx, &my);
    mpos[0] = mx;
    mpos[1] = my;

}

void PlayScene::update() {
    hSprite->update();
}

void PlayScene::draw(SDL_Renderer* renderer) {
    // Set render draw color, and clear renderer ----------
    SDL_SetRenderDrawColor(renderer, 100, 50, 200, 255);
    SDL_RenderClear(renderer);

    hSprite->draw(renderer);

//    // Draw background ------------------------------------
//    SDL_RenderCopy(renderer, bgTexture, NULL, NULL);
//
//    // Draw play field box --------------------------------
//    SDL_Rect boxRect = {
//        (WINDOW_WIDTH / 2) - (448/2),
//        (WINDOW_HEIGHT / 2) - (528/2),
//        448,
//        528
//    };
//    SDL_RenderCopy(renderer, boxTexture, NULL, &boxRect);

    // Draw texts -----------------------------------------
    std::string scoreMessage = "Score: " + std::to_string(score);
    drawText(renderer, scoreMessage, gFont, 10, 10, {255,255,255});

    // Render crap ----------------------------------------
    SDL_RenderPresent(renderer);

}


