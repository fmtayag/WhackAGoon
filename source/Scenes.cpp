#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "scenes.h"
#include "metadata.h"
#include "game_textures.h"

/*
 * SceneContext
 */
SceneContext::SceneContext() {
    mScene = new PlayScene(this);
}

void SceneContext::changeScene(Scene* scene) {
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
}

PlayScene::~PlayScene() {

}

void PlayScene::handleEvents(SDL_Event* e, bool& isRunning) {
    // Handle Events
    while(SDL_PollEvent(e)) {
        if(e->type == SDL_QUIT) {
            isRunning = false;
        }
    }
}

void PlayScene::update() {
    // Update
}

void PlayScene::draw(SDL_Renderer* renderer) {
    // Draw
    SDL_SetRenderDrawColor(renderer, 0, 50, 50, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, fooTexture, NULL, NULL);
    SDL_RenderPresent(renderer);
}


