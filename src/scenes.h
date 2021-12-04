#ifndef SCENES_H
#define SCENES_H
#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <fmt/format.h>
#include <SDL.h>
#include "g_data.h"
#include "widgets.h"
#include "sprites.h"
#include "managers.h"
#include "helpers.h"
#include "renderdata.h"

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
    bool m_flagQuitting;

public:
    // Ctor & Dtor
    SceneContext(Scene *scene) : m_scene(nullptr)
    {
        this->transitionTo(scene);
    }
    ~SceneContext()
    {
        delete m_scene;
    }

    // Scene transition
    void transitionTo(Scene *scene)
    {
        dbgPrint(DPL::DEBUG, fmt::format("Context: Transition to {}.", typeid(*scene).name()));
        if (m_scene != nullptr)
            delete m_scene;
        this->m_scene = scene;
        this->m_scene->setContext(this);
    }

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
    std::shared_ptr<GTexture> uiElementsTexture;
    std::unique_ptr<GTexture> brickBGTexture;
    std::unique_ptr<GTexture> logoTexture;

    // Callbacks
    void cbPlay();
    void cbHelp();
    void cbInfo();

    // Initializer methods
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
    // Sprite groups
    std::vector<std::shared_ptr<Particle>> m_particles;
    std::vector<std::shared_ptr<Hole>> m_holes;
    std::vector<std::shared_ptr<PenaltyText>> m_penaltyTexts;

    // Buttons
    std::unique_ptr<GButton> btnToMenu;

    // UI Stuff
    std::unique_ptr<TimerBar> m_timerBar;
    std::unique_ptr<Scoreboard> m_scoreboard;

    // Textures
    std::shared_ptr<GTexture> m_uiElementsTexture;
    std::unique_ptr<GTexture> m_cityhallBGTexture;
    std::unique_ptr<GTexture> m_auraBGTexture;
    std::shared_ptr<GTexture> m_holeSheetTexture;

    // Fonts
    std::shared_ptr<GFont> m_gFontMedium;

    // Timers
    GTimer m_tmrWarmup;
    Uint32 m_delayWarmup = 3000;
    GTimer m_tmrSpawnParticle;
    Uint32 m_delaySpawnParticle = 1000;
    static const int m_MAX_PARTICLES = 20;
    GTimer m_tmrAntiIdle;
    Uint32 m_delayAntiIdle = 5000;

    // Scores and other stuff
    int m_score = 0;
    ShakeGenerator m_shakeGen;

    // Managers
    std::unique_ptr<HoleManager> m_holeMgr;
    std::unique_ptr<HoleCollisionManager> m_collideMgr;

    // Callbacks
    void cbToMenu();

    // Initializer methods
    void loadAssets();
    void createButtons();
    void initializeTimers();
    void createHoles();
    void initializeHoleMgr();
    void initializeCollisionMgr();
    void initializeTargetTexture();
    void initializeTimerBar();
    void initializeScoreboard();

public:
    PlayScene();
    ~PlayScene() override;

    void handleEvents(SDL_Event *e) override;
    void update() override;
    void draw() override;
};

class DebugScene : public Scene
{
private:
    // Sprite groups
    std::vector<std::shared_ptr<Hole>> m_holes;

    // Textures
    std::shared_ptr<GTexture> m_holeSheetTexture;
    std::unique_ptr<GTexture> m_cityhallBGTexture;
    std::unique_ptr<GTexture> m_targetTexture;

    // Initializers
    void loadAssets();
    void createHoles();

public:
    DebugScene();
    ~DebugScene() override;

    void handleEvents(SDL_Event *e) override;
    void update() override;
    void draw() override;
};

#endif // SCENES_H