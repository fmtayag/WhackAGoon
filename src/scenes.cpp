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
#include "renderdata.h"

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
    // Debug print
    std::string dbgMsg = fmt::format("{} constructor called.", typeid(*this).name());
    dbgPrint(DPL::DEBUG, dbgMsg);

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

    // Constrict the mouse position within the native window size.
    WindowMetadata winDat;
    m_gMouse.position.x = m_gMouse.position.x / winDat.PXSCALE;
    m_gMouse.position.y = m_gMouse.position.y / winDat.PXSCALE;
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
    SDL_Rect logoRect;
    logoRect.w = 80;
    logoRect.h = 50;
    logoRect.x = (winData.NATIVE_WIDTH / 2) - (logoRect.w / 2);
    logoRect.y = 0;

    targetTexture->setAsTarget();

    SDL_SetRenderDrawColor(gameRenderer, 0, 0, 0, 255);
    SDL_RenderClear(gameRenderer);

    brickBGTexture->draw();
    logoTexture->draw(NULL, &logoRect);
    btnPlay->draw();
    btnHelp->draw();
    btnInfo->draw();

    targetTexture->unsetAsTarget();
    targetTexture->draw();

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
    int btnW = 16;
    int btnH = 16;
    int cx = (winData.NATIVE_WIDTH / 2) - (btnW / 2);

    // --- btnPlay --
    SDL_Rect btnPlay_rect = {cx, static_cast<int>(winData.NATIVE_HEIGHT * 0.70), btnW, btnH};
    std::map<BtnState, SDL_Rect> btnPlay_clips;

    btnPlay_clips[BST_NORMAL] = {0, 24, 16, 16};
    btnPlay_clips[BST_HOVERED] = {16, 24, 16, 16};
    btnPlay = std::make_unique<GButton>(GButton(uiElementsTexture, btnPlay_rect, btnPlay_clips));
    btnPlay->bindCallback(std::bind(&MenuScene::cbPlay, this));

    // --- btnHelp ---
    SDL_Rect btnHelp_rect = {static_cast<int>(cx - (btnW * 1.5)), static_cast<int>(winData.NATIVE_HEIGHT * 0.75), btnW, btnH};
    std::map<BtnState, SDL_Rect> btnHelp_clips;
    btnHelp_clips[BST_NORMAL] = {32, 24, 16, 16};
    btnHelp_clips[BST_HOVERED] = {48, 24, 16, 16};
    btnHelp = std::make_unique<GButton>(GButton(uiElementsTexture, btnHelp_rect, btnHelp_clips));
    btnHelp->bindCallback(std::bind(&MenuScene::cbHelp, this));

    // --- btnInfo ---
    SDL_Rect btnInfo_rect = {static_cast<int>(cx + (btnW * 1.5)), static_cast<int>(winData.NATIVE_HEIGHT * 0.75), btnW, btnH};
    std::map<BtnState, SDL_Rect> btnInfo_clips;
    btnInfo_clips[BST_NORMAL] = {0, 56, 16, 16};
    btnInfo_clips[BST_HOVERED] = {16, 56, 16, 16};
    btnInfo = std::make_unique<GButton>(GButton(uiElementsTexture, btnInfo_rect, btnInfo_clips));
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
    // Debug print
    std::string dbgMsg = fmt::format("{} constructor called.", typeid(*this).name());
    dbgPrint(DPL::DEBUG, dbgMsg);

    resetMouseState();
    loadAssets();
    createButtons();
    initializeTimers();
    createHoles();
    initializeHoleMgr();
    initializeCollisionMgr();
    initializeTimerBar();
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
                m_timerBar->enable();
                m_tmrAntiIdle.start();
                break;
            case SDLK_2:
                m_timerBar->disable();
                m_tmrAntiIdle.stop();
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
    // Constrict the mouse position within the native window size.
    WindowMetadata winDat;
    m_gMouse.position.x = m_gMouse.position.x / winDat.PXSCALE;
    m_gMouse.position.y = m_gMouse.position.y / winDat.PXSCALE;
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
        m_penaltyTexts.push_back(std::shared_ptr<PenaltyText>(new PenaltyText(m_gFontMedium, {4, 4}, penaltyMsg)));

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

            std::shared_ptr<Particle> particle(new Particle({xPos, yPos, winData.PXSCALE, winData.PXSCALE}, {0, -1}, gColors.NEON_TOXICGREEN));
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

    // Update timer bar
    m_timerBar->update(m_tmrAntiIdle.getTicks(), m_delayAntiIdle);
}
void PlayScene::draw()
{
    GameColors gColors;
    WindowMetadata winDat;

    targetTexture->setAsTarget();
    SDL_SetRenderDrawColor(gameRenderer, 200, 200, 0, 255);
    SDL_RenderClear(gameRenderer);

    m_auraBGTexture->draw();

    // Draw particles
    for (std::shared_ptr<Particle> particle : m_particles)
    {
        particle->draw();
    }

    m_cityhallBGTexture->draw();
    btnToMenu->draw();

    // Draw holes
    for (std::shared_ptr<Hole> hole : m_holes)
    {
        hole->draw();
    }

    std::string msgLabel = "SCORE";
    std::string msgScore = std::to_string(m_score);
    m_gFontMedium->draw(msgLabel, {16, 2}, gColors.WHITE, PosCentering::POSCEN_X);
    m_gFontMedium->draw(msgScore, {16, 8}, gColors.WHITE, PosCentering::POSCEN_X);

    // Draw penaltyTexts
    for (std::shared_ptr<PenaltyText> pentxt : m_penaltyTexts)
    {
        pentxt->draw();
    }

    // Draw timer bar
    m_timerBar->draw();

    targetTexture->unsetAsTarget();

    // Render target texture
    SDL_Point displacement = m_shakeGen.fetchDisplacement();
    SDL_Rect targRect = {displacement.x, displacement.y, winDat.TARG_WIDTH, winDat.TARG_HEIGHT};
    targetTexture->draw(NULL, &targRect);

    SDL_RenderPresent(gameRenderer);
}
// *** INITIALIZERS ***
void PlayScene::loadAssets()
{
    WindowMetadata winDat;

    // Load images
    m_cityhallBGTexture = std::unique_ptr<GTexture>(new GTexture());
    m_cityhallBGTexture->loadFromFile("assets/images/cityhall_bg.png");

    m_auraBGTexture = std::unique_ptr<GTexture>(new GTexture());
    m_auraBGTexture->loadFromFile("assets/images/aura_bg.png");

    m_uiElementsTexture = std::unique_ptr<GTexture>(new GTexture());
    m_uiElementsTexture->loadFromFile("assets/images/ui_elements.png");

    m_holeSheetTexture = std::unique_ptr<GTexture>(new GTexture());
    m_holeSheetTexture->loadFromFile("assets/images/holeSheet.png");

    // Load fonts
    m_gFontMedium = std::unique_ptr<GFont>(new GFont());
    m_gFontMedium->loadFontFromFile("assets/fonts/04B03.TTF", 8);
}
void PlayScene::createButtons()
{
    WindowMetadata winData;

    // --- btnToMenu --
    const int btnToMenu_rw = 8;
    const int btnToMenu_rh = 8;
    const int btnToMenu_rx = (winData.NATIVE_WIDTH) - btnToMenu_rw - 1;
    const int btnToMenu_ry = 1;

    SDL_Rect btnToMenu_rect = {btnToMenu_rx, btnToMenu_ry, btnToMenu_rw, btnToMenu_rh};
    std::map<BtnState, SDL_Rect> btnToMenu_clips;
    btnToMenu_clips[BST_NORMAL] = {0, 16, 8, 8};
    btnToMenu_clips[BST_HOVERED] = {8, 16, 8, 8};
    btnToMenu = std::unique_ptr<GButton>(new GButton(m_uiElementsTexture, btnToMenu_rect, btnToMenu_clips));
    btnToMenu->bindCallback(std::bind(&PlayScene::cbToMenu, this));
}
void PlayScene::initializeTimers()
{
    m_tmrWarmup.start();
    m_tmrSpawnParticle.start();
    m_tmrAntiIdle.start();
}
void PlayScene::createHoles()
{
    WindowMetadata winData;
    const int cwx = winData.NATIVE_WIDTH / 2;
    const int cwy = static_cast<int>(winData.NATIVE_HEIGHT * 0.50f);
    const int offy_row1 = winData.PXSCALE * 1;
    const int offy_row2 = winData.PXSCALE * 2;
    const int offy_row3 = winData.PXSCALE * 4;
    const int offx_col1 = winData.PXSCALE * 3;

    std::shared_ptr<Hole> hole1(new Hole(m_holeSheetTexture, {cwx - offx_col1, cwy + offy_row1}, PosCentering::POSCEN_BOTH));
    std::shared_ptr<Hole> hole2(new Hole(m_holeSheetTexture, {cwx, cwy + offy_row1}, PosCentering::POSCEN_BOTH));
    std::shared_ptr<Hole> hole3(new Hole(m_holeSheetTexture, {cwx + offx_col1, cwy + offy_row1}, PosCentering::POSCEN_BOTH));

    std::shared_ptr<Hole> hole4(new Hole(m_holeSheetTexture, {cwx - offx_col1, cwy + offy_row2}, PosCentering::POSCEN_BOTH));
    std::shared_ptr<Hole> hole5(new Hole(m_holeSheetTexture, {cwx, cwy + offy_row2}, PosCentering::POSCEN_BOTH));
    std::shared_ptr<Hole> hole6(new Hole(m_holeSheetTexture, {cwx + offx_col1, cwy + offy_row2}, PosCentering::POSCEN_BOTH));

    std::shared_ptr<Hole> hole7(new Hole(m_holeSheetTexture, {cwx - offx_col1, cwy + offy_row3}, PosCentering::POSCEN_BOTH));
    std::shared_ptr<Hole> hole8(new Hole(m_holeSheetTexture, {cwx, cwy + offy_row3}, PosCentering::POSCEN_BOTH));
    std::shared_ptr<Hole> hole9(new Hole(m_holeSheetTexture, {cwx + offx_col1, cwy + offy_row3}, PosCentering::POSCEN_BOTH));

    m_holes.push_back(hole1);
    m_holes.push_back(hole2);
    m_holes.push_back(hole3);
    m_holes.push_back(hole4);
    m_holes.push_back(hole5);
    m_holes.push_back(hole6);
    m_holes.push_back(hole7);
    m_holes.push_back(hole8);
    m_holes.push_back(hole9);
}
void PlayScene::initializeHoleMgr()
{
    m_holeMgr = std::unique_ptr<HoleManager>(new HoleManager(m_holes));
}
void PlayScene::initializeCollisionMgr()
{
    m_collideMgr = std::unique_ptr<HoleCollisionManager>(new HoleCollisionManager(m_holes, &m_gMouse, &m_score));
}
void PlayScene::initializeTimerBar()
{
    WindowMetadata winDat;

    CSize timerBarSize = {40, 3};
    std::vector<SDL_Rect> timerBarClips = {
        {32, 8, 8, 8},
        {40, 8, 8, 8},
        {48, 8, 8, 8},
        {56, 8, 8, 8}};
    m_timerBar = std::unique_ptr<TimerBar>(new TimerBar(m_uiElementsTexture, timerBarSize, timerBarClips));
}
// *** CALLBACKS ***
void PlayScene::cbToMenu()
{
    m_stflag = STF_TOMENU;
}
#pragma endregion PlayScene

#pragma region DebugScene
DebugScene::DebugScene()
{
    resetMouseState();
    loadAssets();
    createHoles();
}
DebugScene::~DebugScene()
{
}
void DebugScene::handleEvents(SDL_Event *e)
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
            case SDLK_1:
                m_holes[0]->awaken(Hole::HoleType::HT_GOON);
                break;
            }
        }
    }

    SDL_GetMouseState(&m_gMouse.position.x, &m_gMouse.position.y);

    // Constrict the mouse position within the native window size.
    WindowMetadata winDat;
    m_gMouse.position.x = m_gMouse.position.x / winDat.PXSCALE;
    m_gMouse.position.y = m_gMouse.position.y / winDat.PXSCALE;
    dbgPrint(DPL::DEBUG, fmt::format("mouse.x: {}, mouse.y: {}", m_gMouse.position.x, m_gMouse.position.y));
}
void DebugScene::update()
{
    // Update holes
    for (std::shared_ptr<Hole> hole : m_holes)
    {
        hole->update(m_gMouse);
    }
}
void DebugScene::draw()
{
    WindowMetadata winDat;
    m_targetTexture->setAsTarget();
    SDL_SetRenderDrawColor(gameRenderer, 200, 200, 0, 255);
    SDL_RenderClear(gameRenderer);

    m_cityhallBGTexture->draw();

    // Draw holes
    for (std::shared_ptr<Hole> hole : m_holes)
    {
        hole->draw();
    }

    m_targetTexture->unsetAsTarget();

    SDL_Rect targdst = {0, 0, winDat.WIDTH, winDat.HEIGHT};
    m_targetTexture->draw(NULL, &targdst);

    SDL_RenderPresent(gameRenderer);
}
// *** INITIALIZERS ***
void DebugScene::loadAssets()
{
    WindowMetadata winDat;

    m_holeSheetTexture = std::make_shared<GTexture>(GTexture());
    m_holeSheetTexture->loadFromFile("assets/images/holeSheet.png");

    m_cityhallBGTexture = std::make_unique<GTexture>(GTexture());
    m_cityhallBGTexture->loadFromFile("assets/images/cityhall_bg.png");

    m_targetTexture = std::make_unique<GTexture>(GTexture());
    m_targetTexture->loadAsTarget({0, 0, 100, 100});
}
void DebugScene::createHoles()
{
    WindowMetadata winData;
    const int cwx = winData.NATIVE_WIDTH / 2;
    const int cwy = static_cast<int>(winData.NATIVE_HEIGHT * 0.50f);
    const int offy_row1 = winData.PXSCALE * 1;
    const int offy_row2 = winData.PXSCALE * 2;
    const int offy_row3 = winData.PXSCALE * 3;
    const int offx_col1 = winData.PXSCALE * 2;

    std::shared_ptr<Hole> hole1(new Hole(m_holeSheetTexture, {cwx - offx_col1, cwy + offy_row1}, PosCentering::POSCEN_BOTH));
    std::shared_ptr<Hole> hole2(new Hole(m_holeSheetTexture, {cwx, cwy + offy_row1}, PosCentering::POSCEN_BOTH));
    std::shared_ptr<Hole> hole3(new Hole(m_holeSheetTexture, {cwx + offx_col1, cwy + offy_row1}, PosCentering::POSCEN_BOTH));

    std::shared_ptr<Hole> hole4(new Hole(m_holeSheetTexture, {cwx - offx_col1, cwy + offy_row2}, PosCentering::POSCEN_BOTH));
    std::shared_ptr<Hole> hole5(new Hole(m_holeSheetTexture, {cwx, cwy + offy_row2}, PosCentering::POSCEN_BOTH));
    std::shared_ptr<Hole> hole6(new Hole(m_holeSheetTexture, {cwx + offx_col1, cwy + offy_row2}, PosCentering::POSCEN_BOTH));

    std::shared_ptr<Hole> hole7(new Hole(m_holeSheetTexture, {cwx - offx_col1, cwy + offy_row3}, PosCentering::POSCEN_BOTH));
    std::shared_ptr<Hole> hole8(new Hole(m_holeSheetTexture, {cwx, cwy + offy_row3}, PosCentering::POSCEN_BOTH));
    std::shared_ptr<Hole> hole9(new Hole(m_holeSheetTexture, {cwx + offx_col1, cwy + offy_row3}, PosCentering::POSCEN_BOTH));

    m_holes.push_back(hole1);
    m_holes.push_back(hole2);
    m_holes.push_back(hole3);
    m_holes.push_back(hole4);
    m_holes.push_back(hole5);
    m_holes.push_back(hole6);
    m_holes.push_back(hole7);
    m_holes.push_back(hole8);
    m_holes.push_back(hole9);
}
#pragma endregion DebugScene