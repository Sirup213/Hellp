#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "raylib.h"
#include "raymath.h"

#define MAX_BULLETS 5000
#define MAX_STARS 20
#define StarTextures 7

using namespace std;

struct Bullet {
    Vector2 position;
    Vector2 velocity;
    bool disabled; /*i Am being Inclusive*/
    bool spawned;
    Color colour;
};

class Stars{
public:
    Vector2 pos;
    int texture_num;
    bool Far;
};


void Etriangle(Vector2 vector, float radius, Color colour);

void swap(int i, int j);


int main(){
    const int screenWidth = 800;
    const int screenHeight = 450; 
    const int fps=60;
    int current_fps=fps;
    Color BG = {17, 47, 90, 230};


    InitWindow(screenWidth, screenHeight, "Hellp");

    Texture2D star_texture[StarTextures];
    for(int i=0; i<=StarTextures; i++){
        Image Star = LoadImage(TextFormat("Assets/Star%02d.png",i));
        star_texture[i]=LoadTextureFromImage(Star);
        UnloadImage(Star);
    }
    
    Vector2 Circle={(float)screenWidth/2.0f, (float)screenHeight/2.0f};
    Vector2 StarPos={screenWidth/2, 0};

    //Bullet
    Bullet *bull = (Bullet *)RL_CALLOC(MAX_BULLETS, sizeof(Bullet));
    int j = 0;
    Vector2 spawn = {0, 0};
    int bulletcount = 0;
    int bulletDisabled = 0;
    float cooldown = 0.08;
    float timer=cooldown;
    Color bullet_color = YELLOW;

    //Player
    int velocity = 10;
    float spedmult = 4.59;
    bool alive = true;
    bool immune = false;
    float spawn_immunity = 2;
    float immune_timer = 0;

    //Star
    Stars *star = (Stars *)RL_CALLOC(MAX_STARS, sizeof(Stars));
    int starcount=0;
    float scrollspeed = 8;

    RenderTexture bulletTexture = LoadRenderTexture(6,6);

    BeginTextureMode(bulletTexture);
        DrawCircle(bulletTexture.texture.width/2,bulletTexture.texture.height/2,(float)2, YELLOW);
    EndTextureMode();

    SetTargetFPS(current_fps);

    for(int i=0; i<10; i++){
        spawn.x = GetRandomValue(0, screenWidth);
        spawn.y = -GetRandomValue(1, 5);

        bull[bulletcount].position = spawn;
        bull[bulletcount].velocity = (Vector2){(float)2.0*GetRandomValue(0, velocity),(float)2.8*GetRandomValue(0.8*velocity, 2.9*velocity)};
        bull[bulletcount].disabled= false;
        bull[bulletcount].colour = bullet_color;
        
        bulletcount++;
    }

    for(int i=0; i<20;i++){
        spawn.x = GetRandomValue(0, screenWidth);
        spawn.y = -GetRandomValue(-680, 0);
        star[starcount].pos = spawn;
        star[starcount].texture_num = GetRandomValue(0,StarTextures-1);
        starcount++;
    }

    while(!WindowShouldClose()){

        if(immune_timer<spawn_immunity){alive = true; immune_timer += (float) GetFrameTime();}
        if(alive){
            //Player
            if(IsKeyDown(KEY_W)==true){ Circle.y -= GetFrameTime()*spedmult*velocity;}
            if(IsKeyDown(KEY_A)==true){ Circle.x -= GetFrameTime()*spedmult*velocity;}
            if(IsKeyDown(KEY_S)==true){ Circle.y += GetFrameTime()*spedmult*velocity;}
            if(IsKeyDown(KEY_D)==true){ Circle.x += GetFrameTime()*spedmult*velocity;}

            //Star
            for(int i=0;i<starcount;i++){
                star[i].pos.y+=scrollspeed*GetFrameTime();
                if(star[i].pos.y>screenHeight){
                star[i].pos.y=GetRandomValue(-680,0); 
                star[i].pos.x=GetRandomValue(0,screenWidth);
                }
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

            spawn.x = GetRandomValue(0, screenWidth);
            spawn.y = -GetRandomValue(1, 5);

            float vel_x = GetRandomValue(0, velocity);
            float vel_y = GetRandomValue(0.9*velocity, 2.9*velocity);

            if(bulletcount<MAX_BULLETS){
                bull[bulletcount].position = spawn;
                bull[bulletcount].velocity = (Vector2){(float) 10.0*vel_x,(float)10.0*vel_y};
                bull[bulletcount].disabled= false;
                bull[bulletcount].colour = bullet_color;
                
                bulletcount++;
            }
        }

        for(int i=0; i<bulletcount; i++){
            if(alive==true && !bull[i].disabled){
                bull[i].position.x += GetFrameTime()*bull[i].velocity.x;
                bull[i].position.y += GetFrameTime()*bull[i].velocity.y;
                if(Vector2Length(Vector2Subtract(Circle, bull[i].position)) <= 6){alive = false;}
                if(bull[i].position.x < 0 | bull[i].position.x > screenWidth){ bull[i].disabled = true; }
                else{
                    if(bull[i].position.y > screenHeight){ bull[i].disabled = true; }
                }
            }
        }

        BeginDrawing();

        ClearBackground(BG);

        for(int i=0; i<starcount;i++){
            DrawTexture(star_texture[star[i].texture_num], star[i].pos.x, star[i].pos.y, WHITE);
        }

        DrawLineBezier({0, 0}, {screenWidth, 0}, 7, DARKGRAY);
        DrawLineBezier({0, 0}, {0, screenHeight}, 7, DARKGRAY);
        DrawLineBezier({0, screenHeight}, {screenWidth, screenHeight}, 7, DARKGRAY);
        DrawLineBezier({screenWidth, 0}, {screenWidth, screenHeight}, 7, DARKGRAY);

        Etriangle(Circle, 8, BLUE);

        for(int i=0; i<bulletcount; i++){
            if(!bull[i].disabled){
                DrawTexture(bulletTexture.texture, bull[i].position.x - bulletTexture.texture.width/2, bull[i].position.y - bulletTexture.texture.height/2, bull[i].colour);
            }
        }

        if(!alive){
            DrawText("ded!", (int)screenWidth/2.0 , (int)screenHeight/2.0, 25, LIGHTGRAY);
            if(IsKeyPressed(KEY_R)){immune_timer=0;}
        }

        EndDrawing();
        
    }

    for(int i=1; i<=StarTextures; i++){
        UnloadTexture(star_texture[i]);
    }
    UnloadRenderTexture(bulletTexture);

    RL_FREE(bull);
    RL_FREE(star);

    CloseWindow();

    return 0;
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

void swap(int i, int j){
    int temp;

    temp = i;
    i=j; j=temp;
}