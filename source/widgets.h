#ifndef WIDGETS_H
#define WIDGETS_H
#pragma once

#include <functional>
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
	Button(SDL_Texture *btnTexture, std::string btnText, int btnW, int btnH, int btnX, int btnY);
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

	// Callback
	std::function<void()> m_callback;
	void makeCallback();
};

class DecrementText
{
public:
	DecrementText(std::string text, SDL_Rect rect, SDL_Point velocity);
	~DecrementText();
	void update();
	void draw(SDL_Renderer *renderer);

	void initTimer();
	void makeTransparent();
	void move();
	void checkVisible();
	bool getf_dead() { return m_flag_dead; };

private:
	std::string m_text;
	short int m_opacity = 255;
	const short int OPAC_STEP = -10;
	SDL_Rect m_rect;
	SDL_Point m_velocity;
	bool m_flag_dead = false;

	// timers
	int tmr_text = 0;
	const short int DUR_TEXT = 50;
};

#endif // WIDGETS_H