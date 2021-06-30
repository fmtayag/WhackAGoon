#include "SDL2/SDL.h"

#pragma once

class Scene {
public:
    virtual void initialize(SDL_Renderer** renderer) = 0;
    virtual void handleEvents(SDL_Event* e) = 0;
    virtual void update() = 0;
    virtual void draw(SDL_Renderer*& renderer) = 0;
    virtual void close() = 0;
};

class SceneContext {
public:
    SceneContext();
    void changeScene(Scene* scene);
    void exit();
    bool isExited();

    void Initialize(SDL_Renderer** renderer);
    void HandleEvents(SDL_Event* e);
    void Update();
    void Draw(SDL_Renderer** renderer);

private:
    Scene* mScene;
    bool isRunning;
};

class PlayScene : public Scene {
public:
    PlayScene(SceneContext* mContext);
    ~PlayScene();

    void initialize(SDL_Renderer** renderer);
    void handleEvents(SDL_Event* e);
    void update();
    void draw(SDL_Renderer*& renderer);
    void close();

private:
    bool isInitialized = false;
    SceneContext* mContext;
    SDL_Texture* fooTexture;
    SDL_Rect fooRect {8, 8, 50, 50};
};
