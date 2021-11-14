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
    delete brickBGTexture;
    delete uiElementsTexture;

    delete btnPlay;
    delete btnHelp;
    delete btnInfo;
    printf("MenuScene deleted.\n");
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
    btnHelp->update(&m_gMouse);
    btnInfo->update(&m_gMouse);
}
void MenuScene::draw()
{
    SDL_SetRenderDrawColor(gameRenderer, 0, 0, 0, 255);
    SDL_RenderClear(gameRenderer);

    brickBGTexture->draw();
    btnPlay->draw();
    btnHelp->draw();
    btnInfo->draw();

    SDL_RenderPresent(gameRenderer);
};
void MenuScene::loadAssets()
{
    // Images
    uiElementsTexture = new GTexture();
    uiElementsTexture->loadFromFile("assets/images/ui_elements.png");

    brickBGTexture = new GTexture();
    brickBGTexture->loadFromFile("assets/images/brick_bg.png");

    // Sounds
    // --- TODO ---
}
void MenuScene::createButtons()
{
    WindowMetadata winData;

    // --- btnPlay --
    SDL_Rect btnPlay_rect = {50, 50, 16 * winData.PXSCALE, 16 * winData.PXSCALE};
    std::map<BtnState, SDL_Rect> btnPlay_clips;
    btnPlay_clips[BST_NORMAL] = {0, 24, 16, 16};
    btnPlay_clips[BST_HOVERED] = {16, 24, 16, 16};
    btnPlay = new GButton(uiElementsTexture, btnPlay_rect, btnPlay_clips);
    btnPlay->bindCallback(std::bind(&MenuScene::cbPlay, this));

    // --- btnHelp ---
    SDL_Rect btnHelp_rect = {150, 50, 16 * winData.PXSCALE, 16 * winData.PXSCALE};
    std::map<BtnState, SDL_Rect> btnHelp_clips;
    btnHelp_clips[BST_NORMAL] = {32, 24, 16, 16};
    btnHelp_clips[BST_HOVERED] = {48, 24, 16, 16};
    btnHelp = new GButton(uiElementsTexture, btnHelp_rect, btnHelp_clips);
    btnHelp->bindCallback(std::bind(&MenuScene::cbHelp, this));

    // --- btnInfo ---
    SDL_Rect btnInfo_rect = {150, 150, 16 * winData.PXSCALE, 16 * winData.PXSCALE};
    std::map<BtnState, SDL_Rect> btnInfo_clips;
    btnInfo_clips[BST_NORMAL] = {0, 56, 16, 16};
    btnInfo_clips[BST_HOVERED] = {16, 56, 16, 16};
    btnInfo = new GButton(uiElementsTexture, btnInfo_rect, btnInfo_clips);
    btnInfo->bindCallback(std::bind(&MenuScene::cbInfo, this));
}
// *** CALLBACKS ***
void MenuScene::cbPlay()
{
    printf("MenuScene::cbPlay() | TODO.\n");
}
void MenuScene::cbHelp()
{
    printf("MenuScene::cbHelp() | TODO.\n");
}
void MenuScene::cbInfo()
{
    printf("MenuScene::cbInfo() | TODO.\n");
}
#pragma endregion MenuScene