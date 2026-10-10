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

void Bullet::spawntop(int &bulletcount, Texture2D tex, Vector2 Circle, bool targeted, bool active){
    position = {rndx(gen), y(gen)};

    if(targeted = true){
    velocity = Vector2Normalize(Vector2Subtract(Circle, position));
    velocity = Vector2Rotate(velocity,Clamp(ntheta(gen), -(0.8)*PI, (0.8)*PI));
    }
    else{
        velocity.x = Clamp(One(gen), -0.88, 0.88);
        velocity.y = Clamp(One(gen), -0.88, 0.88);
        velocity = Vector2Normalize(velocity);
    }
    radius = 3;
    velocity = Vector2Scale(velocity, Clamp(speed(gen), 10, 400));

    if(!active){}
    else{}


    disabled = false;
    texture = tex;
    color = YELLOW;

    bulletcount++;
}

void Bullet::update(int &bulletdisabled, float dt, Vector2 Circle, bool immune, bool& alive){
    pastpos = position;
    position = Vector2Add(position, Vector2Scale(velocity, dt));
    velocity = Vector2Subtract(velocity, Vector2Scale(velocity, 0.0001*dt));

    if(Vector2Length(Vector2Subtract(Circle, position)) <= 4+radius & immune==false ){alive = false;}
    if(position.x < 0 | position.x > screenWidth){ disabled = true; bulletdisabled++; }
    else{
        if(position.y > screenHeight){ disabled = true; bulletdisabled++; }
    }
}

void Bullet::draw(){
    DrawTexturePro(texture,
        {0, 0, (float)texture.width, (float)texture.height},
        {position.x - radius, position.y - radius, 2.0f*radius, 2.0f*radius},
        {0, 0}, 0, color);
}

void Bullet::trail(int stepsize, int length, int skip){
    Vector2 diff = Vector2Subtract(pastpos, position);

    if(Vector2Length(diff) < 1){return;}

    Vector2 step = Vector2Scale(diff, (float)1/stepsize);
    Color trail_colour = color;

    for(int i=0; i < 4*stepsize ; i+=skip){
        if(trail_colour.a < 2){break;}
        Vector2 stepI = Vector2Scale(step, i);

        DrawTexturePro(texture,
        {0, 0, (float)texture.width, (float)texture.height},
        {(position.x + stepI.x) - radius, (position.y+stepI.y) - radius, 2.0f*radius, 2.0f*radius},
        {0, 0}, 0, trail_colour);
        trail_colour.a = trail_colour.a*0.8625;
    }
}

void Bullet::swapdata(Bullet &bullet){
    using std::swap;
    swap(position, bullet.position);
    swap(pastpos, bullet.pastpos);
    swap(velocity, bullet.velocity);
    swap(texture, bullet.texture);
    swap(disabled, bullet.disabled);
}