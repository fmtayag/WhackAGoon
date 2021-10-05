#ifndef WIDGETS_H
#define WIDGETS_H
#pragma once

#include <functional>
#include <SDL2/SDL.h>

struct MouseState {
	bool isClicked;
	SDL_Point pos;
};

enum BtnEvent {
	BEVNT_CLICK,
	BEVNT_UNCLICK,
	BEVNT_HOVER,
	BEVNT_UNHOVER
};

enum BtnState {
	BST_DISABLED,
	BST_NORMAL,
	BST_CLICKED,
	BST_HOVERED
};

class Button {
	public:
		Button(SDL_Texture* btnTexture, std::string btnText, int btnW, int btnH, int btnX, int btnY);
		~Button();
		void update(MouseState* mouse_s);
		void draw(SDL_Renderer* renderer);
		
		SDL_Rect getRect() { return m_rect; };
		void setState(BtnState state);
		void bindCallback(std::function<void()> cback);
		
	private:
		// update() sub-methods
		void u_state(MouseState* mouse_s);
	
		SDL_Rect m_rect;
		SDL_Texture* m_texture;
		std::string m_text;
		BtnState m_state;
		
		// Callback
		std::function<void()> m_callback;
		void makeCallback();
	
};

#endif // WIDGETS_H