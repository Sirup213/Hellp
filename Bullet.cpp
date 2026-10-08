#include <iostream>
#include <random>
#include <raylib.h>
#include <raymath.h>
#include "Bullet.h"

static std::random_device rd;
static std::mt19937 gen;
static std::uniform_real_distribution <float> rndx(0, screenWidth);
static std::uniform_real_distribution <float> y(-10, 0);
static std::normal_distribution <float> speed(210, 10);
static std::normal_distribution <float> ntheta(0, (float)(1.0f/2.0f)*PI);
static std::uniform_real_distribution <float> One(-1,1); 

bool Bullet::GetDisabled(){
    return disabled;
}

void Bullet::spawntop(int &bulletcount, Texture2D tex, Vector2 Circle, bool targeted){
    position = {rndx(gen), y(gen)};

    if(targeted = true){
    velocity = Vector2Normalize(Vector2Subtract(Circle, position));
    velocity = Vector2Rotate(velocity,Clamp(ntheta(gen), -(0.8)*PI, (0.8)*PI));
    velocity = Vector2Scale(velocity, Clamp(speed(gen), 10, 400));
    }
    else{
        velocity = Vector2Scale({Clamp(One(gen), -0.88, 0.88), Clamp(One(gen), -0.88, 0.88)}, (float) Clamp(speed(gen), 10, 400)/Vector2LengthSqr({1,1}));
    }
    disabled = false;
    texture = tex;
    color = YELLOW;

    bulletcount++;
}

void Bullet::update(float dt, Vector2 Circle, bool immune, bool& alive){
    position = Vector2Add(position, Vector2Scale(velocity, dt));
    velocity = Vector2Subtract(velocity, Vector2Scale(velocity, 0.0001*dt));

    if(Vector2Length(Vector2Subtract(Circle, position)) <= 6 & immune==false ){alive = false;}
    if(position.x < 0 | position.x > screenWidth){ disabled = true; }
    else{
        if(position.y > screenHeight){ disabled = true; }
    }
}

void Bullet::draw(){
    DrawTexture(texture, position.x - texture.width/2, position.y - texture.height/2, color);
}