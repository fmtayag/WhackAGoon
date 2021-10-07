#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <functional>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "scenes.h"
#include "metadata.h"
#include "assets.h"
#include "sprites.h"
#include "utils.h"

//{ SceneContext
SceneContext::SceneContext(SceneID scene)
{	
	switch(scene) {
		case MENU_SCENE:
			this->pScene = new MenuScene(this);
			break;
		case PLAY_SCENE:
			this->pScene = new PlayScene(this);
			break;
		case GAMEOVER_SCENE:
			this->pScene = new GameOverScene(this);
			break;
		default:
			this->pScene = NULL;
			break;
	}
}

SceneContext::~SceneContext() {
	delete this->pScene;
	this->pScene = NULL;
}

void SceneContext::changeScene(SceneID scene)
{
    delete this->pScene;
	this->pScene = NULL;
	
	switch(scene) {
		case MENU_SCENE:
			this->pScene = new MenuScene(this);
			break;
		case PLAY_SCENE:
			this->pScene = new PlayScene(this);
			break;
		case GAMEOVER_SCENE:
			this->pScene = new GameOverScene(this);
			break;
		default:
			this->pScene = NULL;
			break;
	}

}

void SceneContext::quit() {
	delete this->pScene;
	this->pScene = NULL;
}

void SceneContext::HandleEvents(SDL_Event *e)
{
	if(this->pScene != NULL) {
		pScene->handleEvents(e);
	}
    
}

void SceneContext::Update()
{
	if(this->pScene != NULL) {
		pScene->update();
	}
}

void SceneContext::Draw(SDL_Renderer *renderer)
{
	if(this->pScene != NULL) {
		pScene->draw(renderer);
	}
}
//}

//{ MenuScene
MenuScene::MenuScene(SceneContext* context) {
	mContext = context;
	z_mouse.isClicked = false;

	// Buttons
	Button* btn1 = new Button(btnTexture, "TESTING", 96, 32, 100, 100);
	btn1->bindCallback(std::bind(&MenuScene::chs_playGame, this));
	buttons.push_back(btn1);

}
 
MenuScene::~MenuScene() {
	for(Button* btn : buttons) {
		delete btn;
		btn = NULL;
	}
	printf("Deleted menu scene.\n");
}
 
void MenuScene::handleEvents(SDL_Event* e) {
	// Handle events --------------------------------------
	while (SDL_PollEvent(e))
	{
		if (e->type == SDL_QUIT)
		{
			mContext->quit();
		}
		else if (e->type == SDL_MOUSEBUTTONDOWN)
		{
			z_mouse.isClicked = true;
		}
		else if (e->type == SDL_MOUSEBUTTONUP)
		{
			z_mouse.isClicked = false;
		}
	}

	// Update mouse position
	SDL_GetMouseState(&z_mouse.pos.x, &z_mouse.pos.y);

}
 
void MenuScene::update() {
	//  Update buttons
	for(Button* btn : buttons) {
		btn->update(&z_mouse);
	}
}
 
void MenuScene::draw(SDL_Renderer* renderer) {
	// Set render draw color, and clear renderer ----------
	SDL_SetRenderDrawColor(renderer, BG_COLOR.r, BG_COLOR.g, BG_COLOR.b, 255);
	SDL_RenderClear(renderer);

	drawText(renderer, "MENU SCENE", gFont, 0, 0, {255, 255, 255});
	drawText(renderer, "PRESS ENTER TO PLAY", gFont, 0, 64, {255, 255, 255});

	// Draw buttons --------------------------
	for (Button* button : buttons) {
		button->draw(renderer);
	}

	// Render
	SDL_RenderPresent(renderer);
}
 
void MenuScene::chs_playGame() {
	mContext->changeScene(PLAY_SCENE);
}
//}

//{ PlayScene
PlayScene::PlayScene(SceneContext *context)
{
    mContext = context;
	mk_holes();
}

PlayScene::~PlayScene()
{
	for (HoleSprite* hole : holeSprites)
    {
        delete hole;
    }
	holeSprites.clear();
	printf("Deleted play scene.\n");
}

void PlayScene::handleEvents(SDL_Event *e)
{
    // Handle events --------------------------------------
    while (SDL_PollEvent(e))
    {
        if (e->type == SDL_QUIT)
        {
            mContext->quit();
        }
        else if (e->type == SDL_MOUSEBUTTONDOWN)
        {
            mMouseClicked = true;
        }
        else if (e->type == SDL_MOUSEBUTTONUP)
        {
            mMouseClicked = false;
        }
		else if (e->type == SDL_KEYDOWN) {
			switch(e->key.keysym.sym) {
			case SDLK_ESCAPE:
				mContext->changeScene(MENU_SCENE);
				break;
			case SDLK_RETURN:
				// FOR DEBUGGING ONLY!!
				ch_gstate(PS_GAMEOVER);
				break;
			default:
				break;
			}
		}
    }

    // Update mouse position
    SDL_GetMouseState(&mpos.x, &mpos.y);
}

void PlayScene::update()
{	
	switch(m_gstate) {
		case PS_PAUSED:
			break;
		case PS_WARMUP:
			u_wuTimer();
			break;
		case PS_GAMEOVER:
			u_transgameover();
			break;
		case PS_RUNNING:
			//u_timer();
			u_timecheck();
			u_collision();
			u_holes();
			u_activateDur();
			u_activateHoles();
			break;
		default:
			printf("Warning: m_gstate reached default case.\n");
			break;
	}
		
}

void PlayScene::u_wuTimer() {
	// Update warm up timer
	int now = SDL_GetTicks();
	bool timesUp = now - tmr_warmuptimer > DUR_WARMUPTIMER;
	
	if(timesUp) {
		ch_gstate(PS_RUNNING);
	}
}

void PlayScene::u_transgameover() {
	// Init TMR for game over transition 
	if(tmr_transtogameover == 0) {
		tmr_transtogameover = SDL_GetTicks();
	}

	int now = SDL_GetTicks();
	bool timesUp = now - tmr_transtogameover > DUR_TRANSTOGAMEOVER;
	
	if(timesUp) {
		mContext->changeScene(GAMEOVER_SCENE);
	}
}

void PlayScene::u_timer() {
	tmr_game = SDL_GetTicks();
}

void PlayScene::u_timecheck() {
	int now = SDL_GetTicks();
	bool timesUp = now - tmr_game > dur_game;
	if(timesUp) {
		ch_gstate(PS_GAMEOVER);
		gOverMsg = "TIME'S UP!";
	}
}

void PlayScene::u_collision() {
	
    for (HoleSprite* hole : holeSprites)
    {
        bool collided = isPointCollide(mpos, hole->getRect());
        if (collided && mMouseClicked)
        {
            bool isWhacked = hole->whack();
            mMouseClicked = false;

            if (isWhacked)
            {
				switch(hole->getType()) {
					case HT_Goon:
						score++;
						break;
					case HT_Townie:
						score -= SCOR_PENALTY;
						break;
					case HT_Mayor:
						ch_gstate(PS_GAMEOVER);
						gOverMsg = "YOU HIT THE MAYOR!";
						break;
					default:
						printf("Warning: default case has been reached for collision check.\n");
						break;
				}
            }

            // neg score check
            if (score < 0)
                score = 0;
        }
    }
	
}

void PlayScene::u_holes() {
    for (HoleSprite* hole : holeSprites)
    {
        hole->update();
    }
}

void PlayScene::u_activateDur() {
	// Update dur_activateHole as time goes on.
	int now = SDL_GetTicks();
	bool timesUp = now - tmr_upd_durActv > DUR_UPD_DURACTV;
	bool aboveMin = dur_activateHole > MIN_DURACTV_VAL;
	
	if(timesUp && aboveMin) {
		tmr_upd_durActv = now;
		
		dur_activateHole -= rand() % DECREMENT_MIN + DECREMENT_MAX;
		printf("Debug: dur_activateHole is %d.\n", dur_activateHole);
	}
}

void PlayScene::u_activateHoles() {
	int now = SDL_GetTicks();
	bool timesUp = now - tmr_activateHole > dur_activateHole;
	
	if(timesUp) {
		tmr_activateHole = now;
		for(HoleSprite* hole : holeSprites) {
			
			bool selectThis = rand() & 1; // choose either 0 or 1
			bool holeIsResting = hole->getAnimState() == AS_Resting;
			
			if(selectThis && holeIsResting) {
				// Awake hole
				HoleType ht = (HoleType) pick_holeType();
				hole->awake(ht);
				break;
			}
			
		}
	}
}

void PlayScene::draw(SDL_Renderer *renderer) {
	// Colors
	SDL_Color BLACK = {0, 0, 0, 255};
	SDL_Color WHITE = {255, 255, 255, 255};
	
    SDL_SetRenderDrawColor(renderer, BG_COLOR.r, BG_COLOR.g, BG_COLOR.b, BG_COLOR.a);
    SDL_RenderClear(renderer);
	
	draw_bg(renderer);
	draw_holes(renderer);
	draw_texts(renderer);

    SDL_RenderPresent(renderer);
}

void PlayScene::draw_texts(SDL_Renderer* renderer) {
	// Colors
	SDL_Color WHITE = {255, 255, 255, 255};
	SDL_Color RED = {255, 0, 0, 255};
	
	// Anchor points, offsets
	const int winCenterW = WINDOW_WIDTH / 2;
	const int winCenterH = WINDOW_HEIGHT / 2;
    const int offsetX = 16;

	if(m_gstate == PS_RUNNING) {
		// Messages
		int now = SDL_GetTicks();
		int timeLeft = (dur_game - (now - tmr_game)) / 1000;
		std::string timeMessage = std::to_string(timeLeft) + "s";
		std::string scoreMessage = std::to_string(score);
		
		// Time message color
		SDL_Color timeColor = WHITE;
		bool underCritTime = (dur_game - (now - tmr_game)) <= critTime;
		if(underCritTime) { timeColor = RED; }
		
		// Draw texts
		drawText(renderer, "SCORE", gFont, winCenterW - 64, 42, WHITE, true);
		drawText(renderer, scoreMessage.c_str(), gFont, winCenterW - 64, 64, WHITE, true);
		drawText(renderer, "TIME", gFont, winCenterW + 64, 42, timeColor, true);
		drawText(renderer, timeMessage.c_str(), gFont, winCenterW + 64, 64, timeColor, true);
	}
	else if(m_gstate == PS_WARMUP) {
		int now = SDL_GetTicks();
		int warmupTimeLeft = (DUR_WARMUPTIMER - (now - tmr_warmuptimer)) / 1000;
		std::string warmupTimeMsg = std::to_string(warmupTimeLeft);
		
		drawText(renderer, "GET READY", gFontL, winCenterW, 22, WHITE, true);
		drawText(renderer, warmupTimeMsg.c_str(), gFontL, winCenterW , 58, WHITE, true);
	}
	else if(m_gstate == PS_GAMEOVER) {
		drawText(renderer, "GAME OVER!", gFontL, winCenterW, 28, WHITE, true);
		drawText(renderer, gOverMsg.c_str(), gFontM, winCenterW, 64, WHITE, true);
	}
}

void PlayScene::draw_holes(SDL_Renderer* renderer) {
    for (HoleSprite* hole : holeSprites)
    {
        hole->draw(renderer);
    }
}

void PlayScene::draw_bg(SDL_Renderer* renderer) {
	SDL_RenderCopy(renderer, bgTexture, NULL, NULL);
}

void PlayScene::mk_holes() {
	/*** Create the holes ***/
	
	// Anchor points, offsets
    const int centerW = (WINDOW_WIDTH / 2) - (HOLE_WIDTH / 2);
    const int centerH = (WINDOW_HEIGHT / 2) - (HOLE_HEIGHT / 2);
    const int offsetW = 48;
    const int offsetH = 32;
	
    // Column 1
    holeSprites.push_back(new HoleSprite(spritesTexture, centerW - offsetW, centerH - 72 + offsetH));
    holeSprites.push_back(new HoleSprite(spritesTexture, centerW - offsetW - 32, centerH + offsetH));
    holeSprites.push_back(new HoleSprite(spritesTexture, centerW - offsetW, centerH + 72 + offsetH));

    // Column 2
    holeSprites.push_back(new HoleSprite(spritesTexture, centerW + offsetW, centerH - 72 + offsetH));
    holeSprites.push_back(new HoleSprite(spritesTexture, centerW + offsetW + 32, centerH + offsetH));
    holeSprites.push_back(new HoleSprite(spritesTexture, centerW + offsetW, centerH + 72 + offsetH));
}

int PlayScene::pick_holeType() {
	// Pick hole type
	std::vector<HoleType> bag;
	int wgt_goon = 85;
	int wgt_town = 10;
	int wgt_mayr = 5;
	
	for(int i=0; i < wgt_goon; i++) {
		bag.push_back(HT_Goon);
	}
	for(int i=0; i < wgt_town; i++) {
		bag.push_back(HT_Townie);
	}
	for(int i=0; i < wgt_mayr; i++) {
		bag.push_back(HT_Mayor);
	}
	
	std::random_shuffle(bag.begin(), bag.end());
	int pick = rand() % bag.size() + 0;
	
	return bag[pick];
}

void PlayScene::ch_gstate(PlaySceneState n_state) {
	// Change state and apply changes based on past state and new state
	if(m_gstate == PS_WARMUP && n_state == PS_RUNNING) {
		m_gstate = n_state;
		
		// Update timers
		int now = SDL_GetTicks();
		tmr_activateHole = now;
		tmr_game = now;
		tmr_upd_durActv = now;
		dur_game = MAX_GAME_DURATION;
		critTime = dur_game * ctPerc;
	}
	else {
		m_gstate = n_state;
	}
}
//}

//{ GameOverScene
GameOverScene::GameOverScene(SceneContext* context) {
	mContext = context;
	mMouseClicked = false;
}
 
GameOverScene::~GameOverScene() {
	printf("Deleted game over scene.\n");
}
 
void GameOverScene::handleEvents(SDL_Event* e) {
	// Handle events --------------------------------------
	while (SDL_PollEvent(e))
	{
		if (e->type == SDL_QUIT)
		{
			mContext->quit();
		}
		else if (e->type == SDL_MOUSEBUTTONDOWN)
		{
			mMouseClicked = true;
		}
		else if (e->type == SDL_MOUSEBUTTONUP)
		{
			mMouseClicked = false;
		}
		
		else if(e->type == SDL_KEYDOWN) {
			switch(e->key.keysym.sym) {
			case SDLK_RETURN:
				mContext->changeScene(MENU_SCENE);
				break;
			default:
				break;
			}
		}
	}

}
 
void GameOverScene::update() {
	
}

void GameOverScene::draw(SDL_Renderer* renderer) {
	// Set render draw color, and clear renderer ----------
	SDL_SetRenderDrawColor(renderer, BG_COLOR.r, BG_COLOR.g, BG_COLOR.b, 255);
	SDL_RenderClear(renderer);

	drawText(renderer, "GAME OVER", gFont, 0, 0, {255, 255, 255});
	drawText(renderer, "PRESS ENTER TO GO TO MENU", gFont, 0, 64, {255, 255, 255});

	// Render 
	SDL_RenderPresent(renderer);
}
//}
 
 
