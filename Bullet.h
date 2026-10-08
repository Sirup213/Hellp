#ifndef BULLET_H

#define BULLET_H

#include <raylib.h>
#include "Hellp.h"

class Bullet{
private:
    Vector2 position;
    Vector2 velocity;
    Texture2D texture;
    float radius;
    bool disabled; /*i Am being Inclusive*/
    bool spawned;
    Color color;
public:
    bool GetDisabled();
    void spawntop(int &bulletcount, Texture2D texture, Vector2 Circle, bool targeted = false);
    void draw();
    void update(float dt, Vector2 Circle, bool immune, bool& alive);
};

#endif