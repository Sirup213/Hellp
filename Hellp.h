#ifndef HELLP_H

#define HELLP_H

#include <string>

#define StarTextures 8

const int screenWidth = 800;
const int screenHeight = 450;

inline Rectangle placeat(Rectangle r, Vector2 p){
    return {r.x+p.x, r.y+p.y, r.width, r.height};
}

inline Rectangle scalerec(Rectangle r, int m){
    return {m*r.x, m*r.y, m*r.width, m*r.height};
}

struct Player{
    Camera2D cam;
    int health;
    Vector2 position;
    Vector2 velocity;
    int radius;
    Color color;
    float speed;
    float speedmult;
    bool alive;
    bool immune;
    bool damaged;
    float immune_timer;
    int iframes;

    Player(float s, float m, int r){
        cam.target={0, 0};
        cam.rotation=0;
        cam.zoom = 1.0f;
        position={screenWidth/2.0f, screenHeight-20.0f};
        velocity={0,0};
        radius = r;
        color = BLUE;
        speed = s; speedmult = m;
        health = 2;
        alive = true;
        immune = false;
        immune_timer=0;
        damaged=false;
        iframes=0;
    };

    void revive(float spawn_immunity){
        health = 2;
        alive = true;
        immune_timer = spawn_immunity;
        immune = true;
    }

    Vector2 Movement(){
        speedmult = 1;
        if(IsKeyDown(KEY_S)==true){ velocity.y = 1;}
        else if(IsKeyDown(KEY_W)==true){ velocity.y = -1;}
        else { velocity.y =0; }
        if(IsKeyDown(KEY_D)==true){ velocity.x = 1;}
        else if(IsKeyDown(KEY_A)==true){ velocity.x = -1;}
        else { velocity.x = 0; }
        velocity = Vector2Normalize(velocity);
        velocity = Vector2Scale(velocity, speed*speedmult);
        return velocity;
    }

    void damage(int d){
        if(iframes==0 && !damaged){
            health-=d;
            iframes = 5;
            if(health==0){alive=false;}
            damaged = true;
        }
    }


};

struct Textbox{
    Texture2D texture;
    Rectangle splice[9];
    int w1,w2,h1,h2;

    Textbox(Texture2D tex, float x1, float x2, float y1, float y2){
        texture = tex;
        w1=x1; w2=x2; h1=y1; h2=y2;
        splice[0] = {0, 0, x1, y1};//1,1
        splice[1] = {x1, 0, x2, y1};//2,1
        splice[2] = {x1+x2, 0, x1, y1};//3,1
        splice[3] = {0, y1, x1, y2};//1,2
        splice[4] = {x1, y1, x2, y2};//2,2
        splice[5] = {x1+x2, y1, x1, y2};//3,2
        splice[6] = {0, y1+y2, x1, y1};//1,3
        splice[7] = {x1, y1+y2, x2, y1};//2,3
        splice[8] = {x1+x2, y1+y2, x1, y1};//3,3
    }
    
    void Dialogue(const std::string &text, float fontsize, Vector2 position){
        float length = MeasureText(text.c_str(), fontsize);
        float m = roundf(fontsize/5.0f);
        Vector2 p = {roundf(position.x), roundf(position.y)};
        int scalefactor = m;
        Vector2 correction = {0, 0};

        for(int i=0; i<9; i++){
            if(length>m*splice[1].width){
                if(i%3==1){scalefactor = length/splice[i].width;}
                if(i%3==2){correction = {length, 0};}
                else{scalefactor = m; correction = {0, 0};}
            }
            DrawTexturePro(texture, splice[i], placeat(scalerec(splice[i], scalefactor), p), {0,0}, 0, WHITE);
        }

        DrawText(text.c_str(), p.x+m*splice[0].width, p.y+m*splice[0].height, fontsize, BLACK);
    }
};

#endif