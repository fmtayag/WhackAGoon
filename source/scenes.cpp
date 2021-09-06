#include <iostream>
#include <iterator>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "scenes.h"
#include "metadata.h"
#include "assets.h"
#include "sprites.h"
#include "utils.h"

/* -------------------------------------------------
 * Scene Context def
 * -------------------------------------------------
 */
SceneContext::SceneContext()
{
    mScene = new PlayScene(this);
}

void SceneContext::changeScene(AbstractScene *scene)
{
    delete this->mScene;
    mScene = scene;
}

void SceneContext::HandleEvents(SDL_Event *e, bool &isRunning)
{
    mScene->handleEvents(e, isRunning);
}

void SceneContext::Update()
{
    mScene->update();
}

void SceneContext::Draw(SDL_Renderer *renderer)
{
    mScene->draw(renderer);
}

/* -------------------------------------------------
 * Play Scene def
 * -------------------------------------------------
 */
PlayScene::PlayScene(SceneContext *context)
{
    mContext = context;

    gameTimer = SDL_GetTicks();
    score = 0;
    towniesHit = 0;
    isGameOver = false;
    gameOverMessage = "GAME OVER MESSAGE";
    mMouseClicked = false;

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

    hManager = new HoleManager(holeSprites);
}

PlayScene::~PlayScene()
{
    hManager = NULL;
}

void PlayScene::handleEvents(SDL_Event *e, bool &isRunning)
{
    // Handle events --------------------------------------
    while (SDL_PollEvent(e))
    {
        if (e->type == SDL_QUIT)
        {
            isRunning = false;
        }
        if (e->type == SDL_MOUSEBUTTONDOWN)
        {
            mMouseClicked = true;
        }
        if (e->type == SDL_MOUSEBUTTONUP)
        {
            mMouseClicked = false;
        }
    }

    // End game check
    if (isGameOver)
    {
        isRunning = false;
        printf("Game over: %s!\n", gameOverMessage.c_str());
    }

    // Update mouse position
    SDL_GetMouseState(&mx, &my);
    mpos[0] = mx;
    mpos[1] = my;
}

void PlayScene::update()
{
	// Update timer
	gameTimer = SDL_GetTicks();
	
	// time up check
	if( (GAME_DUR - gameTimer) / 1000 <= 0) {
		isGameOver = true;
		gameOverMessage = "Time's up!";
	}

    // Collision check
    for (std::vector<HoleSprite *>::const_iterator iter = holeSprites.begin(); iter != holeSprites.end(); iter++)
    {
        bool collided = isPointCollide(mpos, (*iter)->getRect());
        if (collided && mMouseClicked)
        {
            bool isWhacked = (*iter)->whack();
            mMouseClicked = false;

            if (isWhacked)
            {
                if ((*iter)->getType() == HT_Goon)
                {
                    score++;
                }
                else if ((*iter)->getType() == HT_Townie)
                {
                    score -= 3;
                }
                else if ((*iter)->getType() == HT_Mayor)
                {
                    printf("Debug: You hit the Mayor. Game over!\n");
					isGameOver = true;
                    gameOverMessage = "You hit the Mayor!\n";
                }
            }

            // neg score check
            if (score < 0)
                score = 0;
        }
    }

    // run update methods for the holes
    for (std::vector<HoleSprite *>::const_iterator iter = holeSprites.begin(); iter != holeSprites.end(); iter++)
    {
        (*iter)->update();
    }

    hManager->update();
}

void PlayScene::draw(SDL_Renderer *renderer)
{
    // Set render draw color, and clear renderer ----------
    SDL_SetRenderDrawColor(renderer, 5, 10, 10, 255);
    SDL_RenderClear(renderer);

    // Draw background ------------------------------------
    SDL_RenderCopy(renderer, bgTexture, NULL, NULL);

    // Draw holes ------------------------
    for (std::vector<HoleSprite*>::const_iterator iter = holeSprites.begin(); iter != holeSprites.end(); iter++)
    {
        (*iter)->draw(renderer);
    }

    // Draw texts -----------------------------------------
    std::string scoreMessage = std::to_string(score);
	int timeLeft = (GAME_DUR - gameTimer) / 1000;
	std::string timeMessage = std::to_string(timeLeft) + "s";
    const int txtCenterW = WINDOW_WIDTH / 2;
    const int offsetX = 16;
	
	// time msg color
	SDL_Color timeColor = {255, 255, 255};
	if( (GAME_DUR - gameTimer) / 1000 <= 10) {
		timeColor = {255, 0, 0};
	}
	
    drawText(renderer, "SCORE", gFont, txtCenterW - 64, 42, {255, 255, 255}, true);
    drawText(renderer, scoreMessage.c_str(), gFont, txtCenterW - 64, 64, {255, 255, 255}, true);
	
    drawText(renderer, "TIME", gFont, txtCenterW + 64, 42, timeColor, true);
    drawText(renderer, timeMessage.c_str(), gFont, txtCenterW + 64, 64, timeColor, true);

    // Render crap ----------------------------------------
    SDL_RenderPresent(renderer);
}
