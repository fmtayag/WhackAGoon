#ifndef WIDGETS_H
#define WIDGETS_H
#pragma once

#include <SDL2/SDL.h>

enum BtnEvent {
	BEVNT_CLICK,
	BEVNT_UNCLICK,
	BEVNT_HOVER,
	BEVNT_UNHOVER
};

enum BtnState {
	BST_INACTIVE,
	BST_ACTIVE,
	BST_CLICKED,
	BST_HOVERED
};

class Button {
	public:
		Button(SDL_Texture* btnTexture, std::string btnText, int btnX, int btnY);
		~Button();
		void update(BtnEvent event);
		void draw(SDL_Renderer* renderer);
		
		SDL_Rect getRect() { return m_rect; };
		
	private:
		SDL_Rect m_rect;
		SDL_Texture* m_texture;
		std::string m_text;
		BtnState m_state;
	
};

#endif // WIDGETS_H