#ifndef SCENES_H
#define SCENES_H
#pragma once

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include "sprites.h"

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

    std::string gameOverMessage;
    bool isGameOver;
    bool mMouseClicked;
    int mx, my;
    int mpos[2];
    int gameTimer;
    const int GAME_DUR = SDL_GetTicks() + 30000; // 30000 ticks is 30 seconds
    int score;
    int towniesHit;

    // test
    HoleSprite* hSprite;

    HoleManager* hManager;
    std::vector<HoleSprite*> holeSprites;

};

#endif // SCENES_H




