#include "Scenes.h"
#include <iostream>

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

void SceneContext::Draw(SDL_Renderer* renderer) {
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
    initialize();
}

PlayScene::~PlayScene() {
    close();
}

void PlayScene::initialize() {
    //printf("Play Scene - Initializing\n");
}

void PlayScene::handleEvents(SDL_Event* e) {

    //printf("Play Scene - Handling events\n");

    while(SDL_PollEvent(e)) {
        if(e->type == SDL_QUIT) {
            mContext->exit();
        }
        if(e->type == SDL_KEYDOWN) {
            switch(e->key.keysym.sym) {
            case SDLK_LEFT:
                printf("pressed left.\n");
                break;
            }
        }
    }

}

void PlayScene::update() {
    //printf("Play Scene - Updating\n");
}

void PlayScene::draw(SDL_Renderer* renderer) {
    //printf("Play Scene - Drawing\n");
}

void PlayScene::close() {
    //printf("Play Scene - Closing\n");
}
