#include <iostream>
#include <functional>
#include <map>
#include <cassert>
#include <algorithm>
#include <vector>
#include <fmt/format.h>
#include <SDL.h>
#include "sprites.h"
#include "g_data.h"
#include "helpers.h"
#include "renderdata.h"

#pragma region GButton
//{ GButton
GButton::GButton(std::shared_ptr<GTexture> texture, SDL_Rect rect, std::map<BtnState, SDL_Rect> clips)
    : m_texture(texture)
{
    dbgPrint(DPL::DEBUG, fmt::format("{} constructor called.", typeid(*this).name()));

    m_rect.x = rect.x;
    m_rect.y = rect.y;
    m_rect.w = rect.w;
    m_rect.h = rect.h;
    m_state = BST_NORMAL;
    m_clips = clips;

    WindowMetadata winDat;

    if (m_rect.w <= 0)
        m_rect.w = m_clips[m_state].w * winDat.PXSCALE;
    if (m_rect.h <= 0)
        m_rect.h = m_clips[m_state].h * winDat.PXSCALE;
}
GButton::~GButton()
{
    std::string dbgMsg = fmt::format("{} destructor called.", typeid(*this).name());
    dbgPrint(DPL::DEBUG, dbgMsg);
}
void GButton::update(GameMouse *mouse_s)
{
    if (this != nullptr)
    {
        u_state(mouse_s);

        if (m_state == BST_CLICKED)
        {
            makeCallback();
            m_state = BST_NORMAL;
            mouse_s->isClicked = false;
        }
    }
}
void GButton::u_state(GameMouse *mouse_s)
{
    bool collided = isPointCollide(mouse_s->position, this->getRect());
    if (collided)
    {
        this->setState(BST_HOVERED);

        if (mouse_s->isClicked)
        {
            this->setState(BST_CLICKED);
        }
    }
    else
    {
        this->setState(BST_NORMAL);
    }
}
void GButton::draw()
{
    if (this != nullptr)
    {
        m_texture->draw(&m_clips[m_state], &m_rect);
    }
}
void GButton::setState(BtnState state)
{
    if (m_state != BST_DISABLED)
    {
        m_state = state;
    }
}
void GButton::bindCallback(std::function<void()> cback)
{
    m_callback = cback;
}
void GButton::makeCallback()
{
    if (m_callback != NULL)
    {
        m_callback();
    }
}
//}
#pragma endregion GButton

#pragma region Particle
Particle::Particle(SDL_Rect rect, Vector2 velocity, SDL_Color color)
{
    dbgPrint(DPL::DEBUG, fmt::format("{} constructor called.", typeid(*this).name()));

    m_rect = rect;
    m_velocity = velocity;
    m_color = color;
}
Particle::~Particle()
{
    std::string dbgMsg = fmt::format("{} destructor called.", typeid(*this).name());
    dbgPrint(DPL::DEBUG, dbgMsg);
}
void Particle::update()
{
    m_rect.x += m_velocity.x;
    m_rect.y += m_velocity.y;
}
void Particle::draw()
{
    SDL_SetRenderDrawColor(gameRenderer, m_color.r, m_color.g, m_color.b, m_color.a);
    SDL_RenderFillRect(gameRenderer, &m_rect);
}
SDL_Point Particle::getPos()
{
    SDL_Point pos;
    pos.x = m_rect.x;
    pos.y = m_rect.y;

    return pos;
}
#pragma endregion Particle

#pragma region Hole
Hole::Hole(std::shared_ptr<GTexture> texture, SDL_Point pos, PosCentering poscenter)
{
    dbgPrint(DPL::DEBUG, fmt::format("{} constructor called.", typeid(*this).name()));

    WindowMetadata winData;
    m_texture = texture;

    m_rect.x = pos.x;
    m_rect.y = pos.y;
    m_rect.w = 16;
    m_rect.h = 16;

    // Check for position centers
    switch (poscenter)
    {
    case PosCentering::POSCEN_NONE:
        // Do nothing
        break;
    case PosCentering::POSCEN_X:
        m_rect.x = (pos.x / 2) - (m_rect.w / 2);
        break;
    case PosCentering::POSCEN_Y:
        m_rect.y = pos.y - (m_rect.h / 2);
        break;
    case PosCentering::POSCEN_BOTH:
        m_rect.x = pos.x - (m_rect.w / 2);
        m_rect.y = pos.y - (m_rect.h / 2);
        break;
    default:
        std::string dbgMsg = fmt::format("{}() -> switch case reached default case.", __FUNCTION__);
        dbgPrint(DPL::WARNING, dbgMsg);
        break;
    }

    m_state = HoleState(HT_NONE, HAS_REST);
    m_curF = 0;
    m_tmrNxtF.start();
}
Hole::~Hole()
{
    std::string dbgMsg = fmt::format("{} destructor called.", typeid(*this).name());
    dbgPrint(DPL::DEBUG, dbgMsg);
}
void Hole::update(GameMouse &gMouse)
{
    // --- Collision stuff ---
    setHitStatus(UNSET); // reset hit status
    bool isColliding = isPointCollide(gMouse.position, m_rect);
    if (isColliding)
    {
        if (gMouse.isClicked)
        {
            whack();
            gMouse.isClicked = false;
        }
    }

    // --- Update animation state ---
    if (m_tmrNxtF.getTicks() > m_delayNxtF)
    {
        // Reset timer
        m_tmrNxtF.stop();
        m_tmrNxtF.start();

        // Advance or reset current frame
        const int vec_size = static_cast<int>(m_sheetMap[m_state].size());
        if (m_curF < vec_size - 1)
        {
            m_curF++;
        }
        else
        {
            nextAS();
            m_curF = 0;
        }
    }

    // Awake->ToRest does not depend on the animation frames finishing.
    if (m_state.second == HAS_AWAKE)
    {
        if (m_tmrAwake.getTicks() > m_delayAwake)
        {
            m_tmrAwake.stop();
            m_state.second = HAS_TOREST;
            normalFrameDelay(); // reset next frame delay back to normal
            m_curF = 0;
        }
    }
}
void Hole::draw()
{
    // Draw hole
    m_texture->draw(&m_sheetMap[HoleState(HT_NONE, HAS_REST)][0], &m_rect);

    // Draw person
    m_texture->draw(&m_sheetMap[m_state][m_curF], &m_rect);
}
void Hole::nextAS()
{
    // --- Check for transition to next animation state ---

    if (m_state.second == HAS_TOAWAKE)
    {
        m_state.second = HAS_AWAKE;
        m_tmrAwake.start();

        const int vec_size = static_cast<int>(m_sheetMap[m_state].size());
        const int tick_offset = (m_delayNxtF * vec_size);
        m_delayAwake = (rand() % 2000 + 200) + tick_offset; // Add tick offset to compensate for ticks lost finishing the "TO AWAKE" animation state.

        slowFrameDelay(); // set next frame delay to slow, otherwise the awake animation will look silly
    }
    else if (m_state.second == HAS_WHACKED)
    {
        // Don't transition to "TO REST" state until animation has played MAX_WHACKED_LOOP times.
        if (whacked_loop >= MAX_WHACKED_LOOP)
        {
            whacked_loop = 0;
            m_state.second = HAS_TOREST;
        }
        else
        {
            whacked_loop++;
        }
    }
    else if (m_state.second == HAS_TOREST)
    {
        m_state.first = HT_NONE;
        m_state.second = HAS_REST;
    }
}
void Hole::awaken(HoleType type)
{
    if (type != HT_NONE)
    {
        if (m_state == HoleState(HT_NONE, HAS_REST))
        {
            m_state.first = type;
            m_state.second = HAS_TOAWAKE;
        }
        else
        {
            dbgPrint(DPL::WARNING, "Cannot awaken Hole because it is not of type HT_NONE, and not of state HAS_REST");
        }
    }
    else
    {
        std::string dbgMsg = fmt::format("Cannot awaken Hole. {}() received HT_NONE as argument.", __FUNCTION__);
        dbgPrint(DPL::WARNING, dbgMsg);
    }
}
void Hole::whack()
{
    if (m_state.second == HAS_AWAKE ||
        (m_state.second == HAS_TOAWAKE && m_curF >= 3) &&
            (m_state.second != HAS_WHACKED))
    {
        m_state.second = HAS_WHACKED;
        m_curF = 0;
        normalFrameDelay(); // reset next frame delay back to normal
        setHitStatus(HIT);
    }
    else
    {
        setHitStatus(MISS);
    }
}
Hole::HoleType Hole::fetchHoleType()
{
    return m_state.first;
}
// *** Create spritesheet map ***
Hole::SheetMap Hole::createSheetMap()
{
    SheetMap smap;

    // Empty hole sprite
    smap[HoleState(HT_NONE, HAS_REST)] = {
        {0, 16, 16, 16}};

    // --- Goon sprites ---
    smap[HoleState(HT_GOON, HAS_TOAWAKE)] = {
        {0, 32, 16, 16},
        {16, 32, 16, 16},
        {32, 32, 16, 16},
        {48, 32, 16, 16},
        {64, 32, 16, 16},
        {80, 32, 16, 16},
        {96, 32, 16, 16},
        {112, 32, 16, 16}};

    smap[HoleState(HT_GOON, HAS_AWAKE)] = {
        {0, 48, 16, 16},
        {16, 48, 16, 16},
        {32, 48, 16, 16},
        {48, 48, 16, 16}};

    smap[HoleState(HT_GOON, HAS_WHACKED)] = {
        {0, 64, 16, 16},
        {16, 64, 16, 16},
        {32, 64, 16, 16},
        {48, 64, 16, 16}};

    // ... just reverse the "to awake" frames for the "to rest" frames.
    smap[HoleState(HT_GOON, HAS_TOREST)] = smap[HoleState(HT_GOON, HAS_TOAWAKE)];
    std::reverse(smap[HoleState(HT_GOON, HAS_TOREST)].begin(), smap[HoleState(HT_GOON, HAS_TOREST)].end());

    // --- Townie sprites ---
    smap[HoleState(HT_TOWNIE, HAS_TOAWAKE)] = {
        {0, 128, 16, 16},
        {16, 128, 16, 16},
        {32, 128, 16, 16},
        {48, 128, 16, 16},
        {64, 128, 16, 16},
        {80, 128, 16, 16},
        {96, 128, 16, 16},
        {112, 128, 16, 16}};

    smap[HoleState(HT_TOWNIE, HAS_AWAKE)] = {
        {0, 144, 16, 16},
        {16, 144, 16, 16},
        {32, 144, 16, 16},
        {48, 144, 16, 16},
        {64, 144, 16, 16},
        {80, 144, 16, 16}};

    smap[HoleState(HT_TOWNIE, HAS_WHACKED)] = {
        {0, 160, 16, 16},
        {16, 160, 16, 16},
        {32, 160, 16, 16},
        {48, 160, 16, 16}};

    // ... just reverse the "to awake" frames for the "to rest" frames.
    smap[HoleState(HT_TOWNIE, HAS_TOREST)] = smap[HoleState(HT_TOWNIE, HAS_TOAWAKE)];
    std::reverse(smap[HoleState(HT_TOWNIE, HAS_TOREST)].begin(), smap[HoleState(HT_TOWNIE, HAS_TOREST)].end());

    // --- Mayor sprites ---
    smap[HoleState(HT_MAYOR, HAS_TOAWAKE)] = {
        {0, 80, 16, 16},
        {16, 80, 16, 16},
        {32, 80, 16, 16},
        {48, 80, 16, 16},
        {64, 80, 16, 16},
        {80, 80, 16, 16},
        {96, 80, 16, 16},
        {112, 80, 16, 16}};

    smap[HoleState(HT_MAYOR, HAS_AWAKE)] = {
        {0, 96, 16, 16},
        {16, 96, 16, 16},
        {32, 96, 16, 16},
        {48, 96, 16, 16}};

    smap[HoleState(HT_MAYOR, HAS_WHACKED)] = {
        {0, 112, 16, 16},
        {16, 112, 16, 16},
        {32, 112, 16, 16},
        {48, 112, 16, 16}};

    // ... just reverse the "to awake" frames for the "to rest" frames.
    smap[HoleState(HT_MAYOR, HAS_TOREST)] = smap[HoleState(HT_MAYOR, HAS_TOAWAKE)];
    std::reverse(smap[HoleState(HT_MAYOR, HAS_TOREST)].begin(), smap[HoleState(HT_MAYOR, HAS_TOREST)].end());

    return smap;
}
Hole::SheetMap Hole::m_sheetMap = Hole::createSheetMap();
#pragma endregion Hole

#pragma region PenaltyText
PenaltyText::PenaltyText(std::shared_ptr<GFont> font, SDL_Point initpos, std::string msg)
{
    dbgPrint(DPL::DEBUG, fmt::format("{} constructor called.", typeid(*this).name()));

    GameColors gColors;

    m_font = font;
    m_msg = msg;
    m_color = gColors.WHITE;
    m_pos = initpos;
    m_velocity = {0, 1};
    m_alpha = 255;
}
PenaltyText::~PenaltyText()
{
    std::string dbgMsg = fmt::format("{} destructor called.", typeid(*this).name());
    dbgPrint(DPL::DEBUG, dbgMsg);
}
void PenaltyText::update()
{
    // Update alpha
    if (m_alpha > 0)
    {
        m_alpha -= ALPHA_DECREASE;
    }

    if (m_alpha <= 0)
    {
        m_alpha = 0;
    }

    // Update position
    m_pos.x += m_velocity.x;
    m_pos.y += m_velocity.y;
}
void PenaltyText::draw()
{
    m_font->drawWithAlpha(m_msg, m_pos, m_color, PosCentering::POSCEN_NONE, m_alpha);
}
Uint8 PenaltyText::fetchAlpha()
{
    return m_alpha;
}
#pragma endregion PenaltyText

#pragma region TimerBar
TimerBar::TimerBar(std::shared_ptr<GTexture> clockTexture, CSize size, std::vector<SDL_Rect> clips)
    : m_clockTexture(clockTexture)
{
    dbgPrint(DPL::DEBUG, fmt::format("{} constructor called.", typeid(*this).name()));

    m_clips = clips;

    WindowMetadata winDat;
    m_rect.x = (winDat.NATIVE_WIDTH / 2) - (m_rect.w / 2);
    m_rect.y = 3;
    m_origWidth = size.w;
    m_rect.w = m_origWidth;
    m_rect.h = size.h;

    m_clockFrame = 0;
    m_tmrAdvanceFrame.start();
    m_state = TimerBar_State::DISABLED;
    m_flickerState = FlickerState::FLICKER_HIDE;
}
TimerBar::~TimerBar()
{
    std::string dbgMsg = fmt::format("{} destructor called.", typeid(*this).name());
    dbgPrint(DPL::DEBUG, dbgMsg);
}
void TimerBar::update(Uint32 timerTicks, Uint32 delayTicks)
{
    if (m_state == TimerBar_State::ENABLED)
    {
        WindowMetadata winDat;

        // *** Flicker effect ***
        if (m_tmrStopFlicker.getTicks() >= FLICKER_DURATION)
        {
            m_tmrStopFlicker.stop();
            m_tmrDoFlicker.stop();
            m_flickerState = FLICKER_SHOW;
        }

        //dbgPrint(DPL::DEBUG, fmt::format("doFlicker: {} delay: {}", m_tmrDoFlicker.getTicks(), FLICKER_DELAY));
        if (m_tmrDoFlicker.getTicks() >= FLICKER_DELAY)
        {
            m_tmrDoFlicker.stop();
            m_tmrDoFlicker.start();
            doFlicker();
        }

        // *** The clock ***
        if (m_tmrAdvanceFrame.getTicks() >= FRAME_DELAY)
        {
            m_tmrAdvanceFrame.stop();
            m_tmrAdvanceFrame.start();

            if (m_clockFrame < m_clips.size() - 1)
                m_clockFrame++;
            else
                m_clockFrame = 0;
        }
        //dbgPrint(DPL::DEBUG, fmt::format("clockFrame: {}", m_clockFrame));

        // *** The bar ***
        // Normalize ticks
        float normalizedTicks = normalizeValue(static_cast<float>(timerTicks), 0.0f, static_cast<float>(delayTicks), true);

        // Update bar length
        float newBarWidth = static_cast<float>(m_origWidth) * normalizedTicks;

        if (newBarWidth <= static_cast<float>(winDat.PXSCALE))
            newBarWidth = 8.0f;

        m_rect.w = static_cast<int>(newBarWidth);

        // Center the bar
        m_rect.x = (winDat.NATIVE_WIDTH / 2) - (m_rect.w / 2);
        //dbgPrint(DPL::DEBUG, fmt::format("m_rect.x: {}", m_rect.x));
        m_rect.y = 3;
    }
}
void TimerBar::draw()
{
    if (m_state == TimerBar_State::ENABLED)
    {
        GameColors gColors;
        WindowMetadata winDat;

        if (m_flickerState == FlickerState::FLICKER_SHOW)
        {
            // *** Draw the bar ***
            SDL_SetRenderDrawColor(gameRenderer, gColors.WHITE.r, gColors.WHITE.g, gColors.WHITE.b, gColors.WHITE.a);
            SDL_RenderFillRect(gameRenderer, &m_rect);

            // So the bar appears rounded ... yeah i know there are better ways to do this but fuck it
            SDL_Rect leftPixelRect = {m_rect.x - 1, m_rect.y + 1, 1, 1};
            SDL_RenderFillRect(gameRenderer, &leftPixelRect);

            SDL_Rect rightPixelRect = {m_rect.x + m_rect.w, m_rect.y + 1, 1, 1};
            SDL_RenderFillRect(gameRenderer, &rightPixelRect);

            // *** Draw the clock
            const int clockW = m_clips[m_clockFrame].w;
            const int clockH = m_clips[m_clockFrame].h;
            const int clockX = ((winDat.NATIVE_WIDTH / 2) - (clockW / 2));
            const int clockY = 1;
            SDL_Rect clockRect = {clockX, clockY, clockW, clockH};
            m_clockTexture->draw(&m_clips[m_clockFrame], &clockRect);
        }
    }
}
void TimerBar::enable()
{
    if (m_state == TimerBar_State::DISABLED)
    {
        m_state = TimerBar_State::ENABLED;
        m_tmrDoFlicker.start();
        m_tmrStopFlicker.start();
    }
}
void TimerBar::disable()
{
    if (m_state == TimerBar_State::ENABLED)
    {
        m_state = TimerBar_State::DISABLED;
    }
}
void TimerBar::doFlicker()
{
    switch (m_flickerState)
    {
    case FlickerState::FLICKER_SHOW:
        m_flickerState = FLICKER_HIDE;
        break;
    case FlickerState::FLICKER_HIDE:
        m_flickerState = FLICKER_SHOW;
        break;
    }
}
#pragma endregion TimerBar