#include "Scenes.h"
#include <iostream>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

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

void SceneContext::Initialize(SDL_Renderer** renderer) {
    mScene->initialize(renderer);
}

void SceneContext::HandleEvents(SDL_Event* e) {
    mScene->handleEvents(e);
}

void SceneContext::Update() {
    mScene->update();
}

void SceneContext::Draw(SDL_Renderer** renderer) {
    mScene->draw(*renderer);
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
    close();
}

void PlayScene::initialize(SDL_Renderer** renderer) {
    if(!isInitialized) {
        SDL_Surface* fooSurface = IMG_Load("foo.png");
        fooTexture = SDL_CreateTextureFromSurface(*renderer, fooSurface);
        SDL_FreeSurface(fooSurface);
        fooSurface = NULL;

        isInitialized = true;
    }

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
                fooRect.x -= 5;
                break;
            case SDLK_RIGHT:
                fooRect.x += 5;
                break;
            case SDLK_UP:
                fooRect.y -= 5;
                break;
            case SDLK_DOWN:
                fooRect.y += 5;
                break;
            case SDLK_q:
                break;
            }
        }
    }

}

void PlayScene::update() {
    // Update
}

void PlayScene::draw(SDL_Renderer*& renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 50, 50, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, fooTexture, NULL, &fooRect);
    SDL_RenderPresent(renderer);
}

void PlayScene::close() {

}
