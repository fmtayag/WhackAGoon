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

/* -------------------------------------------------
 * Scene Context
 * -------------------------------------------------
 */

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

/* -------------------------------------------------
 * Play Scene
 * -------------------------------------------------
 */

class PlayScene : public AbstractScene {
public:
    PlayScene(SceneContext* mContext);
    ~PlayScene();

    void handleEvents(SDL_Event* e, bool& isRunning);
    void update();
    void draw(SDL_Renderer* renderer);

private:
    SceneContext* mContext;
    bool mMouseClicked;

    // Textures list
    SDL_Texture** mHoleTextures[3];

    // Entities
    std::vector<HoleEntity*> mEntities;
    HammerEntity* mHammer;

    // Scores, and crap
    int score;
};

#endif // SCENES_H




