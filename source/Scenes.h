#ifndef SCENES_H
#define SCENES_H
#pragma once

#include <SDL2/SDL.h>

class Scene {
public:
    virtual void handleEvents(SDL_Event* e, bool& isRunning) = 0;
    virtual void update() = 0;
    virtual void draw(SDL_Renderer* renderer) = 0;
};

class SceneContext {
public:
    SceneContext();
    void changeScene(Scene* scene);

    void HandleEvents(SDL_Event* e, bool& isRunning);
    void Update();
    void Draw(SDL_Renderer* renderer);

private:
    Scene* mScene;
    bool isRunning;
};

class PlayScene : public Scene {
public:
    PlayScene(SceneContext* mContext);
    ~PlayScene();

    void handleEvents(SDL_Event* e, bool& isRunning);
    void update();
    void draw(SDL_Renderer* renderer);

private:
    SceneContext* mContext;
};

#endif // SCENES_H




