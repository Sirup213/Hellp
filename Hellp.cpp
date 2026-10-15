#include <iostream>
#include <string.h>
#include <random>
#include <raylib.h>
#include <raymath.h>

#include "Stars.h"
#include "Bullet.h" 
#include "Hellp.h"

#define MAX_BULLETS 20
#define MAX_STARS 20

using namespace std;

void Etriangle(Vector2 vector, float radius, Color colour);

Vector2 Scorebox_spawn(int &Length, int tolerance);

void Dialogue(const string& text, float fontsize, Vector2 position, Texture2D tex){
    float length = MeasureText(text.c_str(), fontsize);
    float m = roundf(fontsize/5.0f);
    Vector2 p = {roundf(position.x), roundf(position.y)};

    DrawTexturePro(tex, {0,0,3,3}, {p.x, p.y, m*3, m*3}, {0,0}, 0, WHITE);//(1,1)
    DrawTexturePro(tex, {3,0,24,3}, {p.x + m*3, p.y, length, m*3}, {0,0}, 0, WHITE);//(2,1)
    DrawTexturePro(tex, {27,0,3,3}, {p.x + length + m*3, p.y, m*3, m*3}, {0,0}, 0, WHITE);//(3,1)
    DrawTexturePro(tex, {0,3,3,3}, {p.x, p.y + m*3, m*3, fontsize}, {0,0}, 0, WHITE);//(1,2)
    DrawTexturePro(tex, {3,3,24,3}, {p.x + m*3, p.y+m*3, length, fontsize}, {0,0}, 0, WHITE);//(2,2)
    DrawTexturePro(tex, {27,3,3,3}, {p.x + length + m*3, p.y+m*3, m*3, fontsize}, {0,0}, 0, WHITE);//(3,2)
    DrawTexturePro(tex, {0,6,3,3}, {p.x, p.y + m*3 + fontsize, m*3, m*3}, {0,0}, 0, WHITE);//(1,3)
    DrawTexturePro(tex, {3,6,24,3}, {p.x + m*3, p.y + m*3 + fontsize, length, m*3}, {0,0}, 0, WHITE);//(2,3)
    DrawTexturePro(tex, {27,6,3,3}, {p.x + length + m*3, p.y+m*3 + fontsize, m*3, m*3}, {0,0}, 0, WHITE);//(3,3)
    DrawText(text.c_str(), p.x+m*3, p.y+m*3, fontsize, BLACK);
}

struct Character
{
    string name;
    string Imagepath;
    Texture2D spritesheet;
    int expression;

    Character(string n, string p){ name = n; Imagepath = p; expression = 0; }
    int move(){expression++; return expression%4;}
};

int main(){

    //Resolution in Hellp.h
    const int fps=60;
    const int Textarea = 240;
    int current_fps=fps;
    //Starting the window
    InitWindow(screenWidth, screenHeight, "Hellp");
    SetTargetFPS(current_fps);

    Color BG = {0,0,0,150};
    //Textures
    RenderTexture bulletTexture = LoadRenderTexture(7,7);
    BeginTextureMode(bulletTexture);
        DrawCircle(bulletTexture.texture.width/2,bulletTexture.texture.height/2,(float)2, YELLOW);
    EndTextureMode();

    Texture2D star_texture[StarTextures];
    for(int i=0; i<StarTextures; i++){
        Image Star = LoadImage(TextFormat("Assets/Stars/Star%02d.png",i));
        star_texture[i]=LoadTextureFromImage(Star);
        UnloadImage(Star);
    }

    Image Back = LoadImage("Assets/starrysky02.png");
    Texture2D Starrysky = LoadTextureFromImage(Back);
    UnloadImage(Back);

    Vector2 StarPos={screenWidth/2, 0};

    //Player(Cam inside)
    Player player(60, 1, 8);
    float spawn_immunity = 2;
    float cam_timer = 0;
    float shake_time = 0.5;
    bool camera_shake=false;

    //Score
    float Score = 0;
    Vector2 Score_box_position = {0,0};
    float Score_timer = 0;
    float Score_time = 1;
    float Score_check = 0;
    int Length = 0;
    bool active = false;
    bool exist = false;

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

    for(int i=0; i<20;i++){
        star[starcount].spawn();
        starcount++;
    }

    while(!WindowShouldClose()){

        float dt = GetFrameTime();
        if(player.iframes>0){player.iframes--;} 
        if(IsKeyPressed(KEY_O)){player.damage(1);}

        if(player.immune_timer>0){ player.immune_timer -= (float) dt;}
        else{player.immune_timer=0; player.immune=false;}
        if(player.alive){
            //Player
            player.position = Vector2Add(player.position, Vector2Scale(player.Movement(), dt));

            //Star
            for(int i=0;i<starcount;i++){
                star[i].update(dt);
            }

            if(!exist){
                Score_box_position = Scorebox_spawn(Length, 20);
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
        
        if( player.alive == true && timer>=cooldown){
            timer = 0;

            if(bulletcount < MAX_BULLETS){
                int val = 0;
                if(active){val = GetRandomValue(0, 1); cooldown = 0.1;}
                else{val = GetRandomValue(0, 23)/23.0f; cooldown = 0.08;}
                bullet[bulletcount].spawntop(bulletcount, bulletTexture.texture, player.position, (bool) val, active);
            }
        }

        if(exist){
            Score_check+=dt;
            Score_timer+=dt;
            if(!active){
                if(abs(Score_box_position.x - player.position.x)<=Length/2 && abs(Score_box_position.y - player.position.y )<=Length/2){active = true;}
            }
            else if(active && Score_check > 0.1){
                Score_check = 0;
                if(player.alive && Score_timer > Score_time){
                    Score_timer = 0;
                    Score++;
                }
                if(abs((int)Score_box_position.x -(int)player.position.x)>Length/2 || abs((int)Score_box_position.y - (int)player.position.y)>Length/2){
                    active = false;
                    exist = false;
                }
            }
        }

        for(int i=0; i<bulletcount; i++){
            if(player.alive==true && !bullet[i].GetDisabled()){
                int disabled = bulletDisabled;
                bullet[i].update(bulletDisabled , GetFrameTime(), player);
                if(disabled != bulletDisabled){
                    bullet[i].swapdata(bullet[MAX_BULLETS - (bulletDisabled-1)]);
                }
            }
        }
        float shake_magnitude = 6.0;
        if(player.damaged && !camera_shake){
            if(player.alive){cam_timer = shake_time; player.damaged=false; camera_shake=true;}
            else{player.damaged = false;}            
        }
        if(cam_timer<=shake_time*1.0f && cam_timer>shake_time*2.0f/3.0f){
            player.cam.offset.x += (shake_magnitude*3.0f)*dt;
            player.cam.offset.y += (shake_magnitude*3.0f)*dt;
            cam_timer-=dt;
            player.color = RED;
        }
        else if(cam_timer>0.0f && cam_timer<shake_time*2.0f/3.0f){
            player.cam.offset.x -= (shake_magnitude*3.0f/2.0f)*dt;
            player.cam.offset.y -= (shake_magnitude*3.0f/2.0f)*dt;
            cam_timer-=dt;
            player.color = BLUE;
            if(player.cam.offset.x < 0.01){player.cam.offset.x=0;player.cam.offset.y=0; camera_shake=false;}
        }
        if(cam_timer<0){cam_timer=0; camera_shake=false;}

        BeginDrawing();

        ClearBackground(BG);

        DrawTexture(Starrysky, 0, 0, WHITE);

        for(int i=0; i<starcount;i++){
            star[i].draw(star_texture);
        }

        if(exist){
            DrawRectangle(Score_box_position.x - Length/2, Score_box_position.y - Length/2, Length, Length, {200, 200, 200, 90});
        }

        BeginMode2D(player.cam);
            Etriangle(player.position, player.radius, player.color);
        EndMode2D();

        for(int i=0; i<bulletcount; i++){
            if(!bullet[i].GetDisabled()){
                bullet[i].draw();
                if(player.alive){}
                bullet[i].trail(6);
            }
        }

        DrawLineBezier({0, 0}, {screenWidth, 0}, 7, DARKGRAY);
        DrawLineBezier({0, 0}, {0, screenHeight}, 7, DARKGRAY);
        DrawLineBezier({0, screenHeight}, {screenWidth, screenHeight}, 7, DARKGRAY);
        DrawLineBezier({screenWidth, 0}, {screenWidth, screenHeight}, 7, DARKGRAY);
    
        DrawText(TextFormat("Score: %.0f", Score), 10, 10, 20, DARKGRAY);
        DrawText(TextFormat("Position: (%.0f, %.0f)", player.position.x, player.position.y), 10, 30, 20, DARKGRAY);
        
        if(IsKeyDown(KEY_R)){player.revive(spawn_immunity);}
        if(!player.alive){
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

Vector2 Scorebox_spawn(int &Length, int tolerance){
    random_device rd;
    std::mt19937 gen(rd());
    Length = GetRandomValue(40,60);
    std::uniform_int_distribution  X(0+Length, screenWidth-Length);
    std::uniform_int_distribution  Y(0+Length+tolerance, screenHeight-Length-tolerance);

    int x = X(gen);
    int y = Y(gen);
    Vector2 v = {(float) x, (float) y};
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