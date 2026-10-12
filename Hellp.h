#ifndef HELLP_H

#define HELLP_H

#define StarTextures 8

const int screenWidth = 800;
const int screenHeight = 450;

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

#endif