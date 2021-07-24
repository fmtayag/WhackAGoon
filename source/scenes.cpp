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
#include "entities.h"
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

    // Textures list --------------------------------------
    mHoleTextures[0] = &goonTexture;
    mHoleTextures[1] = &townieTexture;
    mHoleTextures[2] = &mayorTexture;

    // Create holes entities ------------------------------
    HoleEntity* hole1 = new HoleEntity(mHoleTextures, (WINDOW_WIDTH / 2) - 128-32, 128);
    HoleEntity* hole2 = new HoleEntity(mHoleTextures, (WINDOW_WIDTH / 2) + 128-96, 128);
    HoleEntity* hole3 = new HoleEntity(mHoleTextures, (WINDOW_WIDTH / 2) - 128-32, 256);
    HoleEntity* hole4 = new HoleEntity(mHoleTextures, (WINDOW_WIDTH / 2) + 128-96, 256);
    HoleEntity* hole5 = new HoleEntity(mHoleTextures, (WINDOW_WIDTH / 2) - 64, 384);
    mEntities.push_back(hole1);
    mEntities.push_back(hole2);
    mEntities.push_back(hole3);
    mEntities.push_back(hole4);
    mEntities.push_back(hole5);

    // Initialize mouse entity ----------------------------
    mHammer = new HammerEntity(&hammerTexture);
}

PlayScene::~PlayScene() {
    // TODO: Set pointers to null
    mHoleTextures[0] = NULL;
    mHoleTextures[1] = NULL;
    mHoleTextures[2] = NULL;
    mHammer = NULL;
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
    // Check for collision --------------------------------
    std::vector<HoleEntity*>::iterator iter;
    for(iter = mEntities.begin(); iter != mEntities.end(); iter++) {
        bool isCollide = isPointCollide(mpos, (*iter)->getRect());

        if(isCollide && mMouseClicked) {
            bool isActiveState = (*iter)->whack();

            if(isActiveState) {
                int holeType = (*iter)->getType();

                switch(holeType) {
                    case TYPE_GOON:
                        score++;
                        break;
                    case TYPE_TOWNIE:
                        score-=5;
                        towniesHit++;

                        if(towniesHit >= MAX_TOWNIE_HITS) {
                            isGameOver = true;
                            gameOverMessage = "You hit too many Townies";
                        }

                        break;
                    case TYPE_MAYOR:
                        score-=10;
                        isGameOver = true;
                        gameOverMessage = "You hit the Mayor";
                        break;
                }

                // Prevent negative score
                if(score < 0) {
                    score = 0;
                }
            }

            continue;
        }
    }

    // Update hammer entity -------------------------------
    if(mMouseClicked) {
        mHammer->smash();
        mMouseClicked = false;
    }

    mHammer->update();

    // Update entities ------------------------------------
    for(iter = mEntities.begin(); iter != mEntities.end(); iter++) {
        (*iter)->update();
    }
}

void PlayScene::draw(SDL_Renderer* renderer) {
    // Set render draw color, and clear renderer ----------
    SDL_SetRenderDrawColor(renderer, 155, 188, 15, 255);
    SDL_RenderClear(renderer);

    // Draw background ------------------------------------
    SDL_RenderCopy(renderer, bgTexture, NULL, NULL);

    // Draw play field box --------------------------------
    SDL_Rect boxRect = {
        (WINDOW_WIDTH / 2) - (448/2),
        (WINDOW_HEIGHT / 2) - (528/2),
        448,
        528
    };
    SDL_RenderCopy(renderer, boxTexture, NULL, &boxRect);

    // Draw hole entities ---------------------------------
    std::vector<HoleEntity*>::iterator iter;
    for(iter = mEntities.begin(); iter != mEntities.end(); iter++) {
        (*iter)->draw(renderer);
    }

    // Draw hammer ----------------------------------------
    mHammer->draw(renderer);

    // Draw texts -----------------------------------------
    std::string scoreMessage = "Score: " + std::to_string(score);
    drawText(renderer, scoreMessage, gFont, 10, 10, {255,255,255});

    // Render crap ----------------------------------------
    SDL_RenderPresent(renderer);

}


