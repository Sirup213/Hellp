#include <iostream>
#include <string.h>
#include <random>
#include <raylib.h>
#include <raymath.h>

#include "Stars.h"

#define MAX_BULLETS 5000
#define MAX_STARS 20

using namespace std;

struct Bullet {
    Vector2 position;
    Vector2 velocity;
    bool disabled; /*i Am being Inclusive*/
    bool spawned;
    Color colour;
};

void Etriangle(Vector2 vector, float radius, Color colour);

int main(){

    random_device rd;
    mt19937 gen(rd());

    const int screenWidth = 800;
    const int screenHeight = 450; 
    const int fps=60;
    int current_fps=fps;

    uniform_int_distribution rndx(0, screenWidth);
    uniform_int_distribution rndy(0, screenHeight);
    normal_distribution <float> nrdx(screenWidth/2.0f, screenWidth/6.0f);

    InitWindow(screenWidth, screenHeight, "Hellp");
    SetTargetFPS(current_fps);

    Color BG = {0,0,0,255};

    Texture2D star_texture[StarTextures];
    for(int i=0; i<StarTextures; i++){
        Image Star = LoadImage(TextFormat("Assets/Stars/Star%02d.png",i));
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
    float cooldown = 0.1;
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

    for(int i=0; i<5; i++){
        spawn.x = Clamp(nrdx(gen), 0, screenWidth);
        spawn.y = -5*rndy(gen)/screenHeight;

        bull[bulletcount].position = spawn;
        bull[bulletcount].velocity = (Vector2){(float)2.0*GetRandomValue(0, velocity),(float)2.8*GetRandomValue(0.8*velocity, 2.9*velocity)};
        bull[bulletcount].disabled= false;
        bull[bulletcount].colour = bullet_color;
        
        bulletcount++;
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
            if(IsKeyDown(KEY_W)==true){ Circle.y -= dt*spedmult*velocity;}
            if(IsKeyDown(KEY_A)==true){ Circle.x -= dt*spedmult*velocity;}
            if(IsKeyDown(KEY_S)==true){ Circle.y += dt*spedmult*velocity;}
            if(IsKeyDown(KEY_D)==true){ Circle.x += dt*spedmult*velocity;}

            //Star
            for(int i=0;i<starcount;i++){
                star[i].update(dt);
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

            spawn.x = Clamp(nrdx(gen), 0, screenWidth);
            spawn.y = -5*rndy(gen)/screenHeight;

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
                if(Vector2Length(Vector2Subtract(Circle, bull[i].position)) <= 6 & immune==false ){alive = false;}
                if(bull[i].position.x < 0 | bull[i].position.x > screenWidth){ bull[i].disabled = true; }
                else{
                    if(bull[i].position.y > screenHeight){ bull[i].disabled = true; }
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

        Etriangle(Circle, 8, BLUE);

        for(int i=0; i<bulletcount; i++){
            if(!bull[i].disabled){
                DrawTexture(bulletTexture.texture, bull[i].position.x - bulletTexture.texture.width/2, bull[i].position.y - bulletTexture.texture.height/2, bull[i].colour);
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