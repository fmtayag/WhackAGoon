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
    score = 0;

    // Textures list --------------------------------------
    mHoleTextures[0] = &goonTexture;
    mHoleTextures[1] = &townieTexture;
    mHoleTextures[2] = &mayorTexture;

    // Create holes entities ------------------------------
    HoleEntity* hole1 = new HoleEntity(mHoleTextures, (WINDOW_WIDTH / 2) - 92, 60);
    HoleEntity* hole2 = new HoleEntity(mHoleTextures, (WINDOW_WIDTH / 2) + 32, 60);
    HoleEntity* hole3 = new HoleEntity(mHoleTextures, (WINDOW_WIDTH / 2) - 92, 150);
    HoleEntity* hole4 = new HoleEntity(mHoleTextures, (WINDOW_WIDTH / 2) + 32, 150);
    HoleEntity* hole5 = new HoleEntity(mHoleTextures, (WINDOW_WIDTH / 2) - 92, 240);
    HoleEntity* hole6 = new HoleEntity(mHoleTextures, (WINDOW_WIDTH / 2) + 32, 240);
    mEntities.push_back(hole1);
    mEntities.push_back(hole2);
    mEntities.push_back(hole3);
    mEntities.push_back(hole4);
    mEntities.push_back(hole5);
    mEntities.push_back(hole6);

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
}

void PlayScene::update() {
    // Check for collision --------------------------------
    int mx, my;
    SDL_GetMouseState(&mx, &my);
    int mpos[2] = {mx, my};

    std::vector<HoleEntity*>::iterator iter;
    for(iter = mEntities.begin(); iter != mEntities.end(); iter++) {
        bool isCollide = isPointCollide(mpos, (*iter)->getRect());
        if(isCollide && mMouseClicked) {
            bool isActive = (*iter)->whack();

            // Add score
            if(isActive)
                score++;

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

    // Draw hole entities ---------------------------------
    std::vector<HoleEntity*>::iterator iter;
    for(iter = mEntities.begin(); iter != mEntities.end(); iter++) {
        (*iter)->draw(renderer);
    }

    // Draw hammer ----------------------------------------
    mHammer->draw(renderer);

    // Render crap ----------------------------------------
    SDL_RenderPresent(renderer);
    //printf("Score: %d\n", score);
}


