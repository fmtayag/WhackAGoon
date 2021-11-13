#include <iostream>
#include <SDL2/SDL.h>
#include "scenes.h"

#pragma region MenuScene
MenuScene::MenuScene() : tmr1()
{
    tmr1.start();
}
MenuScene::~MenuScene()
{
}
void MenuScene::handleEvents(SDL_Event *e)
{
    while (SDL_PollEvent(e))
    {
        if (e->type == SDL_QUIT)
        {
            m_context->setQuitFlag(true);
        }
        if (e->type == SDL_KEYDOWN)
        {
            switch (e->key.keysym.sym)
            {
            case SDLK_s:
                m_context->transitionTo(new PlayScene());
                break;
            case SDLK_p:
                tmr1.pause();
                break;
            case SDLK_u:
                tmr1.unpause();
                break;
            case SDLK_t:
                tmr1.stop();
                break;
            }
        }
    }
}
void MenuScene::update()
{
}
void MenuScene::draw(SDL_Renderer *renderer)
{
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, m_context->fetchGameAssets()->brickBGTexture, NULL, NULL);
    SDL_RenderPresent(renderer);
    printf("tmr1 ticks: %zu\n", tmr1.getTicks());
    //printf("SDL_Error: %s.\n", SDL_GetError());
};
#pragma endregion MenuScene

#pragma region PlayScene
PlayScene::PlayScene()
{
}
PlayScene::~PlayScene()
{
}
void PlayScene::handleEvents(SDL_Event *e)
{
    while (SDL_PollEvent(e))
    {
        if (e->type == SDL_QUIT)
        {
            m_context->setQuitFlag(true);
        }
        if (e->type == SDL_KEYDOWN)
        {
            switch (e->key.keysym.sym)
            {
            case SDLK_s:
                m_context->transitionTo(new MenuScene());
                break;
            }
        }
    }
}
void PlayScene::update()
{
}
void PlayScene::draw(SDL_Renderer *renderer)
{
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, m_context->fetchGameAssets()->cityBGTexture, NULL, NULL);
    SDL_RenderPresent(renderer);
    printf("SDL_Error: %s.\n", SDL_GetError());
};
#pragma endregion PlayScene