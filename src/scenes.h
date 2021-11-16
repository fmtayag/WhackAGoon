#ifndef SCENES_H
#define SCENES_H
#pragma once

#include <iostream>
#include <memory>
#include <SDL.h>
#include "g_data.h"
#include "widgets.h"
#include "sprites.h"

class SceneContext;
class Scene
{
protected:
    enum SceneTransitionFlags
    {
        STF_NONE,
        STF_TOPLAY,
        STF_TOMENU
    };
    SceneContext *m_context;
    GameMouse m_gMouse;
    SceneTransitionFlags m_stflag = STF_NONE;

    void resetMouseState()
    {
        SDL_GetMouseState(&m_gMouse.position.x, &m_gMouse.position.y);
        m_gMouse.isClicked = false;
    };

public:
    virtual ~Scene() = default;

    virtual void handleEvents(SDL_Event *e) = 0;
    virtual void update() = 0;
    virtual void draw() = 0;

    void setContext(SceneContext *context)
    {
        this->m_context = context;
    }

    virtual void checkTransition() final;
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
        m_gameAssets = new GameAssets;
        this->transitionTo(scene);
    }
    ~SceneContext()
    {
        delete m_scene;
        delete m_gameAssets;
    }

    // Scene transition
    void transitionTo(Scene *scene)
    {
        printf("Context: Transition to %s.\n", typeid(*scene).name());
        if (this->m_scene != nullptr)
            delete this->m_scene;
        this->m_scene = scene;
        this->m_scene->setContext(this);
    }

    // Game assets
    void bindGameAssets(GameAssets &gameAssets) { *m_gameAssets = gameAssets; }
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

    void draw()
    {
        this->m_scene->draw();
    }

    void checkTransition()
    {
        this->m_scene->checkTransition();
    }
};

class MenuScene : public Scene
{
private:
    // Buttons
    std::unique_ptr<GButton> btnPlay;
    std::unique_ptr<GButton> btnHelp;
    std::unique_ptr<GButton> btnInfo;

    // Textures
    std::unique_ptr<GTexture> uiElementsTexture;
    std::unique_ptr<GTexture> brickBGTexture;
    std::unique_ptr<GTexture> logoTexture;

    // Callbacks
    void cbPlay();
    void cbHelp();
    void cbInfo();

    // Initializer functions
    void loadAssets();
    void createButtons();

public:
    MenuScene();
    ~MenuScene() override;

    void handleEvents(SDL_Event *e) override;
    void update() override;
    void draw() override;
};

class PlayScene : public Scene
{
private:
    // Buttons
    std::unique_ptr<GButton> btnToMenu;

    // Textures
    std::unique_ptr<GTexture> uiElementsTexture;
    std::unique_ptr<GTexture> cityBGTexture;
    std::unique_ptr<GTexture> auraBGTexture;

    // Callbacks
    void cbToMenu();

    // Initializer functions
    void loadAssets();
    void createButtons();

public:
    PlayScene();
    ~PlayScene() override;

    void handleEvents(SDL_Event *e) override;
    void update() override;
    void draw() override;
};

#endif // SCENES_H