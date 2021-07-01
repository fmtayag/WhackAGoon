#ifndef SCENES_H
#define SCENES_H
#pragma once

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include "entities.h"

class AbstractScene {
public:
    virtual void handleEvents(SDL_Event* e, bool& isRunning) = 0;
    virtual void update() = 0;
    virtual void draw(SDL_Renderer* renderer) = 0;
};

class SceneContext {
public:
    SceneContext();
    void changeScene(AbstractScene* scene);

    void HandleEvents(SDL_Event* e, bool& isRunning);
    void Update();
    void Draw(SDL_Renderer* renderer);

private:
    AbstractScene* mScene;
    bool isRunning;
};

class PlayScene : public AbstractScene {
public:
    PlayScene(SceneContext* mContext);
    ~PlayScene();

    void handleEvents(SDL_Event* e, bool& isRunning);
    void update();
    void draw(SDL_Renderer* renderer);

private:
    SceneContext* mContext;
    std::vector<AbstractEntity*> gEntities;
};

#endif // SCENES_H




