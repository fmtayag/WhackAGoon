#include <iostream>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "Scenes.h"
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

void SceneContext::HandleEvents(SDL_Event* e) {
    mScene->handleEvents(e);
}

void SceneContext::Update() {
    mScene->update();
}

void SceneContext::Draw(SDL_Renderer** renderer) {
    mScene->draw(renderer);
}

void SceneContext::exit() {
    isRunning = false;
    delete this->mScene;
}

bool SceneContext::isExited() {
    return isRunning;
}

/*
* PlayScene
*/
PlayScene::PlayScene(SceneContext* context) {
    mContext = context;
}

PlayScene::~PlayScene() {

}

void PlayScene::handleEvents(SDL_Event* e) {

    // Handle Events

    while(SDL_PollEvent(e)) {
        if(e->type == SDL_QUIT) {
            mContext->exit();
        }
    }
}

void PlayScene::update() {
    // Update
}

void PlayScene::draw(SDL_Renderer** renderer) {
    SDL_SetRenderDrawColor(*renderer, 0, 50, 50, 255);
    SDL_RenderClear(*renderer);
    SDL_RenderCopy(*renderer, fooTexture, NULL, NULL);
    SDL_RenderPresent(*renderer);
}


