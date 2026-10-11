#ifndef BULLET_H

#define BULLET_H

#include <raylib.h>
#include "Hellp.h"

class Bullet{
private:
    Vector2 position;
    Vector2 pastpos;
    Vector2 velocity;
    Texture2D texture;
    float radius;
    bool disabled; /*i Am being Inclusive*/
    bool spawned;
    Color color;
public:
    Bullet(){}
    ~Bullet(){}
    bool GetDisabled();
    void spawntop(int &bulletcount, Texture2D texture, Vector2 Circle, bool targeted = false, bool active = false);
    void draw();
    void update(int &bulletdisabled, float dt, Player &player);
    void trail(int stepsize , int length = 4, int skip = 1);
    void swapdata(Bullet &bullet);
};

#endif