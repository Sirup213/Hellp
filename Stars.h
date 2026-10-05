#ifndef STARS_H

#define STARS_H

#include<raylib.h>
#include "Hellp.h"

class Stars{
private:
    Vector2 pos;
    float scrollspeed;
    int texture_num;
    bool Far;
    bool fast;
public:
    void spawn();
    void update(float dt);
    void choose();
    void draw(Texture2D star_texture[StarTextures]);
};

#endif