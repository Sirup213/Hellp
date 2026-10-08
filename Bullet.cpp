#include <iostream>
#include <random>
#include <raylib.h>
#include <raymath.h>
#include "Bullet.h"

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution <float> rndx(0, screenWidth);
std::uniform_real_distribution <float> y(-10, 0);

bool Bullet::GetDisabled(){
    return disabled;
}

void Bullet::spawntop(int &bulletcount, Texture2D tex, Vector2 Circle, bool targeted){
    position = {rndx(gen), y(gen)};
    if(targeted = true){
    }
    std::normal_distribution <float> ntheta(0, (float)(1.8f/2.0f)*PI);

    Vector2 velocity = Vector2Normalize((Circle, position));
    velocity = Vector2Rotate(velocity,Clamp(ntheta(gen)+GetRandomValue(-0.0001*PI, 0.0001*PI), -(0.8)*PI, (0.8)*PI));
    velocity = Vector2Scale(velocity, (float) 0.2*GetRandomValue(100,2000));

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