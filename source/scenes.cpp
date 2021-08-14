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
    mMouseClicked = false;

    const int centerW = (WINDOW_WIDTH/2) - (HOLE_WIDTH/2);
    const int centerH = (WINDOW_HEIGHT/2) - (HOLE_HEIGHT/2);
    const int offsetW = 48;
    const int offsetH = 32;
    // Column 1
    holeSprites.push_back(new HoleSprite(spritesTexture, centerW - offsetW, centerH - 72 + offsetH));
    holeSprites.push_back(new HoleSprite(spritesTexture, centerW - offsetW - 32, centerH + offsetH));
    holeSprites.push_back(new HoleSprite(spritesTexture, centerW - offsetW, centerH + 72 + offsetH));

    // Column 2
    holeSprites.push_back(new HoleSprite(spritesTexture, centerW + offsetW, centerH - 72 + offsetH));
    holeSprites.push_back(new HoleSprite(spritesTexture, centerW + offsetW + 32, centerH + offsetH));
    holeSprites.push_back(new HoleSprite(spritesTexture, centerW + offsetW, centerH + 72 + offsetH));

    hManager = new HoleManager(holeSprites);
}

PlayScene::~PlayScene() {
    hManager = NULL;
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

    // Collision check
    for(std::vector<HoleSprite*>::const_iterator iter = holeSprites.begin(); iter != holeSprites.end(); iter++) {
        bool collided = isPointCollide(mpos, (*iter)->getRect());
        if(collided && mMouseClicked) {
            bool isWhacked = (*iter)->whack();
            mMouseClicked = false;

            if(isWhacked) {
                if((*iter)->getType() == HT_Goon) {
                    score++;
                }
                else if((*iter)->getType() == HT_Townie) {
                    score -= 3;
                }
                else if((*iter)->getType() == HT_Mayor) {
                    printf("Debug: You hit the Mayor. Game over!\n");
                    gameOverMessage = "You hit the Mayor!\n";
//                    isGameOver = true;
                }

            }

            // neg score check
            if(score < 0)
                score = 0;

        }
    }

    // run update methods

    for(std::vector<HoleSprite*>::const_iterator iter = holeSprites.begin(); iter != holeSprites.end(); iter++) {
        (*iter)->update();
    }

    hManager->update();
}

void PlayScene::draw(SDL_Renderer* renderer) {
    // Set render draw color, and clear renderer ----------
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw background ------------------------------------
    SDL_RenderCopy(renderer, bgTexture, NULL, NULL);

    // Draw holes ------------------------
    for(std::vector<HoleSprite*>::const_iterator iter = holeSprites.begin(); iter != holeSprites.end(); iter++) {
        (*iter)->draw(renderer);
    }
//    // Draw play field box --------------------------------
//    SDL_Rect boxRect = {
//        (WINDOW_WIDTH / 2) - (448/2),
//        (WINDOW_HEIGHT / 2) - (528/2),
//        448,
//        528
//    };
//    SDL_RenderCopy(renderer, boxTexture, NULL, &boxRect);

    // Draw texts -----------------------------------------
    std::string scoreMessage = std::to_string(score);
    const int txtCenterW = WINDOW_WIDTH/2;
    const int offsetX = 2;
    drawText(renderer, scoreMessage, gFont, txtCenterW+offsetX, 32, {255,255,255});

    // Render crap ----------------------------------------
    SDL_RenderPresent(renderer);

}


