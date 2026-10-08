#include <iostream>
#include <random>
#include <raylib.h>
#include <raymath.h>
#include "Stars.h"

using namespace std;

random_device rd;
mt19937 gen(rd());
normal_distribution <float> nrdx(screenWidth/2.0f, screenWidth/6.0f);

void Stars::spawn(){
    pos.x = Clamp(nrdx(gen), 0, screenWidth);
    pos.y = GetRandomValue(-660,0);
    Stars::choose();
}

void Stars::choose(){
    fast = GetRandomValue(0,1);
    Far = GetRandomValue(0,1);

    if(fast == true){
        scrollspeed = 12;
        if(Far == true){
            int val = GetRandomValue(0,1);
            switch (val)
            {
            case 0:
                texture_num = 2;
                break;
            case 1:
                texture_num = 3;
                break;
            default:
                cout<<"ERROR: Choose failed for far, fast"<<endl;
                break;
            }
        }
        else{
            int val = GetRandomValue(0,1);
            switch (val)
            {
            case 0:
                texture_num = 5;
                break;
            case 1:
                texture_num = 5;
                break;
            default:
                cout<<"ERROR: Choose failed for near, fast"<<endl;
                break;
            }
        }
    }
    else{scrollspeed = 8; texture_num = 4;}
}

void Stars::update(float dt){
    pos.y += scrollspeed*dt;
    if(pos.y > screenHeight){ spawn(); }
}

void Stars::draw(Texture2D star_texture[StarTextures]){
    DrawTexture(star_texture[texture_num], pos.x, pos.y, LIGHTGRAY);
}