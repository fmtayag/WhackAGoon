#include "Scenes.h"
#include <iostream>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

/*
* SceneContext
*/

SceneContext::SceneContext(SDL_Renderer** renderer) {
    mScene = new PlayScene(this);
    mRenderer = *renderer;
}

void SceneContext::changeScene(Scene* scene) {
    delete this->mScene;
    mScene = scene;
}

void SceneContext::HandleEvents(SDL_Event* e) {
    mScene->handleEvents(e);
}

void SceneContext::Update() {
    mScene->update();
}

void SceneContext::Draw() {
    mScene->draw();
}

void SceneContext::exit() {
    isRunning = false;
    delete this->mScene;
}

bool SceneContext::isExited() {
    return isRunning;
}

SDL_Renderer* SceneContext::getRenderer() {
    return mRenderer;
}

/*
* PlayScene
*/

PlayScene::PlayScene(SceneContext* context) {
    mContext = context;
    initialize();
}

PlayScene::~PlayScene() {
    close();
}

void PlayScene::initialize() {

}

void PlayScene::handleEvents(SDL_Event* e) {

    // Handle Events

    while(SDL_PollEvent(e)) {
        if(e->type == SDL_QUIT) {
            mContext->exit();
        }
        if(e->type == SDL_KEYDOWN) {
            switch(e->key.keysym.sym) {
            case SDLK_LEFT:
                break;
            }
        }
    }

}

void PlayScene::update() {
    // Update
}

void PlayScene::draw() {
    SDL_SetRenderDrawColor(mContext->getRenderer(), 0, 50, 50, 255);
    SDL_RenderClear(mContext->getRenderer());
    SDL_RenderPresent(mContext->getRenderer());
}

void PlayScene::close() {
    // Close
}
