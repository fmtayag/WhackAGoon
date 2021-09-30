#ifndef SCENES_H
#define SCENES_H
#pragma once

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include "sprites.h"
#include "widgets.h"

class AbstractScene {
public:
	virtual ~AbstractScene(){ 
		// printf("Base class destroyed.\n"); 
	};
    virtual void handleEvents(SDL_Event* e) = 0;
    virtual void update() = 0;
    virtual void draw(SDL_Renderer* renderer) = 0;
};

/* -------------------------------------------------
 * Scene Context
 * -------------------------------------------------	
 */
 

enum SceneID {
	MENU_SCENE,
	PLAY_SCENE,
	GAMEOVER_SCENE
};

class SceneContext {
public:
    SceneContext(SceneID scene=MENU_SCENE);
	~SceneContext();
    void changeScene(SceneID scene);
	void quit();
	AbstractScene* get_scene() { return pScene; };

    void HandleEvents(SDL_Event* e);
    void Update();
    void Draw(SDL_Renderer* renderer);

private:
    AbstractScene* pScene;
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

    void handleEvents(SDL_Event* e);
    void update();
    void draw(SDL_Renderer* renderer);

private:
	// update() sub-methods
	void u_timer();
	void u_timecheck();
	void u_collision();
	void u_holes();
	
	// draw() sub-methods
	void draw_texts(SDL_Renderer* renderer);
	void draw_holes(SDL_Renderer* renderer);
	void draw_bg(SDL_Renderer* renderer);

    SceneContext* mContext;
    std::string gameOverMessage;
    bool isGameOver;
    bool mMouseClicked;
    SDL_Point mpos;
    int gameTimer;
    int score;
    int towniesHit;
    HoleManager* hManager;
    std::vector<HoleSprite*> holeSprites;
	
	// Game rules
	const int GAME_DUR = SDL_GetTicks() + 5000; // 30000 ticks is 30 seconds
	const int SCR_PENALTY = 3;

};


/* -------------------------------------------------
 * Game over scene
 * -------------------------------------------------
 */

class GameOverScene: public AbstractScene {
public:
	GameOverScene(SceneContext* context);
	~GameOverScene();
	
	void handleEvents(SDL_Event* e);
    void update();
    void draw(SDL_Renderer* renderer);

private:
    SceneContext* mContext;
	bool mMouseClicked;
};


/* -------------------------------------------------
 * Menu scene
 * -------------------------------------------------
 */
 
class MenuScene: public AbstractScene {
public:
	MenuScene(SceneContext* context);
	~MenuScene();
	
	void handleEvents(SDL_Event* e);
    void update();
    void draw(SDL_Renderer* renderer);

private:
	std::vector<Button*> buttons;
	SceneContext* mContext;
	bool mMouseClicked;
 };


#endif // SCENES_H




