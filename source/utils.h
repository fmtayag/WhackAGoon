#ifndef UTILS_H
#define UTILS_H
#pragma once

SDL_Texture* loadTextureFromFile(SDL_Renderer* renderer, std::string path);
void cleanUpTexture(SDL_Texture*& texture);

#endif // UTILS_H
