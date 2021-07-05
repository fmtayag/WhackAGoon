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

/*
 * SceneContext
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

/*
 * PlayScene
 */
PlayScene::PlayScene(SceneContext* context) {
    mContext = context;

    // Create goon
    GoonEntity* goon1 = new GoonEntity(goonTexture, 100, 10);
    GoonEntity* goon2 = new GoonEntity(goonTexture, 150, 250);
    gEntities.push_back(goon1);
    gEntities.push_back(goon2);
}

PlayScene::~PlayScene() {

}

void PlayScene::handleEvents(SDL_Event* e, bool& isRunning) {
    // Handle Events
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
    // Update
    SDL_Rect fooRect = {0, 0, 50, 50};
    int mx, my;
    SDL_GetMouseState(&mx, &my);
    int mpos[2] = {mx, my};

    // Check for collision
    std::vector<AbstractEntity*>::iterator iter;
    for(iter = gEntities.begin(); iter != gEntities.end(); iter++) {
        bool isCollide = isPointCollide(mpos, (*iter)->getRect());
        if(isCollide)
            printf("Is colliding. %d\n", mMouseClicked);
    }


    // Update entities in gEntities
    for(iter = gEntities.begin(); iter != gEntities.end(); iter++) {
        (*iter)->update();
    }
}

void PlayScene::draw(SDL_Renderer* renderer) {
    // Draw
    SDL_SetRenderDrawColor(renderer, 197, 145, 84, 255);
    SDL_RenderClear(renderer);

    // Draw entities in gEntities
    std::vector<AbstractEntity*>::iterator iter;
    for(iter = gEntities.begin(); iter != gEntities.end(); iter++) {
        (*iter)->draw(renderer);
    }

    SDL_RenderPresent(renderer);
}


