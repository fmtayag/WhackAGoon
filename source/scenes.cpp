#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <functional>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "scenes.h"
#include "metadata.h"
#include "assets.h"
#include "sprites.h"
#include "utils.h"
#include "widgets.h"

#pragma region SceneContext
//{ SceneContext
SceneContext::SceneContext(SceneID scene)
{
	sceneSwitch(scene);
	this->pScene->setContext(this);
}

SceneContext::~SceneContext()
{
	delete this->pScene;
	this->pScene = NULL;
}

void SceneContext::changeScene(SceneID scene)
{
	printf("Hello");
	delete this->pScene;
	this->pScene = NULL;

	sceneSwitch(scene);
}

void SceneContext::quit()
{
	delete this->pScene;
	this->pScene = NULL;
}

void SceneContext::HandleEvents(SDL_Event *e)
{
	if (this->pScene != NULL)
	{
		pScene->handleEvents(e);
	}
}

void SceneContext::Update()
{
	if (this->pScene != NULL)
	{
		pScene->update();
	}
}

void SceneContext::Draw(SDL_Renderer *renderer)
{
	if (this->pScene != NULL)
	{
		pScene->draw(renderer);
	}
}

void SceneContext::sceneSwitch(SceneID scene)
{
	switch (scene)
	{
	case MENU_SCENE:
		this->pScene = new MenuScene(this);
		break;
	// case PLAY_SCENE:
	// 	this->pScene = new PlayScene(this);
	// 	break;
	// case GAMEOVER_SCENE:
	// 	this->pScene = new GameOverScene(this);
	// 	break;
	default:
		this->pScene = NULL;
		printf("DEBUG: Reached default cause for scene switch.\n");
		break;
	}
}

//}
#pragma endregion SceneContext

#pragma region MenuScene
//{ MenuScene
MenuScene::MenuScene(SceneContext *context)
{
	initialize();
}

MenuScene::~MenuScene()
{
	destroy();
}

void MenuScene::initialize()
{
	createButtons();
}

void MenuScene::destroy()
{
	// Delete buttons
	for (auto btn : m_buttons)
		delete btn;
	m_buttons.clear();
}

void MenuScene::handleEvents(SDL_Event *e)
{
	// Handle events --------------------------------------
	while (SDL_PollEvent(e))
	{
		if (e->type == SDL_QUIT)
		{
			m_context->quit();
		}
		else if (e->type == SDL_MOUSEBUTTONDOWN)
		{
			z_mouse.isClicked = true;
		}
		else if (e->type == SDL_MOUSEBUTTONUP)
		{
			z_mouse.isClicked = false;
		}
		else if (e->type == SDL_KEYDOWN)
		{
			switch (e->key.keysym.sym)
			{
			case SDLK_BACKQUOTE:
				m_context->changeScene(DEBUG_SCENE);
			}
		}
	}

	// Update mouse position
	SDL_GetMouseState(&z_mouse.pos.x, &z_mouse.pos.y);
}

void MenuScene::update()
{
	//  Update buttons
	for (Button(*button) : m_buttons)
	{
		button->update(&z_mouse);
	}
}

void MenuScene::draw(SDL_Renderer *renderer)
{
	// Set render draw color, and clear renderer ----------
	SDL_SetRenderDrawColor(renderer, BG_COLOR.r, BG_COLOR.g, BG_COLOR.b, 255);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, menuBgTexture, NULL, NULL);

	std::string txt_menuscene = "MENU SCENE";
	std::string txt_copyright = "(c) 2021 Zyenapz";
	const int logoWidth = 80 * PXSCALE;
	const int logoHeight = 50 * PXSCALE;
	SDL_Rect logoRect = {(WINDOW_WIDTH / 2) - (logoWidth / 2), PXSCALE * 8, logoWidth, logoHeight};

	SDL_RenderCopy(renderer, logoTexture, NULL, &logoRect);

	//drawText(renderer, txt_menuscene, gFontL, 0, 0, {255, 255, 255});
	//drawText(renderer, "PRESS ENTER TO PLAY", gFontL, 0, 64, {255, 255, 255});
	//drawText(renderer, "Game v1.0", gFontS, WINDOW_WIDTH / 2, (int)(WINDOW_HEIGHT * 0.88), {255, 255, 255}, true);
	drawText(renderer, txt_copyright, gFontS, WINDOW_WIDTH / 2, (int)(WINDOW_HEIGHT * 0.90), {255, 255, 255}, true);
	//drawText(renderer, "Codelic, GPL-3.0. Artlic CC BY-NC 4.0.", gFontS, WINDOW_WIDTH / 2, (int)(WINDOW_HEIGHT * 0.92), {255, 255, 255}, true);

	// Draw buttons --------------------------
	for (Button(*button) : m_buttons)
	{
		button->draw(renderer);
	}

	// Render
	SDL_RenderPresent(renderer);
}

void MenuScene::cb_playGame()
{
	m_context->changeScene(PLAY_SCENE);
}

void MenuScene::cb_help()
{
	printf("cb_help()\n");
}

void MenuScene::cb_info()
{
	printf("cb_info()\n");
}

void MenuScene::createButtons()
{
	// Anchor points
	const int wcx = WINDOW_WIDTH / 2;
	const int wcy = WINDOW_HEIGHT / 2;
	const int wbtom = WINDOW_HEIGHT;
	const int btnW = 16;
	const int btnH = 16;
	const int bcx = btnW / 2;
	const int x_offset = 25;

	// btnPlay
	std::map<BtnState, SDL_Rect> btnPlay_clips;
	btnPlay_clips[BST_NORMAL] = {0, 24, btnW, btnH};
	btnPlay_clips[BST_HOVERED] = {16, 24, btnW, btnH};

	Button *btnPlay = new Button(uiTexture, {wcx - bcx - x_offset, 256, 0, 0}, btnPlay_clips);
	btnPlay->bindCallback(std::bind(&MenuScene::cb_playGame, this));
	m_buttons.push_back(btnPlay);

	// btnHelp
	std::map<BtnState, SDL_Rect> btnHelp_clips;
	btnHelp_clips[BST_NORMAL] = {32, 24, btnW, btnH};
	btnHelp_clips[BST_HOVERED] = {48, 24, btnW, btnH};

	Button *btnHelp = new Button(uiTexture, {(wcx - bcx - x_offset) - 80, 256, 0, 0}, btnHelp_clips);
	btnHelp->bindCallback(std::bind(&MenuScene::cb_help, this));
	m_buttons.push_back(btnHelp);

	// btnInfo
	std::map<BtnState, SDL_Rect> btnInfo_clips;
	btnInfo_clips[BST_NORMAL] = {0, 56, btnW, btnH};
	btnInfo_clips[BST_HOVERED] = {16, 56, btnW, btnH};

	Button *btnInfo = new Button(uiTexture, {(wcx - bcx - x_offset) + 80, 256, 0, 0}, btnInfo_clips);
	btnInfo->bindCallback(std::bind(&MenuScene::cb_info, this));
	m_buttons.push_back(btnInfo);

	printf("m_buttons size: %d\n", m_buttons.size());
	printf("m_buttons capacity: %d\n", m_buttons.capacity());
}

//}
#pragma endregion MenuScene

// #pragma region PlayScene
// //{ PlayScene
// PlayScene::PlayScene(SceneContext *context) : scoreIcon(uiTexture, {PXSCALE, PXSCALE, 32, 32}, 500),
// 											  opinionIcon(uiTexture, {PXSCALE, PXSCALE * 8, 32, 32}, 500)
// {
// 	m_context = context;
// 	mk_holes();
// 	mk_buttons();
// 	init_uistuff();
// }

// PlayScene::~PlayScene()
// {
// 	for (auto hole : holeSprites)
// 		delete hole;
// 	holeSprites.clear();

// 	for (auto btn : m_buttons)
// 		delete btn;
// 	m_buttons.clear();

// 	for (auto p : m_particles)
// 		delete p;
// 	m_particles.clear();

// 	m_fadetexts.clear();

// 	printf("Deleted play scene.\n");
// }

// void PlayScene::handleEvents(SDL_Event *e)
// {
// 	// Handle events --------------------------------------
// 	while (SDL_PollEvent(e))
// 	{
// 		if (e->type == SDL_QUIT)
// 		{
// 			m_context->quit();
// 		}
// 		else if (e->type == SDL_MOUSEBUTTONDOWN)
// 		{
// 			z_mouse.isClicked = true;
// 		}
// 		else if (e->type == SDL_MOUSEBUTTONUP)
// 		{
// 			z_mouse.isClicked = false;
// 		}
// 		else if (e->type == SDL_KEYDOWN)
// 		{
// 			// FOR DEBUGGING ONLY!!
// 			switch (e->key.keysym.sym)
// 			{
// 			case SDLK_e:
// 				ch_gstate(PS_GAMEOVER);
// 				break;
// 			case SDLK_BACKQUOTE:
// 				m_context->changeScene(MENU_SCENE);
// 				break;
// 			case SDLK_ESCAPE:
// 				m_context->changeScene(MENU_SCENE);
// 				break;
// 			case SDLK_RETURN:
// 				ch_gstate(PS_GAMEOVER);
// 				break;
// 			case SDLK_LEFT:
// 				targRect.x -= 5;
// 				break;
// 			case SDLK_RIGHT:
// 				targRect.x += 5;
// 				break;
// 			case SDLK_s:
// 				genShake();
// 				break;
// 			case SDLK_f:
// 				spawnFadeText();
// 				break;
// 			default:
// 				break;
// 			}
// 		}
// 	}

// 	// Update mouse position
// 	SDL_GetMouseState(&z_mouse.pos.x, &z_mouse.pos.y);
// }

// void PlayScene::update()
// {
// 	switch (m_gstate)
// 	{
// 	case PS_PAUSED:
// 		break;
// 	case PS_WARMUP:
// 		u_wuTimer();
// 		u_buttons();
// 		shake();
// 		break;
// 	case PS_GAMEOVER:
// 		u_initTransitionTimer();
// 		u_transgameover();
// 		u_fadetexts();
// 		//u_holes();
// 		//u_spawnPrt();
// 		//u_prt();
// 		//u_uistuff();
// 		shake();
// 		break;
// 	case PS_RUNNING:
// 		u_checkDeathTimer();
// 		u_checkopinion();
// 		u_collision();
// 		u_holes();
// 		u_activateDur();
// 		u_activateHoles();
// 		u_spawnPrt();
// 		u_prt();
// 		u_uistuff();
// 		u_fadetexts();
// 		u_buttons();
// 		shake();
// 		break;
// 	default:
// 		printf("Warning: m_gstate reached default case.\n");
// 		break;
// 	}
// }

// void PlayScene::u_checkDeathTimer()
// {
// 	bool timerInitialized = tmr_deathCdown != 0;
// 	if (timerInitialized)
// 	{
// 		int now = SDL_GetTicks();
// 		int timeLeft = now - tmr_deathCdown;
// 		bool tUp = timeLeft > dur_deathCdown; // is time up?

// 		const int troThreshold = 1000;
// 		bool trOut = (dur_deathCdown - timeLeft) <= troThreshold; // is time running out?
// 		bool fsAllowed = now - tmr_fspawning > DUR_FSPAWNING;

// 		//printf("now - tmr: %d, dur: %d\n", now - tmr_deathCdown, dur_deathCdown);

// 		if (trOut && fsAllowed)
// 		{
// 			tmr_fspawning = now;

// 			// force spawn a goon type hole
// 			u_activateHoles(true, HT_Goon);
// 			printf("Force spawning a goon!\n");
// 		}

// 		if (tUp)
// 		{
// 			ch_gstate(PS_GAMEOVER);
// 			gOverMsg = "SMASH, YA LAZY BUM!";
// 		}
// 	}
// }

// void PlayScene::u_checkopinion()
// {
// 	if (opinion <= 0)
// 	{
// 		ch_gstate(PS_GAMEOVER);
// 		gOverMsg = "YOU'RE FIRED!";
// 	}
// }

// void PlayScene::u_wuTimer()
// {
// 	// Update warm up timer
// 	int now = SDL_GetTicks();
// 	bool timesUp = now - tmr_warmuptimer > DUR_WARMUPTIMER;

// 	if (timesUp)
// 	{
// 		ch_gstate(PS_RUNNING);
// 	}
// }

// void PlayScene::u_initTransitionTimer()
// {
// 	// init timer
// 	if (tmr_transtogameover == 0)
// 		tmr_transtogameover = SDL_GetTicks();
// }

// void PlayScene::u_transgameover()
// {

// 	int now = SDL_GetTicks();
// 	bool timesUp = now - tmr_transtogameover > DUR_TRANSTOGAMEOVER;

// 	if (timesUp)
// 	{
// 		m_context->gInfo.score = score;
// 		m_context->changeScene(GAMEOVER_SCENE); // should be placed as last instruction!
// 	}
// }

// void PlayScene::u_collision()
// {

// 	for (HoleSprite(*hole) : holeSprites)
// 	{
// 		bool collided = isPointCollide(z_mouse.pos, hole->getRect());
// 		if (collided && z_mouse.isClicked)
// 		{
// 			bool isWhacked = hole->whack();
// 			z_mouse.isClicked = false;

// 			if (isWhacked)
// 			{
// 				switch (hole->getType())
// 				{
// 				case HT_Goon:
// 					score++;
// 					delayDeathCdown();
// 					initDthCdown();
// 					break;
// 				case HT_Townie:
// 					score -= SCOR_PENALTY;
// 					opinion -= 1;
// 					genShake();
// 					decrDthCdownDur();
// 					spawnFadeText();
// 					break;
// 				case HT_Mayor:
// 					ch_gstate(PS_GAMEOVER);
// 					gOverMsg = "YOU HIT THE MAYOR!";
// 					genShake();
// 					break;
// 				default:
// 					printf("Warning: default case has been reached for collision check.\n");
// 					break;
// 				}

// 				// neg score check
// 				if (score < 0)
// 					score = 0;
// 			}
// 		}
// 	}
// }

// void PlayScene::u_holes()
// {
// 	for (HoleSprite(*hole) : holeSprites)
// 	{
// 		hole->update();
// 	}
// }

// void PlayScene::u_activateDur()
// {
// 	// Update dur_activateHole as time goes on.
// 	int now = SDL_GetTicks();
// 	bool timesUp = now - tmr_progression > DUR_PROGRESSION;
// 	bool aboveMin = dur_activateHole > MIN_DURACTV_VAL;

// 	if (timesUp && aboveMin)
// 	{
// 		tmr_progression = now;

// 		dur_activateHole -= rand() % DECREMENT_MIN + DECREMENT_MAX;
// 		if (dur_activateHole <= MIN_DURACTV_VAL)
// 		{
// 			dur_activateHole = MIN_DURACTV_VAL;
// 		}
// 		printf("Debug: dur_activateHole is %d.\n", dur_activateHole);
// 	}
// }

// void PlayScene::u_activateHoles(bool isForced, HoleType forcedType)
// {
// 	int now = SDL_GetTicks();
// 	bool timesUp = now - tmr_activateHole > dur_activateHole;

// 	if (timesUp || isForced)
// 	{
// 		tmr_activateHole = now;
// 		for (HoleSprite(*hole) : holeSprites)
// 		{

// 			bool selectThis = rand() & 1; // choose either 0 or 1
// 			bool holeIsResting = hole->getAnimState() == AS_Resting;

// 			if (selectThis && holeIsResting)
// 			{
// 				// Awake hole
// 				HoleType ht = HT_None;

// 				if (forcedType != HT_None)
// 					ht = forcedType;
// 				else
// 					ht = (HoleType)pick_holeType();

// 				hole->awake(ht);
// 				break;
// 			}
// 		}
// 	}
// }

// void PlayScene::u_spawnPrt()
// {
// 	int now = SDL_GetTicks();
// 	bool timesUp = now - tmr_prtspawn > DUR_PRTSPAWN;

// 	if (timesUp)
// 	{
// 		tmr_prtspawn = now;

// 		// spawn particle
// 		if (m_particles.size() < 10)
// 		{
// 			SDL_Rect prtRect;
// 			prtRect.x = rand() % WINDOW_WIDTH + 1;
// 			prtRect.y = rand() % 132 + 80;
// 			prtRect.w = PXSCALE;
// 			prtRect.h = PXSCALE;

// 			printf("X: %d, Y: %d\n", prtRect.x, prtRect.y);

// 			GameColors gColors;
// 			SDL_Color prtCol = gColors.DARKGREEN; // toxic green
// 			SDL_Point prtVel = {0, -1};

// 			m_particles.push_back(new Particle(prtRect, prtCol, prtVel));
// 		}
// 	}
// }

// void PlayScene::u_prt()
// {
// 	//printf("m_particles size: %d\n", m_particles.size());
// 	// update particles
// 	auto iter = m_particles.begin();
// 	for (Particle(*p) : m_particles)
// 	{
// 		if (p->isVisible())
// 		{
// 			p->update();
// 		}
// 		else
// 		{
// 			m_particles.erase(iter);
// 		}
// 		iter++;
// 	}
// }

// void PlayScene::u_uistuff()
// {
// 	scoreIcon.update();

// 	// opinion icon stuff
// 	opinionIcon.update();
// 	Clips_OpinionIcon opin_clips;
// 	switch (opinion)
// 	{
// 	case 3:
// 		opinionIcon.set_clip(opin_clips.HIGH);
// 		break;
// 	case 2:
// 		opinionIcon.set_clip(opin_clips.MED);
// 		break;
// 	case 1:
// 		opinionIcon.set_clip(opin_clips.LOW);
// 		break;
// 	default:
// 		opinionIcon.set_clip(opin_clips.LOW);
// 		break;
// 	}
// }

// void PlayScene::u_fadetexts()
// {
// 	auto iter = m_fadetexts.begin();
// 	for (FadeText &fade : m_fadetexts)
// 	{
// 		if (fade.isVisible())
// 		{
// 			fade.update();
// 		}
// 		else
// 		{
// 			m_fadetexts.erase(iter);
// 		}
// 		iter++;
// 	}
// }

// void PlayScene::u_buttons()
// {
// 	for (Button(*btn) : m_buttons)
// 	{
// 		btn->update(&z_mouse);
// 	}
// }

// void PlayScene::draw(SDL_Renderer *renderer)
// {
// 	// Colors
// 	SDL_Color BLACK = {0, 0, 0, 255};
// 	SDL_Color WHITE = {255, 255, 255, 255};

// 	SDL_SetRenderDrawColor(renderer, BG_COLOR.r, BG_COLOR.g, BG_COLOR.b, BG_COLOR.a);
// 	SDL_RenderClear(renderer);

// 	SDL_Texture *targetTexture = SDL_CreateTexture(
// 		renderer,
// 		SDL_PIXELFORMAT_RGBA8888,
// 		SDL_TEXTUREACCESS_TARGET,
// 		targRect.w,
// 		targRect.h);

// 	SDL_SetRenderTarget(renderer, targetTexture);

// 	draw_bg(renderer);
// 	draw_prt(renderer, targetTexture);
// 	draw_city(renderer);
// 	draw_holes(renderer);
// 	draw_texts(renderer);
// 	draw_deathTimer(renderer);
// 	draw_uistuff(renderer);
// 	draw_fadetexts(renderer);
// 	draw_buttons(renderer);

// 	SDL_SetRenderTarget(renderer, NULL);
// 	SDL_RenderCopy(renderer, targetTexture, NULL, &targRect);
// 	SDL_RenderPresent(renderer);

// 	SDL_DestroyTexture(targetTexture);
// }

// void PlayScene::draw_texts(SDL_Renderer *renderer)
// {
// 	// Colors
// 	SDL_Color WHITE = {255, 255, 255, 255};
// 	SDL_Color RED = {255, 0, 0, 255};

// 	// Anchor points, offsets
// 	const int winCenterW = WINDOW_WIDTH / 2;
// 	const int winCenterH = WINDOW_HEIGHT / 2;
// 	const int offsetX = 16;

// 	// Messages
// 	std::string scoreMessage = std::to_string(score);

// 	// Draw Score
// 	int scormsg_x = (scoreIcon.get_rect().x + scoreIcon.get_rect().w) + 4;
// 	int scormsg_y = scoreIcon.get_rect().y + (scoreIcon.get_rect().h / 2);
// 	drawText(renderer, scoreMessage.c_str(), gFont, scormsg_x, scormsg_y, WHITE, false, true);

// 	// Draw Timer Label
// 	SDL_Color timerLblColor;
// 	GameColors colorList;
// 	if (tmr_deathCdown == 0)
// 		timerLblColor = colorList.DARKGREEN;
// 	else
// 		timerLblColor = colorList.WHITE;
// 	drawText(renderer, "TIMER", gFont, winCenterW, 32, timerLblColor, true);

// 	if (m_gstate == PS_WARMUP)
// 	{
// 		int now = SDL_GetTicks();
// 		int warmupTimeLeft = (DUR_WARMUPTIMER - (now - tmr_warmuptimer)) / 1000;
// 		std::string warmupTimeMsg = std::to_string(warmupTimeLeft).c_str();

// 		drawText(renderer, "GET READY", gFontL, winCenterW, winCenterH, WHITE, true);
// 		drawText(renderer, warmupTimeMsg, gFontL, winCenterW, winCenterH + 32, WHITE, true);
// 	}
// 	else if (m_gstate == PS_GAMEOVER)
// 	{
// 		drawText(renderer, "GAME OVER!", gFontL, winCenterW, winCenterH, WHITE, true);
// 		drawText(renderer, gOverMsg.c_str(), gFont, winCenterW, winCenterH + 32, WHITE, true);
// 	}
// }

// void PlayScene::draw_holes(SDL_Renderer *renderer)
// {
// 	for (HoleSprite(*hole) : holeSprites)
// 	{
// 		hole->draw(renderer);
// 	}
// }

// void PlayScene::draw_bg(SDL_Renderer *renderer)
// {
// 	SDL_RenderCopy(renderer, bgTexture, NULL, NULL);
// }

// void PlayScene::draw_city(SDL_Renderer *renderer)
// {
// 	SDL_RenderCopy(renderer, cityTexture, NULL, NULL);
// }

// void PlayScene::draw_deathTimer(SDL_Renderer *renderer)
// {
// 	int now;
// 	if (m_gstate == PS_RUNNING)
// 	{
// 		now = SDL_GetTicks();
// 		old_frame = now;
// 	}
// 	else if (m_gstate == PS_GAMEOVER)
// 		now = old_frame; // used to stop the bar from shrinking when it's already game over

// 	int tmr = tmr_deathCdown;
// 	double dur = dur_deathCdown;

// 	// Normalize time left
// 	float valTL = now - tmr;
// 	float minTL = 0;
// 	float maxTL = dur_deathCdown;
// 	float normTL = (valTL - minTL) / (maxTL - minTL);

// 	// Set bar color
// 	GameColors colors;
// 	SDL_Color barColor = colors.WHITE;
// 	if (normTL >= 0.30)
// 		barColor = colors.ORANGE;
// 	if (normTL >= 0.60)
// 		barColor = colors.RED;

// 	// Normalize death countdown duration
// 	float valDD = dur_deathCdown;
// 	float minDD = 0;
// 	float maxDD = MAX_DUR_DEATHCDOWN;
// 	float normDD = (valDD - minDD) / (maxDD - minDD);

// 	// Set bar width
// 	float y = 192 * normDD;
// 	//printf("y: %f.\n", y);
// 	float bar_width = y - (y * normTL);
// 	if (bar_width <= 0)
// 	{
// 		bar_width = 0;
// 	}

// 	if (tmr_deathCdown == 0)
// 	{
// 		bar_width = y;
// 		barColor = colors.DARKGREEN;
// 	}

// 	short int wcW = WINDOW_WIDTH / 2;
// 	short int bwc = bar_width / 2;
// 	short int x = wcW - bwc;

// 	SDL_Rect dtbarRect;
// 	dtbarRect.x = x;
// 	dtbarRect.y = 16;
// 	dtbarRect.w = (int)bar_width;
// 	dtbarRect.h = 8;

// 	SDL_SetRenderDrawColor(renderer, barColor.r, barColor.g, barColor.b, barColor.a);
// 	SDL_RenderFillRect(renderer, &dtbarRect);
// }

// void PlayScene::draw_prt(SDL_Renderer *renderer, SDL_Texture *parentTargTexture)
// {
// 	for (Particle(*p) : m_particles)
// 	{
// 		p->bindParentTargTexture(parentTargTexture);
// 		p->draw(renderer);
// 	}
// }

// void PlayScene::draw_uistuff(SDL_Renderer *renderer)
// {
// 	scoreIcon.draw(renderer);
// 	opinionIcon.draw(renderer);
// }

// void PlayScene::draw_fadetexts(SDL_Renderer *renderer)
// {
// 	for (FadeText &fade : m_fadetexts)
// 	{
// 		fade.draw(renderer);
// 	}
// }

// void PlayScene::draw_buttons(SDL_Renderer *renderer)
// {
// 	for (Button(*btn) : m_buttons)
// 	{
// 		btn->draw(renderer);
// 	}
// }

// void PlayScene::mk_holes()
// {
// 	/*** Create the holes ***/

// 	// Anchor points, offsets
// 	const int centerW = (WINDOW_WIDTH / 2) - (HOLE_WIDTH / 2);
// 	const int centerH = (WINDOW_HEIGHT / 2) - (HOLE_HEIGHT / 2);
// 	const int offsetW = 80;
// 	const int offsetH = 80;

// 	// Column 1
// 	holeSprites.push_back(new HoleSprite(spritesTexture, centerW - offsetW, centerH - 72 + offsetH));
// 	holeSprites.push_back(new HoleSprite(spritesTexture, centerW - offsetW - 32, centerH + offsetH));
// 	holeSprites.push_back(new HoleSprite(spritesTexture, centerW - offsetW, centerH + 72 + offsetH));

// 	// Center hole
// 	holeSprites.push_back(new HoleSprite(spritesTexture, centerW, centerH + offsetH));

// 	// Column 2
// 	holeSprites.push_back(new HoleSprite(spritesTexture, centerW + offsetW, centerH - 72 + offsetH));
// 	holeSprites.push_back(new HoleSprite(spritesTexture, centerW + offsetW + 32, centerH + offsetH));
// 	holeSprites.push_back(new HoleSprite(spritesTexture, centerW + offsetW, centerH + 72 + offsetH));
// }

// int PlayScene::pick_holeType()
// {
// 	// Pick hole type
// 	std::vector<HoleType> bag;
// 	int wgt_goon = 85;
// 	int wgt_town = 10;
// 	int wgt_mayr = 5;

// 	for (int i = 0; i < wgt_goon; i++)
// 	{
// 		bag.push_back(HT_Goon);
// 	}
// 	for (int i = 0; i < wgt_town; i++)
// 	{
// 		bag.push_back(HT_Townie);
// 	}
// 	for (int i = 0; i < wgt_mayr; i++)
// 	{
// 		bag.push_back(HT_Mayor);
// 	}

// 	std::random_shuffle(bag.begin(), bag.end());
// 	int pick = rand() % bag.size() + 0;

// 	return bag[pick];
// }

// void PlayScene::ch_gstate(PlaySceneState n_state)
// {
// 	// Change state and apply changes based on past state and new state
// 	if (m_gstate == PS_WARMUP && n_state == PS_RUNNING)
// 	{
// 		m_gstate = n_state;

// 		genShake();

// 		// Init timers
// 		int now = SDL_GetTicks();
// 		tmr_activateHole = now;
// 		tmr_progression = now;
// 		tmr_fspawning = now;
// 		tmr_prtspawn = now;
// 	}
// 	else
// 	{
// 		m_gstate = n_state;
// 	}
// }

// void PlayScene::genShake()
// {
// 	for (int i = 0; i < MAX_SHAKE; i++)
// 	{
// 		int val;
// 		if (i % 2 == 0)
// 		{
// 			val = 5;
// 		}
// 		else
// 		{
// 			val = -5;
// 		}
// 		vShake.push_back(val);
// 	}

// 	tmr_shake = SDL_GetTicks();
// }

// void PlayScene::shake()
// {
// 	if (!vShake.empty())
// 	{
// 		int now = SDL_GetTicks();
// 		bool timesUp = now - tmr_shake > DELAY_SHAKE;

// 		if (timesUp)
// 		{
// 			tmr_shake = now;
// 			int val = vShake.back();
// 			vShake.pop_back();

// 			targRect.x = val;
// 		}
// 	}
// 	else
// 	{
// 		targRect.x = 0;
// 	}
// }

// void PlayScene::delayDeathCdown()
// {
// 	if (tmr_deathCdown != 0)
// 	{
// 		const float dcdown_incr = (float)dur_activateHole / 0.6;
// 		tmr_deathCdown += (int)dcdown_incr;
// 		const int now = SDL_GetTicks();
// 		//printf("tmr: %d, now: %d\n", tmr_deathCdown, now);

// 		if (now - tmr_deathCdown <= 0)
// 		{
// 			tmr_deathCdown = SDL_GetTicks();
// 			//printf("DEBUG: Exceeded duration!\n");
// 		}
// 	}
// }

// void PlayScene::decrDthCdownDur()
// {
// 	// Decrease dur_deathCdown;
// 	dur_deathCdown -= DECREMENT_DUR_DEATHCDOWN;
// }

// void PlayScene::initDthCdown()
// {
// 	if (tmr_deathCdown == 0)
// 	{
// 		int now = SDL_GetTicks();
// 		tmr_deathCdown = now;
// 	}
// }

// void PlayScene::init_uistuff()
// {
// 	scoreIcon.set_clip({0, 8, 8, 8});
// 	opinionIcon.set_clip({0, 0, 8, 8});
// }

// void PlayScene::spawnFadeText()
// {
// 	std::string text = "-" + std::to_string(SCOR_PENALTY);
// 	int x = scoreIcon.get_rect().x + 36;
// 	int y = scoreIcon.get_rect().y;

// 	// i can just set w and h to 0 here.
// 	FadeText ftext(text.c_str(), {x, y, 0, 0}, {0, 4});
// 	m_fadetexts.push_back(ftext);
// }

// void PlayScene::mk_buttons()
// {

// 	std::map<BtnState, SDL_Rect> btnBack_clips;
// 	btnBack_clips[BST_NORMAL] = {0, 16, 8, 8};
// 	btnBack_clips[BST_HOVERED] = {8, 16, 8, 8};
// 	Button *btnBack = new Button(uiTexture, {364, 4, 0, 0}, btnBack_clips);
// 	btnBack->bindCallback(std::bind(&PlayScene::goback, this));
// 	m_buttons.push_back(btnBack);

// 	//delete btnBack;
// }

// void PlayScene::goback()
// {
// 	m_context->changeScene(MENU_SCENE);
// }

// //}
// #pragma endregion PlayScene

// // #pragma region GameOverScene
// //{ GameOverScene
// GameOverScene::GameOverScene(SceneContext *context)
// {
// 	m_context = context;
// 	finalScore = m_context->gInfo.score;

// 	// Mouse
// 	z_mouse.isClicked = false;

// 	// Anchor points
// 	const int wcx = WINDOW_WIDTH / 2;
// 	const int wcy = WINDOW_HEIGHT / 2;
// 	const int wbtom = WINDOW_HEIGHT;
// 	const int btnW = 16;
// 	const int btnH = 16;
// 	const int bcx = btnW / 2;

// 	// Create button
// 	std::map<BtnState, SDL_Rect> btnMenu_clips;
// 	btnMenu_clips[BST_NORMAL] = {32, 40, btnW, btnH};
// 	btnMenu_clips[BST_HOVERED] = {48, 40, btnW, btnH};

// 	Button *btnMenu = new Button(uiTexture, {(wcx - bcx) - 64, wbtom - 128, 0, 0}, btnMenu_clips);
// 	btnMenu->bindCallback(std::bind(&GameOverScene::chs_menu, this));
// 	m_buttons.push_back(btnMenu);

// 	std::map<BtnState, SDL_Rect> btnRetry_clips;
// 	btnRetry_clips[BST_NORMAL] = {0, 40, btnW, btnH};
// 	btnRetry_clips[BST_HOVERED] = {16, 40, btnW, btnH};

// 	Button *btnRetry = new Button(uiTexture, {(wcx - bcx) + 16, wbtom - 128, 0, 0}, btnRetry_clips);
// 	btnRetry->bindCallback(std::bind(&GameOverScene::chs_retry, this));
// 	m_buttons.push_back(btnRetry);
// }

// GameOverScene::~GameOverScene()
// {
// 	for (auto btn : m_buttons)
// 		delete btn;
// 	m_buttons.clear();
// }

// void GameOverScene::handleEvents(SDL_Event *e)
// {
// 	// Handle events --------------------------------------
// 	while (SDL_PollEvent(e))
// 	{
// 		if (e->type == SDL_QUIT)
// 		{
// 			m_context->quit();
// 		}
// 		else if (e->type == SDL_MOUSEBUTTONDOWN)
// 		{
// 			z_mouse.isClicked = true;
// 		}
// 		else if (e->type == SDL_MOUSEBUTTONUP)
// 		{
// 			z_mouse.isClicked = false;
// 		}
// 	}

// 	// Update mouse position
// 	SDL_GetMouseState(&z_mouse.pos.x, &z_mouse.pos.y);
// }

// void GameOverScene::update()
// {
// 	// Update buttons
// 	for (Button(*btn) : m_buttons)
// 	{
// 		btn->update(&z_mouse);
// 	}
// }

// void GameOverScene::draw(SDL_Renderer *renderer)
// {
// 	// Anchor points
// 	const int wcx = WINDOW_WIDTH / 2;
// 	const int wcy = WINDOW_HEIGHT / 2;

// 	// Set render draw color, and clear renderer ----------
// 	SDL_SetRenderDrawColor(renderer, BG_COLOR.r, BG_COLOR.g, BG_COLOR.b, 255);
// 	SDL_RenderClear(renderer);

// 	std::string scoreMsg = "YOU SCORED " + std::to_string(finalScore) + " PTS!";

// 	drawText(renderer, "GAME OVER", gFontL, wcx, 16, {255, 255, 255}, true);
// 	drawText(renderer, scoreMsg.c_str(), gFont, wcx, 96, {255, 255, 255}, true);

// 	// Draw buttons --------------------------
// 	for (Button(*button) : m_buttons)
// 	{
// 		button->draw(renderer);
// 	}

// 	// Render
// 	SDL_RenderPresent(renderer);
// }

// void GameOverScene::chs_menu()
// {
// 	m_context->changeScene(MENU_SCENE);
// }

// void GameOverScene::chs_retry()
// {
// 	m_context->changeScene(PLAY_SCENE);
// }
// //}
#pragma endregion GameOverScene