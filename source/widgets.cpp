#include <iostream>
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
}

Button::~Button() {
	
}

void Button::update(BtnEvent event) {
	switch(event) {
		case BEVNT_CLICK:
			break;
		case BEVNT_UNCLICK:
			break;
		case BEVNT_HOVER:
			break;
		case BEVNT_UNHOVER:
			break;
	}
}

void Button::draw(SDL_Renderer* renderer) {
	switch(m_state) {
		case BST_INACTIVE:
			break;
		case BST_ACTIVE:
			break;
		case BST_CLICKED:
			break;
		case BST_HOVERED:
			break;
	}
	
	SDL_RenderCopy(renderer, m_texture, NULL, &m_rect);

	drawText(renderer, m_text.c_str(), gFont, m_rect.x, m_rect.y, {255, 255, 255});
	
}
