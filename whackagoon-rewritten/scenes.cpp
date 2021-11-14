#include <iostream>
#include <functional>
#include <cassert>
#include <SDL2/SDL.h>
#include "scenes.h"
#include "g_texture.h"
#include "g_data.h"

#pragma region MenuScene
MenuScene::MenuScene()
{
    loadAssets();
    createButtons();
}
MenuScene::~MenuScene()
{
    delete btnPlayTexture;
    delete btnPlay;
}
void MenuScene::handleEvents(SDL_Event *e)
{
    while (SDL_PollEvent(e))
    {
        if (e->type == SDL_QUIT)
        {
            m_context->setQuitFlag(true);
        }
        if (e->type == SDL_MOUSEBUTTONDOWN)
        {
            m_gMouse.isClicked = true;
        }
        if (e->type == SDL_MOUSEBUTTONUP)
        {
            m_gMouse.isClicked = false;
        }
        if (e->type == SDL_KEYDOWN)
        {
            switch (e->key.keysym.sym)
            {
            case SDLK_F1:
                m_context->transitionTo(new MenuScene());
                break;
            }
        }
    }

    SDL_GetMouseState(&m_gMouse.position.x, &m_gMouse.position.y);
}
void MenuScene::update()
{
    btnPlay->update(&m_gMouse);
}
void MenuScene::draw()
{
    SDL_SetRenderDrawColor(gameRenderer, 0, 0, 0, 255);
    SDL_RenderClear(gameRenderer);

    btnPlay->draw();

    SDL_RenderPresent(gameRenderer);
};
void MenuScene::cbPlay()
{
    printf("MenuScene::cbPlay() | TODO.\n");
}
void MenuScene::loadAssets()
{
    // Images
    btnPlayTexture = new GTexture();
    btnPlayTexture->loadFromFile("assets/images/ui_elements.png");

    // Sounds
    // --- TODO ---
}
void MenuScene::createButtons()
{
    WindowMetadata winData;

    SDL_Rect btnPlay_rect = {50, 50, 16 * winData.PXSCALE, 16 * winData.PXSCALE};
    std::map<BtnState, SDL_Rect> btnPlay_clips;
    btnPlay_clips[BST_NORMAL] = {0, 24, 16, 16};
    btnPlay_clips[BST_HOVERED] = {16, 24, 16, 16};
    btnPlay = new GButton(btnPlayTexture, btnPlay_rect, btnPlay_clips);
    btnPlay->bindCallback(std::bind(&MenuScene::cbPlay, this));
}
#pragma endregion MenuScene