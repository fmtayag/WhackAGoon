#include "SDL2/SDL.h"

#pragma once

class Scene {
public:
    virtual void initialize() = 0;
    virtual void handleEvents(SDL_Event* e) = 0;
    virtual void update() = 0;
    virtual void draw() = 0;
    virtual void close() = 0;
};

class SceneContext {
public:
    SceneContext(SDL_Renderer** renderer);
    void changeScene(Scene* scene);
    void exit();
    bool isExited();
    SDL_Renderer* getRenderer();

    void HandleEvents(SDL_Event* e);
    void Update();
    void Draw();

private:
    Scene* mScene;
    bool isRunning;
    SDL_Renderer* mRenderer;
};

class PlayScene : public Scene {
public:
    PlayScene(SceneContext* mContext);
    ~PlayScene();

    void initialize();
    void handleEvents(SDL_Event* e);
    void update();
    void draw();
    void close();

private:
    SceneContext* mContext;
};
