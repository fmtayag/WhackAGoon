#ifndef UTILS_H
#define UTILS_H
#pragma once

SDL_Texture* loadTextureFromFile(SDL_Renderer* renderer, std::string path);
void cleanUpTexture(SDL_Texture*& texture);
bool isPointCollide(int mpos[2], const SDL_Rect* rect);

#endif // UTILS_H
