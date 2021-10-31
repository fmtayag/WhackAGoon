#include <iostream>
#include <functional>
#include <SDL2/SDL.h>
#include "widgets.h"
#include "utils.h"
#include "assets.h"

#pragma region Button
//{ Button
Button::Button(SDL_Texture *btnTexture, std::string btnText, SDL_Rect rect)
{
	m_texture = btnTexture; // deprecate later
	m_text = btnText;
	m_rect.x = rect.x;
	m_rect.y = rect.y;
	m_rect.w = rect.w;
	m_rect.h = rect.h;
	m_state = BST_NORMAL;
}

Button::~Button()
{
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
	// Color
	SDL_Color btn_bgColor;
	SDL_Color btn_fgColor = {100, 100, 100, 255};

	if (m_texture != NULL)
	{
		SDL_RenderCopy(renderer, m_texture, NULL, &m_rect);
	}
	else
	{

		switch (m_state)
		{
		case BST_DISABLED:
			btn_bgColor = {50, 50, 50, 255};
			break;
		case BST_NORMAL:
			btn_bgColor = {0, 0, 0, 0};
			break;
		case BST_CLICKED:
			btn_bgColor = {255, 255, 255, 255};
			break;
		case BST_HOVERED:
			btn_bgColor = {255, 255, 255, 255};
			btn_fgColor = {255, 255, 255, 255};
			break;
		}
		SDL_Texture *targTexture = SDL_CreateTexture(
			renderer,
			SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET,
			m_rect.w,
			m_rect.h);

		SDL_SetTextureBlendMode(targTexture, SDL_BLENDMODE_BLEND);

		// Future reference: (1:14pm) problem with the RenderDrawRect 'spilling' over the entire screen.
		// (1:18pm) well apparently i done goofed up on the DebugScene and accidentally put the
		// SDL_RenderClear() routine BEFORE the SDL_SetRenderDrawColor routine.
		// sometimes...problems have a comically simple solution.

		SDL_SetRenderTarget(renderer, targTexture);
		//SDL_SetRenderDrawColor(renderer, btn_bgColor.r, btn_bgColor.g, btn_bgColor.b, btn_bgColor.a);
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_RenderClear(renderer);
		SDL_SetRenderTarget(renderer, NULL);
		SDL_RenderCopy(renderer, targTexture, NULL, &m_rect);

		SDL_DestroyTexture(targTexture);
		targTexture = NULL;
	}

	// Anchor points
	unsigned int xcent = m_rect.x + (m_rect.w / 2);
	unsigned int ycent = m_rect.y + (m_rect.h / 2);

	drawText(renderer, m_text.c_str(), gFont, xcent, ycent, btn_fgColor, true, true);
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