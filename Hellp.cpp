#include <iostream>
#include <string.h>
#include <random>
#include <raylib.h>
#include <raymath.h>

#include "Stars.h"
#include "Bullet.h"

#define MAX_BULLETS 5000
#define MAX_STARS 20

using namespace std;

void Etriangle(Vector2 vector, float radius, Color colour);

Vector2 Scorebox_spawn(int &Length);

int main(){

    const int screenWidth = 800;
    const int screenHeight = 450; 
    const int fps=60;
    int current_fps=fps;

    InitWindow(screenWidth, screenHeight, "Hellp");
    SetTargetFPS(current_fps);

    Color BG = {0,0,0,150};

    Texture2D star_texture[StarTextures];
    for(int i=0; i<StarTextures; i++){
        Image Star = LoadImage(TextFormat("Assets/Stars/Star%02d.png",i));
        star_texture[i]=LoadTextureFromImage(Star);
        UnloadImage(Star);
    }
    
    Vector2 Circle={(float)screenWidth/2.0f, (float)screenHeight/2.0f};
    Vector2 StarPos={screenWidth/2, 0};

    //Player
    Vector2 velocity = {0, 0};
    int speed = 12;
    float spedmult = 4.59;
    bool alive = true;
    bool immune = false;
    float spawn_immunity = 2;
    float immune_timer = 0;

    //Score
    float Score = 0;
    Vector2 Score_box_position = {0,0};
    float Score_timer = 0;
    float Score_time = 1;
    int Length = 0;
    bool active;
    bool exist;

    //Bullet
    Bullet bullet[MAX_BULLETS];
    int bulletcount = 0;
    int bulletDisabled = 0;
    float cooldown = 0.08;
    float timer=cooldown;
    Color bullet_color = YELLOW;

    //Star
    Stars *star = (Stars *)RL_CALLOC(MAX_STARS, sizeof(Stars));
    int starcount=0;
    float scrollspeed = 8;

    RenderTexture bulletTexture = LoadRenderTexture(5,5);

    BeginTextureMode(bulletTexture);
        DrawCircle(bulletTexture.texture.width/2,bulletTexture.texture.height/2,(float)2, YELLOW);
    EndTextureMode();

    for(int i=0; i<5; i++){
        bullet[i].spawntop(bulletcount, bulletTexture.texture, Circle, false);
    }

    for(int i=0; i<20;i++){
        star[starcount].spawn();
        starcount++;
    }

    while(!WindowShouldClose()){

        float dt = GetFrameTime();

        if(immune_timer>0){ immune_timer -= (float) dt;}
        else{immune_timer=0; immune=false;}
        if(alive){
            //Player
            if(IsKeyDown(KEY_S)==true){ velocity.y = 1;}
            else if(IsKeyDown(KEY_W)==true){ velocity.y = -1;}
            else { velocity.y =0; }
            if(IsKeyDown(KEY_D)==true){ velocity.x = 1;}
            else if(IsKeyDown(KEY_A)==true){ velocity.x = -1;}
            else { velocity.x = 0; }
            velocity = Vector2Normalize(velocity);
            velocity = Vector2Scale(velocity, speed);
            Circle = Vector2Add(Circle, Vector2Scale(velocity, spedmult*dt));

            //Star
            for(int i=0;i<starcount;i++){
                star[i].update(dt);
            }

            if(!exist){
                Score_box_position = Scorebox_spawn(Length);
                exist = true;
                std::cout<<Score<<std::endl;
            }
        }

        if(bulletcount>= MAX_BULLETS){
            bulletcount=0;
            bulletDisabled=0;
            int j = 0;
        }

        timer += (float) GetFrameTime();
        
        if( alive == true && timer>=cooldown){
            timer = 0;

            if(bulletcount < MAX_BULLETS){
                int val = 0;
                if(active){val = GetRandomValue(0, 5)/5.0f; cooldown = 0.8;}
                else{val = GetRandomValue(0, 23)/23.0f; cooldown = 0.08;}
                bullet[bulletcount].spawntop(bulletcount, bulletTexture.texture, Circle, (bool) val);
            }
        }

        if(exist){
            if(abs(Score_box_position.x - (int)Circle.x)<=Length/2 && abs(Score_box_position.y - (int) Circle.y )<=Length/2){
                active = true;
            }
            if(active){
                if(alive){
                    Score++;
                }
                else if(abs(Score_box_position.x - Circle.x)>Length/2 || abs(Score_box_position.y - Circle.y )>Length/2){
                    active = false;
                    exist = false;
                }
            }
        }

        for(int i=0; i<bulletcount; i++){
            if(alive==true && !bullet[i].GetDisabled()){
                int disabled = bulletDisabled;
                bullet[i].update(bulletDisabled , GetFrameTime(), Circle, immune, alive);
                if(disabled != bulletDisabled){
                    bullet[i].swapdata(bullet[bulletDisabled-1]);
                }
            }
        }

        BeginDrawing();

        ClearBackground(BG);

        for(int i=0; i<starcount;i++){
            star[i].draw(star_texture);
        }

        DrawLineBezier({0, 0}, {screenWidth, 0}, 7, DARKGRAY);
        DrawLineBezier({0, 0}, {0, screenHeight}, 7, DARKGRAY);
        DrawLineBezier({0, screenHeight}, {screenWidth, screenHeight}, 7, DARKGRAY);
        DrawLineBezier({screenWidth, 0}, {screenWidth, screenHeight}, 7, DARKGRAY);

        
        DrawText(TextFormat("Score: %.0f", Score), 10, 10, 20, DARKGRAY);
        if(exist){
            DrawRectangle(Score_box_position.x - Length/2, Score_box_position.y - Length/2, Length, Length, {200, 200, 200, 100});
        }

        Etriangle(Circle, 8, BLUE);

        for(int i=0; i<bulletcount; i++){
            if(!bullet[i].GetDisabled()){
                bullet[i].draw();
                if(alive){}
                bullet[i].trail(6);
            }
        }
        
        if(IsKeyDown(KEY_R)){immune_timer=spawn_immunity; immune=true; alive = true; }
        if(!alive){
            DrawText("ded!", (int)screenWidth/2.0 , (int)screenHeight/2.0, 25, LIGHTGRAY);
        //I actually forgot to make him alive when cleaning code. So, he was just immortal & dead, which is famously a bad thing.
        //I just realized that I also forgot to make him immune. So, he was just dead. 
        }

        EndDrawing();
    }

    for(int i=0; i<StarTextures; i++){
        UnloadTexture(star_texture[i]);
    }
    UnloadRenderTexture(bulletTexture);

    RL_FREE(star);

    CloseWindow();

    return 0;
}

Vector2 Scorebox_spawn(int &Length){
    random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution  X(0+Length, screenWidth-Length);
    std::uniform_int_distribution  Y(0+Length, screenHeight-Length);

    int x = X(gen);
    int y = Y(gen);
    Vector2 v = {(float) x, (float) y};
    Length = GetRandomValue(40,60);

    std::cout<<"("<<v.x<<","<<v.y<<")"<<std::endl;
    return v;
}

void Etriangle(Vector2 vector, float radius, Color colour){
    Vector2 Vr = {0, (float) -radius};
    Vector2 v1 = Vector2Add(vector, Vr);
    Vr = Vector2Rotate(Vr, (float)(2*PI/3.0f));
    Vector2 v2 = Vector2Add(vector, Vr);
    Vr = Vector2Rotate(Vr, (float)(2*PI/3.0f));
    Vector2 v3 = Vector2Add(vector, Vr);

    DrawTriangle(v1, v3, v2, colour);
}