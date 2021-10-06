#include <iostream>
#include <functional>
#include <SDL2/SDL.h>
#include "widgets.h"
#include "utils.h"
#include "assets.h"

Button::Button(SDL_Texture* btnTexture, std::string btnText, int btnW, int btnH, int btnX, int btnY) {
	m_texture = btnTexture;
	m_text = btnText;
	m_rect.x = btnX;
	m_rect.y = btnY;
	m_rect.w = btnW;
	m_rect.h = btnH;
	m_state = BST_NORMAL;
}

Button::~Button() {
	printf("Deleted button.\n");
}

void Button::update(MouseState* mouse_s) {
	u_state(mouse_s);
	
	if(m_state == BST_CLICKED) {
		makeCallback();
		m_state = BST_NORMAL;
		mouse_s->isClicked = false;
	}
}

void Button::u_state(MouseState* mouse_s) {
	
	bool collided = isPointCollide(mouse_s->pos, this->getRect());
	if(collided) {
		this->setState(BST_HOVERED);
		
		if(mouse_s->isClicked) {
			this->setState(BST_CLICKED);
		}
	}
	else {
		this->setState(BST_NORMAL);
	}
	
}

void Button::draw(SDL_Renderer* renderer) {
	std::string btnText = "";
	
	switch(m_state) {
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

void Button::setState(BtnState state) {
	if(m_state != BST_DISABLED) {
		m_state = state;
	}
}

void Button::bindCallback(std::function<void()> cback) {
	m_callback = cback;
}

void Button::makeCallback() {
	if(m_callback != NULL) {
		m_callback();
	}
}