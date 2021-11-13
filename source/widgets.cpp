#include <iostream>
#include <functional>
#include <map>
#include <SDL2/SDL.h>
#include "widgets.h"
#include "utils.h"
#include "assets.h"
#include "metadata.h"

#pragma region Button
//{ Button
Button::Button(SDL_Texture *btnTexture, SDL_Rect rect, std::map<BtnState, SDL_Rect> clips)
{
	m_texture = btnTexture;
	m_rect.x = rect.x;
	m_rect.y = rect.y;
	m_rect.w = rect.w;
	m_rect.h = rect.h;
	//m_state = BST_NORMAL; // set at header instead
	m_clips = clips;

	if (m_rect.w <= 0)
		m_rect.w = m_clips[m_state].w * PXSCALE;
	if (m_rect.h <= 0)
		m_rect.h = m_clips[m_state].h * PXSCALE;
}

Button::~Button()
{
	printf("Button deleted.\n");
}

void Button::update(MouseState *mouse_s)
{
	u_state(mouse_s);

	if (m_state == BST_CLICKED)
	{
		makeCallback();
		m_state = BST_NORMAL;
		mouse_s->isClicked = false;
	}
}

void Button::u_state(MouseState *mouse_s)
{

	bool collided = isPointCollide(mouse_s->pos, this->getRect());
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

void Button::draw(SDL_Renderer *renderer)
{
	SDL_RenderCopy(renderer, m_texture, &m_clips[m_state], &m_rect);
}

void Button::setState(BtnState state)
{
	if (m_state != BST_DISABLED)
	{
		m_state = state;
	}
}

void Button::bindCallback(std::function<void()> cback)
{
	m_callback = cback;
}

void Button::makeCallback()
{
	if (m_callback != NULL)
	{
		m_callback();
	}
}
//}
#pragma endregion Button

#pragma region FadeText
//{ FadeText
FadeText::FadeText(std::string text, SDL_Rect rect, SDL_Point velocity)
{
	m_text = text;
	m_rect = rect;
	m_velocity = velocity;
	initTimer();
}

FadeText::~FadeText()
{
	printf("Debug: FadeText deleted.\n");
}

void FadeText::update()
{
	int now = SDL_GetTicks();
	if (now - tmr_text > DUR_TEXT)
	{
		makeTransparent();
		move();
	}
}

void FadeText::draw(SDL_Renderer *renderer)
{
	// Good idea: turn this drawing routine into a generic one.
	// Create temp surface
	SDL_Surface *textSurface = TTF_RenderText_Solid(gFont, m_text.c_str(), {255, 255, 255});

	// Create rect
	m_rect.w = textSurface->w;
	m_rect.h = textSurface->h;

	// Create texture
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, textSurface);

	// Free temp surface
	SDL_FreeSurface(textSurface);
	textSurface = NULL;

	// Set alpha
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(texture, m_opacity);

	// Draw texture
	SDL_RenderCopy(renderer, texture, NULL, &m_rect);

	// Destroy texture
	SDL_DestroyTexture(texture);
	texture = NULL;
}

void FadeText::initTimer()
{
	if (tmr_text == 0)
	{
		tmr_text = SDL_GetTicks();
	}
}

void FadeText::makeTransparent()
{
	m_opacity += OPAC_STEP;

	if (m_opacity < 0)
	{
		m_opacity = 0;
	}

	printf("m_opacity: %d\n", m_opacity);
}

void FadeText::move()
{
	m_rect.x += m_velocity.x;
	m_rect.y += m_velocity.y;
}

bool FadeText::isVisible()
{

	if (m_opacity <= 0)
	{
		return false;
	}
	return true;
}

//}
#pragma endregion FadeText