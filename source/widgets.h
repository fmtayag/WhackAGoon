#ifndef WIDGETS_H
#define WIDGETS_H
#pragma once

#include <functional>
#include <list>
#include <SDL2/SDL.h>

enum BtnEvent
{
	BEVNT_CLICK,
	BEVNT_UNCLICK,
	BEVNT_HOVER,
	BEVNT_UNHOVER
};

enum BtnState
{
	BST_DISABLED,
	BST_NORMAL,
	BST_CLICKED,
	BST_HOVERED
};

struct MouseState
{
	bool isClicked;
	SDL_Point pos;
};

class Button
{
public:
	Button(SDL_Texture *btnTexture, SDL_Rect rect, std::map<BtnState, SDL_Rect> clips);
	~Button();
	void update(MouseState *mouse_s);
	void draw(SDL_Renderer *renderer);

	SDL_Rect getRect() { return m_rect; };
	void setState(BtnState state);
	void bindCallback(std::function<void()> cback);

private:
	// update() sub-methods
	void u_state(MouseState *mouse_s);

	SDL_Rect m_rect;
	SDL_Texture *m_texture;
	std::string m_text;
	BtnState m_state;
	std::map<BtnState, SDL_Rect> m_clips;

	// Callback
	std::function<void()> m_callback;
	void makeCallback();
};

class FadeText
{
public:
	FadeText(std::string text, SDL_Rect rect, SDL_Point velocity);
	~FadeText();
	void update();
	void draw(SDL_Renderer *renderer);

	bool isVisible();

private:
	void initTimer();
	void makeTransparent();
	void move();

	std::string m_text;
	short int m_opacity = 255;
	static const short int OPAC_STEP = -20;
	SDL_Rect m_rect;
	SDL_Point m_velocity;

	// timers
	int tmr_text = 0;
	static const short int DUR_TEXT = 50;
};

#endif // WIDGETS_H