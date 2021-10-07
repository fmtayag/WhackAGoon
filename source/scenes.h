#ifndef SCENES_H
#define SCENES_H
#pragma once

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include "sprites.h"
#include "widgets.h"
#include "metadata.h"

enum SceneID {
	MENU_SCENE,
	PLAY_SCENE,
	GAMEOVER_SCENE
};

enum PlaySceneState {
	PS_PAUSED,
	PS_WARMUP,
	PS_RUNNING,
	PS_GAMEOVER
};

class AbstractScene {
public:
	virtual ~AbstractScene() = default;
	
    virtual void handleEvents(SDL_Event* e) = 0;
    virtual void update() = 0;
    virtual void draw(SDL_Renderer* renderer) = 0;
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
 
class MenuScene: public AbstractScene {
public:
	MenuScene(SceneContext* context);
	~MenuScene();

	void handleEvents(SDL_Event* e);
	void update();
	void draw(SDL_Renderer* renderer);

	void chs_playGame();

private:
	std::vector<Button*> buttons;
	SceneContext* mContext;

	// Mouse
	MouseState z_mouse;

};

class PlayScene : public AbstractScene {
public:
    PlayScene(SceneContext* mContext);
    ~PlayScene();

    void handleEvents(SDL_Event* e);
    void update();
    void draw(SDL_Renderer* renderer);

private:
	// update() sub-methods
	void u_wuTimer();
	void u_transgameover();
	void u_timer();
	void u_timecheck();
	void u_collision();
	void u_holes();
	void u_activateDur();
	void u_activateHoles();
	
	// draw() sub-methods
	void draw_texts(SDL_Renderer* renderer);
	void draw_holes(SDL_Renderer* renderer);
	void draw_bg(SDL_Renderer* renderer);
	
	// helper methods
	void mk_holes();
	int pick_holeType();
	void ch_gstate(PlaySceneState n_state);
	
	// For screen shake
	int tmr_shake;
	const int DELAY_SHAKE = 50;
	const int MAX_SHAKE = 5;
	std::vector<int> vShake; // 'generate' -n and n alternately and store into this arr
	void genShake();
	void shake();
	
	// Target texture stuff
	SDL_Rect targRect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
	
	// Context
    SceneContext* mContext;
	
	// Messages
    std::string gOverMsg = "DEBUG";
    
	// Mouse
    bool mMouseClicked = false;
    SDL_Point mpos;
	
	// Holes
	std::vector<HoleSprite*> holeSprites;
	
	// Game state
	PlaySceneState m_gstate = PS_WARMUP;
	
	// Game rules
	const int SCOR_PENALTY = 3;
	const int MAX_GAME_DURATION = 45000; // 1000 ticks is approx 1 sec
	int score = 0;
	
	// Durations and timers
	int dur_game;
	int tmr_game;
	int critTime;
	const double ctPerc = 0.25; 
	
	int tmr_activateHole;
	int dur_activateHole = 3000;

	int tmr_upd_durActv;
	const int DUR_UPD_DURACTV = 2500;
	const int MIN_DURACTV_VAL = 600;
	const int DECREMENT_MIN = 100;
	const int DECREMENT_MAX = 400;
	
	const int DUR_TRANSTOGAMEOVER = 3000;
	int tmr_transtogameover = 0;
	
	const int DUR_WARMUPTIMER = 3500;
	int tmr_warmuptimer = SDL_GetTicks();
	
};

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

#endif // SCENES_H




