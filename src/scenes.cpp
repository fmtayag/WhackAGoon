#include <iostream>
#include <memory>
#include <functional>
#include <string>
#include <cassert>
#include <vector>
#include <SDL.h>
#include "scenes.h"
#include "widgets.h"
#include "g_data.h"

#pragma region Scene
void Scene::checkTransition()
{
    switch (m_stflag)
    {
    case STF_TOPLAY:
        m_context->transitionTo(new PlayScene());
        break;
    case STF_TOMENU:
        m_context->transitionTo(new MenuScene());
    default:
        break;
    }

    m_stflag = STF_NONE;
}
#pragma endregion Scene

#pragma region MenuScene
MenuScene::MenuScene()
{
    resetMouseState();
    loadAssets();
    createButtons();
}
MenuScene::~MenuScene()
{
    std::string dbgMsg = fmt::format("{} destructor called.", typeid(*this).name());
    dbgPrint(DPL::DEBUG, dbgMsg);
}
void MenuScene::handleEvents(SDL_Event *e)
{
    // Reset mouse
    m_gMouse.isClicked = false;

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
                m_stflag = STF_TOMENU;
                break;
            case SDLK_F2:
                m_stflag = STF_TOPLAY;
                break;
            }
        }
    }

    // Get mouse state
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
    // Rects
    WindowMetadata winData;
    SDL_Rect logoRect = {50, 50, 80 * winData.PXSCALE, 50 * winData.PXSCALE};

    SDL_SetRenderDrawColor(gameRenderer, 0, 0, 0, 255);
    SDL_RenderClear(gameRenderer);

    brickBGTexture->draw();
    logoTexture->draw(NULL, &logoRect);
    btnPlay->draw();
    btnHelp->draw();
    btnInfo->draw();

    SDL_RenderPresent(gameRenderer);
};
void MenuScene::loadAssets()
{
    // Images
    uiElementsTexture = std::unique_ptr<GTexture>(new GTexture());
    uiElementsTexture->loadFromFile("assets/images/ui_elements.png");

    brickBGTexture = std::unique_ptr<GTexture>(new GTexture());
    brickBGTexture->loadFromFile("assets/images/brick_bg.png");

    logoTexture = std::unique_ptr<GTexture>(new GTexture());
    logoTexture->loadFromFile("assets/images/logo.png");

    // Sounds
    // --- TODO ---
}
void MenuScene::createButtons()
{
    WindowMetadata winData;

    // Anchor points
    int btnW = 16 * winData.PXSCALE;
    int btnH = 16 * winData.PXSCALE;
    int cx = (winData.WIDTH / 2) - (btnW / 2);

    // --- btnPlay --
    SDL_Rect btnPlay_rect = {cx, static_cast<int>(winData.HEIGHT * 0.70), btnW, btnH};
    std::map<BtnState, SDL_Rect> btnPlay_clips;

    btnPlay_clips[BST_NORMAL] = {0, 24, 16, 16};
    btnPlay_clips[BST_HOVERED] = {16, 24, 16, 16};
    btnPlay = std::make_unique<GButton>(GButton(uiElementsTexture.get(), btnPlay_rect, btnPlay_clips));
    btnPlay->bindCallback(std::bind(&MenuScene::cbPlay, this));

    // --- btnHelp ---
    SDL_Rect btnHelp_rect = {static_cast<int>(cx - (btnW * 1.5)), static_cast<int>(winData.HEIGHT * 0.75), btnW, btnH};
    std::map<BtnState, SDL_Rect> btnHelp_clips;
    btnHelp_clips[BST_NORMAL] = {32, 24, 16, 16};
    btnHelp_clips[BST_HOVERED] = {48, 24, 16, 16};
    btnHelp = std::make_unique<GButton>(GButton(uiElementsTexture.get(), btnHelp_rect, btnHelp_clips));
    btnHelp->bindCallback(std::bind(&MenuScene::cbHelp, this));

    // --- btnInfo ---
    SDL_Rect btnInfo_rect = {static_cast<int>(cx + (btnW * 1.5)), static_cast<int>(winData.HEIGHT * 0.75), btnW, btnH};
    std::map<BtnState, SDL_Rect> btnInfo_clips;
    btnInfo_clips[BST_NORMAL] = {0, 56, 16, 16};
    btnInfo_clips[BST_HOVERED] = {16, 56, 16, 16};
    btnInfo = std::make_unique<GButton>(GButton(uiElementsTexture.get(), btnInfo_rect, btnInfo_clips));
    btnInfo->bindCallback(std::bind(&MenuScene::cbInfo, this));
}
// *** CALLBACKS ***
void MenuScene::cbPlay()
{
    m_stflag = STF_TOPLAY;
}
void MenuScene::cbHelp()
{
    // --- TODO ---
}
void MenuScene::cbInfo()
{
    // --- TODO ---
}
#pragma endregion MenuScene

#pragma region PlayScene
PlayScene::PlayScene()
{
    resetMouseState();
    loadAssets();
    createButtons();
    initializeTimers();
    createHoles();
    initializeHoleMgr();
    initializeCollisionMgr();
    initializeTargetTexture();
}
PlayScene::~PlayScene()
{
    std::string dbgMsg = fmt::format("{} destructor called.", typeid(*this).name());
    dbgPrint(DPL::DEBUG, dbgMsg);
}
void PlayScene::handleEvents(SDL_Event *e)
{
    // Reset mouse
    m_gMouse.isClicked = false;

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
                m_stflag = STF_TOMENU;
                break;
            case SDLK_F2:
                m_stflag = STF_TOPLAY;
                break;
            case SDLK_1:
                m_holes[0]->awaken(Hole::HoleType::HT_GOON);
                break;
            case SDLK_2:
                m_holes[0]->awaken(Hole::HoleType::HT_TOWNIE);
                break;
            case SDLK_3:
                m_holes[0]->awaken(Hole::HoleType::HT_MAYOR);
                break;
            case SDLK_4:
                m_holes[0]->awaken(Hole::HoleType::HT_NONE);
                break;
            }
        }
    }

    SDL_GetMouseState(&m_gMouse.position.x, &m_gMouse.position.y);
}
void PlayScene::update()
{
    WindowMetadata winData;

    // Update buttons
    btnToMenu->update(&m_gMouse);

    // Update timers
    if (m_tmrWarmup.getTicks() >= m_delayWarmup)
    {
        // dbgPrint(DPL::DEBUG, "Warmup is over!");
    }

    // Check for collisions
    m_collideMgr->update();

    // Create effects if player whacked a Townie.
    bool whackedTownie = m_collideMgr->didWhackTownie();
    if (whackedTownie)
    {
        GameRules gRules;

        // Spawn penalty text
        std::string penaltyMsg = std::to_string(-gRules.SCORE_PENALTY);
        m_penaltyTexts.push_back(std::shared_ptr<PenaltyText>(new PenaltyText(m_gFontMedium.get(), {4, 4}, penaltyMsg)));

        // Generate shake
        m_shakeGen.generateShake();
    }

    // Update shake generator
    m_shakeGen.update();

    // Update hole manager
    m_holeMgr->update();

    // Spawn particles
    if (m_tmrSpawnParticle.getTicks() > m_delaySpawnParticle)
    {
        m_tmrSpawnParticle.stop();
        m_tmrSpawnParticle.start();

        if (m_particles.size() < m_MAX_PARTICLES)
        {
            GameColors gColors;
            const int xPos = (rand() % (winData.WIDTH / winData.PXSCALE)) * winData.PXSCALE; // generate rand number divisible by PXSCALE
            const int yPos = rand() % static_cast<int>(winData.HEIGHT * 0.65) + static_cast<int>(winData.HEIGHT * 0.45);

            std::shared_ptr<Particle> particle(new Particle({xPos, yPos, winData.PXSCALE, winData.PXSCALE}, {0, -1}, gColors.WHITE));
            m_particles.push_back(particle);
        }
    }

    // Update particles
    for (auto iter = m_particles.begin(); iter != m_particles.end();)
    {
        (*iter)->update();
        if ((*iter)->getPos().y <= winData.OUT_OF_BOUNDS)
            iter = m_particles.erase(iter);
        else
            iter++;
    }

    // Update penalty texts
    for (auto iter = m_penaltyTexts.begin(); iter != m_penaltyTexts.end();)
    {
        (*iter)->update();
        if ((*iter)->fetchAlpha() == 0)
            iter = m_penaltyTexts.erase(iter);
        else
            iter++;
    }
}
void PlayScene::draw()
{
    GameColors gColors;
    WindowMetadata winDat;

    SDL_SetRenderDrawColor(gameRenderer, 200, 200, 0, 255);
    SDL_RenderClear(gameRenderer);

    targetTexture->setAsTarget();

    auraBGTexture->draw();

    // Draw particles
    for (std::shared_ptr<Particle> particle : m_particles)
    {
        particle->draw();
    }

    cityBGTexture->draw();
    btnToMenu->draw();

    // Draw holes
    for (std::shared_ptr<Hole> hole : m_holes)
    {
        hole->draw();
    }

    std::string msgScore = std::to_string(m_score);
    m_gFontMedium->draw(msgScore, {winDat.HEIGHT / 2, 50}, gColors.WHITE, PosCentering::POSCEN_X);

    // Draw penaltyTexts
    for (std::shared_ptr<PenaltyText> pentxt : m_penaltyTexts)
    {
        pentxt->draw();
    }

    SDL_Point displacement = m_shakeGen.fetchDisplacement();
    SDL_Rect targRect = {displacement.x, displacement.y, winDat.WIDTH, winDat.HEIGHT};

    targetTexture->unsetAsTarget();
    targetTexture->draw(NULL, &targRect);

    SDL_RenderPresent(gameRenderer);
}
void PlayScene::loadAssets()
{
    // Load images
    cityBGTexture = std::unique_ptr<GTexture>(new GTexture());
    cityBGTexture->loadFromFile("assets/images/city_bg.png");

    auraBGTexture = std::unique_ptr<GTexture>(new GTexture());
    auraBGTexture->loadFromFile("assets/images/aura_bg.png");

    uiElementsTexture = std::unique_ptr<GTexture>(new GTexture());
    uiElementsTexture->loadFromFile("assets/images/ui_elements.png");

    holeSheetTexture = std::unique_ptr<GTexture>(new GTexture());
    holeSheetTexture->loadFromFile("assets/images/holeSheet.png");

    // Load fonts
    m_gFontMedium = std::unique_ptr<GFont>(new GFont());
    m_gFontMedium->loadFontFromFile("assets/fonts/04B03.TTF", 48);
}
void PlayScene::createButtons()
{
    WindowMetadata winData;

    // --- btnToMenu --
    const int btnToMenu_rw = 8 * winData.PXSCALE;
    const int btnToMenu_rh = 8 * winData.PXSCALE;
    const int btnToMenu_rx = (winData.WIDTH - winData.PXSCALE) - btnToMenu_rw;
    const int btnToMenu_ry = winData.PXSCALE;
    SDL_Rect btnToMenu_rect = {btnToMenu_rx, btnToMenu_ry, btnToMenu_rw, btnToMenu_rh};
    std::map<BtnState, SDL_Rect> btnToMenu_clips;
    btnToMenu_clips[BST_NORMAL] = {0, 16, 8, 8};
    btnToMenu_clips[BST_HOVERED] = {8, 16, 8, 8};
    btnToMenu = std::make_unique<GButton>(GButton(uiElementsTexture.get(), btnToMenu_rect, btnToMenu_clips));
    btnToMenu->bindCallback(std::bind(&PlayScene::cbToMenu, this));
}
void PlayScene::initializeTimers()
{
    m_tmrWarmup.start();
    m_tmrSpawnParticle.start();
}
void PlayScene::createHoles()
{
    WindowMetadata winData;
    const int cwx = winData.WIDTH / 2;
    const int cwy = winData.HEIGHT / 2;
    const int offy_row1 = winData.PXSCALE * 4;
    const int offy_row2 = winData.PXSCALE * 20;
    const int offx_col1 = winData.PXSCALE * 20;
    const int offx_col2 = winData.PXSCALE * 40;

    std::shared_ptr<Hole> hole1(new Hole(holeSheetTexture, {cwx - offx_col2, cwy + offy_row1}, PosCentering::POSCEN_BOTH));
    std::shared_ptr<Hole> hole2(new Hole(holeSheetTexture, {cwx - offx_col1, cwy + offy_row1}, PosCentering::POSCEN_BOTH));
    std::shared_ptr<Hole> hole3(new Hole(holeSheetTexture, {cwx, cwy + offy_row1}, PosCentering::POSCEN_BOTH));
    std::shared_ptr<Hole> hole4(new Hole(holeSheetTexture, {cwx + offx_col1, cwy + offy_row1}, PosCentering::POSCEN_BOTH));
    std::shared_ptr<Hole> hole5(new Hole(holeSheetTexture, {cwx + offx_col2, cwy + offy_row1}, PosCentering::POSCEN_BOTH));

    std::shared_ptr<Hole> hole6(new Hole(holeSheetTexture, {cwx - offx_col2, cwy + offy_row2}, PosCentering::POSCEN_BOTH));
    std::shared_ptr<Hole> hole7(new Hole(holeSheetTexture, {cwx - offx_col1, cwy + offy_row2}, PosCentering::POSCEN_BOTH));
    std::shared_ptr<Hole> hole8(new Hole(holeSheetTexture, {cwx, cwy + offy_row2}, PosCentering::POSCEN_BOTH));
    std::shared_ptr<Hole> hole9(new Hole(holeSheetTexture, {cwx + offx_col1, cwy + offy_row2}, PosCentering::POSCEN_BOTH));
    std::shared_ptr<Hole> hole10(new Hole(holeSheetTexture, {cwx + offx_col2, cwy + offy_row2}, PosCentering::POSCEN_BOTH));

    m_holes.push_back(hole1);
    m_holes.push_back(hole2);
    m_holes.push_back(hole3);
    m_holes.push_back(hole4);
    m_holes.push_back(hole5);
    m_holes.push_back(hole6);
    m_holes.push_back(hole7);
    m_holes.push_back(hole8);
    m_holes.push_back(hole9);
    m_holes.push_back(hole10);
}
void PlayScene::initializeHoleMgr()
{
    m_holeMgr = std::unique_ptr<HoleManager>(new HoleManager(m_holes));
}
void PlayScene::initializeCollisionMgr()
{
    m_collideMgr = std::unique_ptr<HoleCollisionManager>(new HoleCollisionManager(m_holes, &m_gMouse, &m_score));
}
void PlayScene::initializeTargetTexture()
{
    WindowMetadata winDat;
    targetTexture = std::unique_ptr<GTexture>(new GTexture());
    targetTexture->loadAsTarget({0, 0, winDat.WIDTH, winDat.HEIGHT});
}
// *** CALLBACKS ***
void PlayScene::cbToMenu()
{
    m_stflag = STF_TOMENU;
}
#pragma endregion PlayScene