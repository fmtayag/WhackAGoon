#ifndef RENDERDATA_H
#define RENDERDATA_H

#include <memory>
#include <SDL.h>
#include "widgets.h"

extern SDL_Window *gameWindow;
extern SDL_Renderer *gameRenderer;
extern std::shared_ptr<GTexture> targetTexture;

#endif // RENDERDATA_H