#ifndef SCENES_H
#define SCENES_H
#pragma once

#include <SDL2/SDL.h>
#include "g_data.h"
#include "widgets.h"

class SceneContext;
class Scene
{
protected:
    SceneContext *m_context;

public:
    virtual ~Scene() = default;

    virtual void handleEvents(SDL_Event *e) = 0;
    virtual void update() = 0;
    virtual void draw(SDL_Renderer *renderer) = 0;

    void setContext(SceneContext *context)
    {
        this->m_context = context;
    }
};

class SceneContext
{
private:
    Scene *m_scene;
    GameAssets *m_gameAssets;
    bool m_flagQuitting;

public:
    // Ctor & Dtor
    SceneContext(Scene *scene) : m_scene(nullptr)
    {
        this->transitionTo(scene);
    }
    ~SceneContext() { delete m_scene; }

    // Scene transition
    void transitionTo(Scene *scene)
    {
        std::cout << "Context: Transition to " << typeid(*scene).name() << ".\n";
        if (this->m_scene != nullptr)
            delete this->m_scene;
        this->m_scene = scene;
        this->m_scene->setContext(this);
    }

    // Game assets
    void bindGameAssets(GameAssets *gameAssets) { m_gameAssets = gameAssets; }
    GameAssets *fetchGameAssets() { return m_gameAssets; }

    // Flags
    void setQuitFlag(bool value) { m_flagQuitting = value; }
    const bool fetchQuitFlag() { return m_flagQuitting; }

    // Delegates
    void handleEvents(SDL_Event *e)
    {
        this->m_scene->handleEvents(e);
    }

    void update()
    {
        this->m_scene->update();
    }

    void draw(SDL_Renderer *renderer)
    {
        this->m_scene->draw(renderer);
    }
};

class MenuScene : public Scene
{
private:
    GTimer tmr1;

public:
    MenuScene();
    ~MenuScene();

    void handleEvents(SDL_Event *e);
    void update();
    void draw(SDL_Renderer *renderer);
};

class PlayScene : public Scene
{
public:
    PlayScene();
    ~PlayScene();

    void handleEvents(SDL_Event *e);
    void update();
    void draw(SDL_Renderer *renderer);
};

#endif // SCENES_H