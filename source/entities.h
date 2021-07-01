#ifndef ENTITIES_H
#define ENTITIES_H

#include <SDL2/SDL.h>

class AbstractEntity {
public:
    virtual void update()=0;
    virtual void draw(SDL_Renderer*& renderer) = 0;
};

class GoonEntity: public AbstractEntity {
public:
    GoonEntity(SDL_Texture* texture, int x, int y);
    void update();
    void draw(SDL_Renderer*& renderer);

private:
    SDL_Texture* mTexture;
    SDL_Rect mRect;
};

#endif // ENTITIES_H
