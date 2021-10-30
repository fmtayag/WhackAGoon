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
}

SceneContext::~SceneContext()
{
	delete this->pScene;
	this->pScene = NULL;
}

void SceneContext::changeScene(SceneID scene)
{
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
	case PLAY_SCENE:
		this->pScene = new PlayScene(this);
		break;
	case GAMEOVER_SCENE:
		this->pScene = new GameOverScene(this);
		break;
	case DEBUG_SCENE:
		this->pScene = new DebugScene(this);
		break;
	default:
		this->pScene = NULL;
		break;
	}
}

//}
#pragma endregion SceneContext

#pragma region MenuScene
//{ MenuScene
MenuScene::MenuScene(SceneContext *context)
{
	mContext = context;
	z_mouse.isClicked = false;

	// Buttons
	Button *btn1 = new Button(NULL, "TESTING", {100, 100, 96, 32});
	btn1->bindCallback(std::bind(&MenuScene::chs_playGame, this));
	buttons.push_back(btn1);
}

MenuScene::~MenuScene()
{
	for (Button *btn : buttons)
	{
		delete btn;
		btn = NULL;
	}

	printf("Deleted menu scene.\n");
}

void MenuScene::handleEvents(SDL_Event *e)
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
				mContext->changeScene(DEBUG_SCENE);
			}
		}
	}

	// Update mouse position
	SDL_GetMouseState(&z_mouse.pos.x, &z_mouse.pos.y);
}

void MenuScene::update()
{
	//  Update buttons
	for (Button *btn : buttons)
	{
		btn->update(&z_mouse);
	}
}

void MenuScene::draw(SDL_Renderer *renderer)
{
	// Set render draw color, and clear renderer ----------
	SDL_SetRenderDrawColor(renderer, BG_COLOR.r, BG_COLOR.g, BG_COLOR.b, 255);
	SDL_RenderClear(renderer);

	drawText(renderer, "MENU SCENE", gFontL, 0, 0, {255, 255, 255});
	drawText(renderer, "PRESS ENTER TO PLAY", gFontL, 0, 64, {255, 255, 255});
	//drawText(renderer, "Game v1.0", gFontS, WINDOW_WIDTH / 2, (int)(WINDOW_HEIGHT * 0.88), {255, 255, 255}, true);
	drawText(renderer, "(c) 2021 Zyenapz", gFontS, WINDOW_WIDTH / 2, (int)(WINDOW_HEIGHT * 0.90), {255, 255, 255}, true);
	//drawText(renderer, "Codelic, GPL-3.0. Artlic CC BY-NC 4.0.", gFontS, WINDOW_WIDTH / 2, (int)(WINDOW_HEIGHT * 0.92), {255, 255, 255}, true);

	// Draw buttons --------------------------
	for (Button *button : buttons)
	{
		button->draw(renderer);
	}

	// Render
	SDL_RenderPresent(renderer);
}

void MenuScene::chs_playGame()
{
	mContext->changeScene(PLAY_SCENE);
}
//}
#pragma endregion MenuScene

#pragma region PlayScene
//{ PlayScene
PlayScene::PlayScene(SceneContext *context)
	: scoreIcon(uiTexture, {PXSCALE, PXSCALE, 32, 32}, 500),
	  opinionIcon(uiTexture, {PXSCALE, PXSCALE * 8, 32, 32}, 500)
{
	mContext = context;
	mk_holes();
	init_uistuff();
}

PlayScene::~PlayScene()
{
	holeSprites.clear();
	m_particles.clear();

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
		else if (e->type == SDL_KEYDOWN)
		{
			// FOR DEBUGGING ONLY!!
			switch (e->key.keysym.sym)
			{
			case SDLK_ESCAPE:
				mContext->changeScene(MENU_SCENE);
				break;
			case SDLK_RETURN:
				ch_gstate(PS_GAMEOVER);
				break;
			case SDLK_LEFT:
				targRect.x -= 5;
				break;
			case SDLK_RIGHT:
				targRect.x += 5;
				break;
			case SDLK_s:
				genShake();
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
	switch (m_gstate)
	{
	case PS_PAUSED:
		break;
	case PS_WARMUP:
		u_wuTimer();
		break;
	case PS_GAMEOVER:
		u_initTransitionTimer();
		u_transgameover();
		shake();
		break;
	case PS_RUNNING:
		u_checkDeathTimer();
		u_checkopinion();
		u_collision();
		u_holes();
		u_activateDur();
		u_activateHoles();
		u_spawnPrt();
		u_prt();
		u_uistuff();
		shake();
		break;
	default:
		printf("Warning: m_gstate reached default case.\n");
		break;
	}
}

void PlayScene::u_checkDeathTimer()
{
	bool timerInitialized = tmr_deathCdown != 0;
	if (timerInitialized)
	{
		int now = SDL_GetTicks();
		int timeLeft = now - tmr_deathCdown;
		bool tUp = timeLeft > dur_deathCdown; // is time up?

		const int troThreshold = 1000;
		bool trOut = (dur_deathCdown - timeLeft) <= troThreshold; // is time running out?
		bool fsAllowed = now - tmr_fspawning > DUR_FSPAWNING;

		//printf("now - tmr: %d, dur: %d\n", now - tmr_deathCdown, dur_deathCdown);

		if (trOut && fsAllowed)
		{
			tmr_fspawning = now;

			// force spawn a goon type hole
			u_activateHoles(true, HT_Goon);
			printf("Force spawning a goon!\n");
		}

		if (tUp)
		{
			ch_gstate(PS_GAMEOVER);
			gOverMsg = "SMASH, YA LAZY BUM!";
		}
	}
}

void PlayScene::u_checkopinion()
{
	if (opinion <= 0)
	{
		ch_gstate(PS_GAMEOVER);
		gOverMsg = "YOU'RE FIRED!";
	}
}

void PlayScene::u_wuTimer()
{
	// Update warm up timer
	int now = SDL_GetTicks();
	bool timesUp = now - tmr_warmuptimer > DUR_WARMUPTIMER;

	if (timesUp)
	{
		ch_gstate(PS_RUNNING);
	}
}

void PlayScene::u_initTransitionTimer()
{
	// init timer
	if (tmr_transtogameover == 0)
		tmr_transtogameover = SDL_GetTicks();
}

void PlayScene::u_transgameover()
{

	int now = SDL_GetTicks();
	bool timesUp = now - tmr_transtogameover > DUR_TRANSTOGAMEOVER;

	if (timesUp)
	{
		mContext->gInfo.score = score;

		// should be placed as last instruction!
		mContext->changeScene(GAMEOVER_SCENE);
	}
}

void PlayScene::u_collision()
{

	for (HoleSprite &hole : holeSprites)
	{
		bool collided = isPointCollide(mpos, hole.getRect());
		if (collided && mMouseClicked)
		{
			bool isWhacked = hole.whack();
			mMouseClicked = false;

			if (isWhacked)
			{
				switch (hole.getType())
				{
				case HT_Goon:
					score++;
					delayDeathCdown();
					initDthCdown();
					break;
				case HT_Townie:
					score -= SCOR_PENALTY;
					opinion -= 1;
					genShake();
					decrDthCdownDur();
					break;
				case HT_Mayor:
					ch_gstate(PS_GAMEOVER);
					gOverMsg = "YOU HIT THE MAYOR!";
					genShake();
					break;
				default:
					printf("Warning: default case has been reached for collision check.\n");
					break;
				}

				// neg score check
				if (score < 0)
					score = 0;
			}
		}
	}
}

void PlayScene::u_holes()
{
	for (HoleSprite &hole : holeSprites)
	{
		hole.update();
	}
}

void PlayScene::u_activateDur()
{
	// Update dur_activateHole as time goes on.
	int now = SDL_GetTicks();
	bool timesUp = now - tmr_progression > DUR_PROGRESSION;
	bool aboveMin = dur_activateHole > MIN_DURACTV_VAL;

	if (timesUp && aboveMin)
	{
		tmr_progression = now;

		dur_activateHole -= rand() % DECREMENT_MIN + DECREMENT_MAX;
		if (dur_activateHole <= MIN_DURACTV_VAL)
		{
			dur_activateHole = MIN_DURACTV_VAL;
		}
		printf("Debug: dur_activateHole is %d.\n", dur_activateHole);
	}
}

void PlayScene::u_activateHoles(bool isForced, HoleType forcedType)
{
	int now = SDL_GetTicks();
	bool timesUp = now - tmr_activateHole > dur_activateHole;

	if (timesUp || isForced)
	{
		tmr_activateHole = now;
		for (HoleSprite &hole : holeSprites)
		{

			bool selectThis = rand() & 1; // choose either 0 or 1
			bool holeIsResting = hole.getAnimState() == AS_Resting;

			if (selectThis && holeIsResting)
			{
				// Awake hole
				HoleType ht = HT_None;

				if (forcedType != HT_None)
					ht = forcedType;
				else
					ht = (HoleType)pick_holeType();

				hole.awake(ht);
				break;
			}
		}
	}
}

void PlayScene::u_spawnPrt()
{
	int now = SDL_GetTicks();
	bool timesUp = now - tmr_prtspawn > DUR_PRTSPAWN;

	if (timesUp)
	{
		tmr_prtspawn = now;

		// spawn particle
		if (m_particles.size() < 10)
		{
			SDL_Rect prtRect;
			prtRect.x = rand() % WINDOW_WIDTH + 1;
			prtRect.y = rand() % 132 + 80;
			prtRect.w = PXSCALE;
			prtRect.h = PXSCALE;

			printf("X: %d, Y: %d\n", prtRect.x, prtRect.y);

			SDL_Color prtCol = {23, 31, 11}; // toxic green
			SDL_Point prtVel = {0, -1};

			m_particles.push_back(Particle(prtRect, prtCol, prtVel));
		}
	}
}

void PlayScene::u_prt()
{
	//printf("m_particles size: %d\n", m_particles.size());
	// update particles
	auto iter = m_particles.begin();
	for (Particle &p : m_particles)
	{
		if (p.isVisible())
		{
			p.update();
		}
		else
		{
			m_particles.erase(iter);
		}
		iter++;
	}
}

void PlayScene::u_uistuff()
{
	scoreIcon.update();

	// opinion icon stuff
	opinionIcon.update();
	opine_clips opin_clips;
	switch (opinion)
	{
	case 3:
		opinionIcon.set_clip(opin_clips.high);
		break;
	case 2:
		opinionIcon.set_clip(opin_clips.med);
		break;
	case 1:
		opinionIcon.set_clip(opin_clips.low);
		break;
	default:
		opinionIcon.set_clip(opin_clips.low);
		break;
	}
}

void PlayScene::draw(SDL_Renderer *renderer)
{
	// Colors
	SDL_Color BLACK = {0, 0, 0, 255};
	SDL_Color WHITE = {255, 255, 255, 255};

	SDL_SetRenderDrawColor(renderer, BG_COLOR.r, BG_COLOR.g, BG_COLOR.b, BG_COLOR.a);
	SDL_RenderClear(renderer);

	SDL_Texture *targetTexture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		targRect.w,
		targRect.h);
	SDL_SetRenderTarget(renderer, targetTexture);

	draw_bg(renderer);
	draw_prt(renderer, targetTexture);
	draw_city(renderer);
	draw_holes(renderer);
	draw_texts(renderer);
	draw_deathTimer(renderer);
	draw_uistuff(renderer);

	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, targetTexture, NULL, &targRect);
	SDL_RenderPresent(renderer);

	SDL_DestroyTexture(targetTexture);
}

void PlayScene::draw_texts(SDL_Renderer *renderer)
{
	// Colors
	SDL_Color WHITE = {255, 255, 255, 255};
	SDL_Color RED = {255, 0, 0, 255};

	// Anchor points, offsets
	const int winCenterW = WINDOW_WIDTH / 2;
	const int winCenterH = WINDOW_HEIGHT / 2;
	const int offsetX = 16;

	// Messages
	std::string scoreMessage = std::to_string(score);

	if (m_gstate == PS_RUNNING | m_gstate == PS_GAMEOVER)
	{
		int scormsg_x = (scoreIcon.get_rect().x + scoreIcon.get_rect().w) + 4;
		int scormsg_y = scoreIcon.get_rect().y + (scoreIcon.get_rect().h / 2);
		drawText(renderer, scoreMessage.c_str(), gFont, scormsg_x, scormsg_y, WHITE, false, true);

		drawText(renderer, "TIMER", gFont, winCenterW, 32, WHITE, true);
	}

	if (m_gstate == PS_WARMUP)
	{
		int now = SDL_GetTicks();
		int warmupTimeLeft = (DUR_WARMUPTIMER - (now - tmr_warmuptimer)) / 1000;
		std::string warmupTimeMsg = std::to_string(warmupTimeLeft).c_str();

		drawText(renderer, "GET READY", gFontL, winCenterW, winCenterH, WHITE, true);
		drawText(renderer, warmupTimeMsg, gFontL, winCenterW, winCenterH + 32, WHITE, true);
	}
	else if (m_gstate == PS_GAMEOVER)
	{
		drawText(renderer, "GAME OVER!", gFontL, winCenterW, winCenterH, WHITE, true);
		drawText(renderer, gOverMsg.c_str(), gFont, winCenterW, winCenterH + 32, WHITE, true);
	}
}

void PlayScene::draw_holes(SDL_Renderer *renderer)
{
	for (HoleSprite &hole : holeSprites)
	{
		hole.draw(renderer);
	}
}

void PlayScene::draw_bg(SDL_Renderer *renderer)
{
	SDL_RenderCopy(renderer, bgTexture, NULL, NULL);
}

void PlayScene::draw_city(SDL_Renderer *renderer)
{
	SDL_RenderCopy(renderer, cityTexture, NULL, NULL);
}

void PlayScene::draw_deathTimer(SDL_Renderer *renderer)
{
	if (m_gstate == PS_RUNNING)
	{

		if (true)
		{
			int now = SDL_GetTicks();
			int tmr = tmr_deathCdown;
			double dur = dur_deathCdown;

			// Normalize time left
			float valTL = now - tmr;
			float minTL = 0;
			float maxTL = dur_deathCdown;
			float normTL = (valTL - minTL) / (maxTL - minTL);

			// Set bar color
			gcolors colors;
			SDL_Color barColor = colors.pure_white;
			if (normTL >= 0.5)
				barColor = colors.red;

			// Normalize death countdown duration
			float valDD = dur_deathCdown;
			float minDD = 0;
			float maxDD = MAX_DUR_DEATHCDOWN;
			float normDD = (valDD - minDD) / (maxDD - minDD);

			// Set bar width
			float y = 192 * normDD;
			//printf("y: %f.\n", y);
			float bar_width = y - (y * normTL);
			if (bar_width <= 0)
			{
				bar_width = 0;
			}

			if (tmr_deathCdown == 0)
			{
				bar_width = y;
				barColor = colors.pure_white;
			}

			short int wcW = WINDOW_WIDTH / 2;
			short int bwc = bar_width / 2;
			short int x = wcW - bwc;

			SDL_Rect dtbarRect;
			dtbarRect.x = x;
			dtbarRect.y = 16;
			dtbarRect.w = (int)bar_width;
			dtbarRect.h = 8;

			SDL_SetRenderDrawColor(renderer, barColor.r, barColor.g, barColor.b, barColor.a);
			SDL_RenderFillRect(renderer, &dtbarRect);
		}
	}
}

void PlayScene::draw_prt(SDL_Renderer *renderer, SDL_Texture *parentTargTexture)
{
	for (Particle &p : m_particles)
	{
		p.bindParentTargTexture(parentTargTexture);
		p.draw(renderer);
	}
}

void PlayScene::draw_uistuff(SDL_Renderer *renderer)
{
	if (m_gstate == PS_RUNNING | m_gstate == PS_GAMEOVER)
	{
		scoreIcon.draw(renderer);
		opinionIcon.draw(renderer);
	}
}

void PlayScene::mk_holes()
{
	/*** Create the holes ***/

	// Anchor points, offsets
	const int centerW = (WINDOW_WIDTH / 2) - (HOLE_WIDTH / 2);
	const int centerH = (WINDOW_HEIGHT / 2) - (HOLE_HEIGHT / 2);
	const int offsetW = 80;
	const int offsetH = 80;

	// Column 1
	holeSprites.push_back(HoleSprite(spritesTexture, centerW - offsetW, centerH - 72 + offsetH));
	holeSprites.push_back(HoleSprite(spritesTexture, centerW - offsetW - 32, centerH + offsetH));
	holeSprites.push_back(HoleSprite(spritesTexture, centerW - offsetW, centerH + 72 + offsetH));

	// Center hole
	holeSprites.push_back(HoleSprite(spritesTexture, centerW, centerH + offsetH));

	// Column 2
	holeSprites.push_back(HoleSprite(spritesTexture, centerW + offsetW, centerH - 72 + offsetH));
	holeSprites.push_back(HoleSprite(spritesTexture, centerW + offsetW + 32, centerH + offsetH));
	holeSprites.push_back(HoleSprite(spritesTexture, centerW + offsetW, centerH + 72 + offsetH));
}

int PlayScene::pick_holeType()
{
	// Pick hole type
	std::vector<HoleType> bag;
	int wgt_goon = 85;
	int wgt_town = 10;
	int wgt_mayr = 5;

	for (int i = 0; i < wgt_goon; i++)
	{
		bag.push_back(HT_Goon);
	}
	for (int i = 0; i < wgt_town; i++)
	{
		bag.push_back(HT_Townie);
	}
	for (int i = 0; i < wgt_mayr; i++)
	{
		bag.push_back(HT_Mayor);
	}

	std::random_shuffle(bag.begin(), bag.end());
	int pick = rand() % bag.size() + 0;

	return bag[pick];
}

void PlayScene::ch_gstate(PlaySceneState n_state)
{
	// Change state and apply changes based on past state and new state
	if (m_gstate == PS_WARMUP && n_state == PS_RUNNING)
	{
		m_gstate = n_state;

		// Init timers
		int now = SDL_GetTicks();
		tmr_activateHole = now;
		tmr_progression = now;
		tmr_fspawning = now;
		tmr_prtspawn = now;
	}
	else
	{
		m_gstate = n_state;
	}
}

void PlayScene::genShake()
{
	for (int i = 0; i < MAX_SHAKE; i++)
	{
		int val;
		if (i % 2 == 0)
		{
			val = 5;
		}
		else
		{
			val = -5;
		}
		vShake.push_back(val);
	}

	tmr_shake = SDL_GetTicks();
}

void PlayScene::shake()
{
	if (!vShake.empty())
	{
		int now = SDL_GetTicks();
		bool timesUp = now - tmr_shake > DELAY_SHAKE;

		if (timesUp)
		{
			tmr_shake = now;
			int val = vShake.back();
			vShake.pop_back();

			targRect.x = val;
		}
	}
}

void PlayScene::delayDeathCdown()
{
	if (tmr_deathCdown != 0)
	{
		const float dcdown_incr = (float)dur_activateHole / 0.6;
		tmr_deathCdown += (int)dcdown_incr;
		const int now = SDL_GetTicks();
		//printf("tmr: %d, now: %d\n", tmr_deathCdown, now);

		if (now - tmr_deathCdown <= 0)
		{
			tmr_deathCdown = SDL_GetTicks();
			//printf("DEBUG: Exceeded duration!\n");
		}
	}
}

void PlayScene::decrDthCdownDur()
{
	// Decrease dur_deathCdown;
	dur_deathCdown -= DECREMENT_DUR_DEATHCDOWN;
}

void PlayScene::initDthCdown()
{
	if (tmr_deathCdown == 0)
	{
		int now = SDL_GetTicks();
		tmr_deathCdown = now;
	}
}

void PlayScene::init_uistuff()
{
	scoreIcon.set_clip({0, 8, 8, 8});
	opinionIcon.set_clip({0, 0, 8, 8});
}

//}
#pragma endregion PlayScene

#pragma region GameOverScene
//{ GameOverScene
GameOverScene::GameOverScene(SceneContext *context)
{
	mContext = context;
	finalScore = mContext->gInfo.score;

	// Mouse
	z_mouse.isClicked = false;

	// Anchor points
	const int wcx = WINDOW_WIDTH / 2;
	const int wcy = WINDOW_HEIGHT / 2;
	const int wbtom = WINDOW_HEIGHT;
	const int btnW = 96;
	const int btnH = 32;
	const int bcx = btnW / 2;

	// Create button
	Button btn1(
		btnTexture,
		"TESTING",
		{wcx - bcx, wbtom - 64, btnW, btnH});

	btn1.bindCallback(std::bind(&GameOverScene::chs_menu, this));
	buttons.push_back(btn1);
}

GameOverScene::~GameOverScene()
{
}

void GameOverScene::handleEvents(SDL_Event *e)
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

void GameOverScene::update()
{
	//  Update buttons
	for (Button btn : buttons)
	{
		btn.update(&z_mouse);
	}
}

void GameOverScene::draw(SDL_Renderer *renderer)
{
	// Anchor points
	const int wcx = WINDOW_WIDTH / 2;
	const int wcy = WINDOW_HEIGHT / 2;

	// Set render draw color, and clear renderer ----------
	SDL_SetRenderDrawColor(renderer, BG_COLOR.r, BG_COLOR.g, BG_COLOR.b, 255);
	SDL_RenderClear(renderer);

	std::string scoreMsg = "YOU SCORED " + std::to_string(finalScore) + " PTS!";

	drawText(renderer, "GAME OVER", gFontL, wcx, 16, {255, 255, 255}, true);
	drawText(renderer, scoreMsg.c_str(), gFont, wcx, 96, {255, 255, 255}, true);

	// Draw buttons --------------------------
	for (Button button : buttons)
	{
		button.draw(renderer);
	}

	// Render
	SDL_RenderPresent(renderer);
}

void GameOverScene::chs_menu()
{
	mContext->changeScene(MENU_SCENE);
}
//}
#pragma endregion GameOverScene

#pragma region DebugScene
DebugScene::DebugScene(SceneContext *context)
{
	m_context = context;
	createButtons();
}
DebugScene::~DebugScene()
{
}
void DebugScene::handleEvents(SDL_Event *e)
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
				m_context->changeScene(MENU_SCENE);
				break;
			case SDLK_1:
				spawnFadeTxt();
				break;
			case SDLK_2:
				spawnParticle();
			default:
				break;
			}
		}
	}

	// Update mouse position
	SDL_GetMouseState(&z_mouse.pos.x, &z_mouse.pos.y);
}
void DebugScene::update()
{
	u_fadeTxts();
	u_buttons();
	u_prts();
}
void DebugScene::u_fadeTxts()
{
	auto iter = m_fadeTexts.begin();
	for (FadeText &fade : m_fadeTexts)
	{
		if (fade.isVisible())
		{
			fade.update();
		}
		else
		{
			m_fadeTexts.erase(iter);
		}
		iter++;
	}
}

void DebugScene::u_buttons()
{
	for (Button &btn : m_buttons)
	{
		btn.update(&z_mouse);
	}
}

void DebugScene::u_prts()
{
	auto iter = m_particles.begin();
	for (Particle &p : m_particles)
	{
		if (p.isVisible())
		{
			p.update();
		}
		else
		{
			m_particles.erase(iter);
		}
		iter++;
	}

	//printf("m_particles size: %d\n", m_particles.size());
}

void DebugScene::draw(SDL_Renderer *renderer)
{
	SDL_SetRenderDrawColor(renderer, BG_COLOR.r, BG_COLOR.g, BG_COLOR.b, BG_COLOR.a);
	SDL_RenderClear(renderer);

	draw_fadeTxts(renderer);
	draw_buttons(renderer);
	draw_prts(renderer);

	drawText(renderer, "DEBUG ROOM", gFontL, 0, 0, {255, 255, 255}, false);
	drawText(renderer, "1 ... SPAWN FADETEXT", gFont, 0, 64, {255, 255, 255}, false);
	drawText(renderer, "2 ... SPAWN PARTICLE", gFont, 0, 96, {255, 255, 255}, false);

	SDL_RenderPresent(renderer);
}

void DebugScene::draw_fadeTxts(SDL_Renderer *renderer)
{
	for (FadeText &fade : m_fadeTexts)
	{
		fade.draw(renderer);
	}
}

void DebugScene::draw_prts(SDL_Renderer *renderer)
{
	for (Particle &p : m_particles)
	{
		p.draw(renderer);
	}
}

void DebugScene::draw_buttons(SDL_Renderer *renderer)
{
	for (Button &btn : m_buttons)
	{
		btn.draw(renderer);
	}
}

void DebugScene::spawnFadeTxt()
{
	FadeText txt("DEBUG BRUH", {WINDOW_WIDTH / 2, 0, 0, 0}, {0, 2});
	m_fadeTexts.push_back(txt);
}
void DebugScene::createButtons()
{
	Button btn1(NULL, "CLICKME", {200, 200, 200, 32});
	m_buttons.push_back(btn1);
	printf("DEBUG: Spawned button.\n");
}
void DebugScene::spawnParticle()
{
	short int xvel = rand() % 19 + (-9);
	short int yvel = rand() % 19 + (-9);
	Particle p1({300, 50, PXSCALE, PXSCALE}, {255, 255, 255, 255}, {xvel, yvel});
	m_particles.push_back(p1);
}

#pragma endregion DebugScene