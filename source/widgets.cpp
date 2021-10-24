#include <iostream>
#include <functional>
#include <SDL2/SDL.h>
#include "widgets.h"
#include "utils.h"
#include "assets.h"

//{ Button
Button::Button(SDL_Texture *btnTexture, std::string btnText, int btnW, int btnH, int btnX, int btnY)
{
	m_texture = btnTexture;
	m_text = btnText;
	m_rect.x = btnX;
	m_rect.y = btnY;
	m_rect.w = btnW;
	m_rect.h = btnH;
	m_state = BST_NORMAL;
}

Button::~Button()
{
	printf("Deleted button.\n");
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
	std::string btnText = "";

	switch (m_state)
	{
	case BST_DISABLED:
		btnText = "INACT";
		break;
	case BST_NORMAL:
		btnText = "NORMAL";
		break;
	case BST_CLICKED:
		btnText = "CLICKED";
		break;
	case BST_HOVERED:
		btnText = "HOVER";
		break;
	}

	SDL_RenderCopy(renderer, m_texture, NULL, &m_rect);

	drawText(renderer, btnText.c_str(), gFont, m_rect.x, m_rect.y, {255, 255, 255});
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

//{ DecrementText
DecrementText::DecrementText(std::string text, SDL_Rect rect, SDL_Point velocity)
{
	m_text = text;
	m_rect = rect;
	m_velocity = velocity;
	initTimer();
}

DecrementText::~DecrementText()
{
	// TODO:
	// Memory leak or some shit here. It crashes some time after the destructor is called.
	// Perhaps it's because of the vector iteration calling a NULL object.
	printf("DEBUG: Deleting DecrementText.\n");
}

void DecrementText::update()
{
	int now = SDL_GetTicks();
	if (now - tmr_text > DUR_TEXT)
	{
		makeTransparent();
		move();
	}

	checkVisible();
}

void DecrementText::draw(SDL_Renderer *renderer)
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

void DecrementText::initTimer()
{
	if (tmr_text == 0)
	{
		tmr_text = SDL_GetTicks();
	}
}

void DecrementText::makeTransparent()
{
	m_opacity += OPAC_STEP;

	if (m_opacity < 0)
	{
		m_opacity = 0;
	}

	printf("m_opacity: %d\n", m_opacity);
}

void DecrementText::move()
{
	m_rect.x += m_velocity.x;
	m_rect.y += m_velocity.y;
}

void DecrementText::checkVisible()
{

	if (m_opacity <= 0)
	{
		delete this;
	}
}

//}