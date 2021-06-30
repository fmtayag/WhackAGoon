#include "SDL2/SDL.h"

#pragma once

class Scene {
public:
    virtual void initialize() = 0;
    virtual void handleEvents(SDL_Event* e) = 0;
    virtual void update() = 0;
    virtual void draw(SDL_Renderer* renderer) = 0;
    virtual void close() = 0;
};

class SceneContext {
public:
    SceneContext();
    void changeScene(Scene* scene);
    void exit();
    bool isExited();

    void HandleEvents(SDL_Event* e);
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

    void initialize();
    void handleEvents(SDL_Event* e);
    void update();
    void draw(SDL_Renderer* renderer);
    void close();

private:
    SceneContext* mContext;
};
