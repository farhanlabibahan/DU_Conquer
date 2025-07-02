#include "raylib.h"
#include "raymath.h"
#include "global.h"
#include "shooting.h"

#define WALL 150
#define ENEMY 20
#define ENEMY_AT_A_TIME 10
#define ENEMY_TIME 2
#define ENEMY_SIZE 1
#define CUBE_LENGTH 2
#define CUBE_HEIGHT 3
#define CUBE_WIDTH 2
#define PLAYER_HEIGHT 3.5

double game_start_time = GetTime();
double last_spawn_time = GetTime();


Vector3 prev_pos_shooting = {0};
Camera3D camera_shooting_shooting = {0};

class Cube{
    public:
    Vector3 pos;
    Color color;
};

class EnemyCube{
    public:
    Vector3 pos;
    Color color;
    float time;
    bool alive;
};


Cube cubes[WALL];
EnemyCube enemy[ENEMY_AT_A_TIME];
int enemy_number = ENEMY;
int shot_dead = 0;

Sound shootingSound,deadSound;
Music bgm_shooting;


void generate_cubes()
{
    for(int i=0;i<WALL;i++)
    {
        cubes[i].pos = {(float)GetRandomValue(-50,50),(float)CUBE_HEIGHT/2,(float)GetRandomValue(-50,50)};
        cubes[i].color = RED;
    }
}

void generate_enemy()
{
    for(int i=0;i<ENEMY_AT_A_TIME;i++)
    {
        enemy[i].pos = {(float)GetRandomValue(-50,50),(float)GetRandomValue(1,PLAYER_HEIGHT-1),(float)GetRandomValue(-50,50)};
        enemy[i].color = MAGENTA;
        enemy[i].time = GetTime();
        enemy[i].alive = true;
    }
}

void draw_cubes()
{
    for(auto it : cubes)
    {
        DrawCube(it.pos, CUBE_LENGTH, CUBE_HEIGHT, CUBE_WIDTH, it.color); 
        DrawCubeWires(it.pos, CUBE_LENGTH, CUBE_HEIGHT, CUBE_WIDTH, BLACK); 
    }
}

void draw_enemy()
{
    for(auto& it : enemy)
    {
        if(it.alive)
        {
            DrawCube(it.pos, ENEMY_SIZE, ENEMY_SIZE, ENEMY_SIZE, it.color); 
            DrawCubeWires(it.pos, ENEMY_SIZE, ENEMY_SIZE, ENEMY_SIZE, BLACK); 
        }
    }
}

bool check_obstacles(Vector3 playerPos_shooting)
{
    for(auto it : cubes)
    {
        float x_range = CUBE_LENGTH/2;
        float z_range = CUBE_WIDTH/2;

        if((playerPos_shooting.x >= it.pos.x-x_range && playerPos_shooting.x <= it.pos.x + x_range) && (playerPos_shooting.z >= it.pos.z - z_range && playerPos_shooting.z <=it.pos.z + z_range))
        {
            return true;
        }
    }

    return false;
}

void init_shooting()
{
    shootingSound = LoadSound("resources/shoot.mp3");
    deadSound = LoadSound("resources/death.mp3");
    bgm_shooting = LoadMusicStream("resources/shoot_bgm2.mp3");
    PlayMusicStream(bgm_shooting);

    camera_shooting_shooting.position = (Vector3){ -5.0f, PLAYER_HEIGHT, -5.0f }; 
    camera_shooting_shooting.target = (Vector3){ 0.0f, 1.0f, 0.0f };    
    camera_shooting_shooting.up = (Vector3){ 0.0f, 1.0f, 0.0f };        
    camera_shooting_shooting.fovy = 60.0f;
    camera_shooting_shooting.projection = CAMERA_PERSPECTIVE;
    prev_pos_shooting = camera_shooting_shooting.position;

    generate_cubes(); // for the first time
}

void logic_shooting()
{
    UpdateCamera(&camera_shooting_shooting, CAMERA_FIRST_PERSON);  
        UpdateMusicStream(bgm_shooting);

        if(check_obstacles(camera_shooting_shooting.position)) camera_shooting_shooting.position = prev_pos_shooting;
        else prev_pos_shooting = camera_shooting_shooting.position;

        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            PlaySound(shootingSound);
            Ray ray = GetMouseRay((Vector2){ (float)screenWidth/2, (float)screenHeight/2 }, camera_shooting_shooting);

            for (int i = 0; i < ENEMY_AT_A_TIME; i++)
            {
                if(enemy[i].alive){

                    BoundingBox box = {
                        (Vector3){ enemy[i].pos.x - ENEMY_SIZE * 0.6f, enemy[i].pos.y - ENEMY_SIZE * 0.6f, enemy[i].pos.z - ENEMY_SIZE * 0.6f },
                        (Vector3){ enemy[i].pos.x + ENEMY_SIZE * 0.6f, enemy[i].pos.y + ENEMY_SIZE * 0.6f, enemy[i].pos.z + ENEMY_SIZE * 0.6f }
                    };
                RayCollision collision = GetRayCollisionBox(ray, box);
                    if(collision.hit)
                    {
                        enemy[i].alive = false;
                        PlaySound(deadSound);
                        shot_dead++;
                        break;
                    }
                }
            }
        }
  

        if(GetTime()-last_spawn_time >= ENEMY_TIME)
        {
            generate_enemy();
            last_spawn_time = GetTime();
        }

        if(shot_dead>=5) shooting_game = true;

}

void draw_shooting()
{
            // DisableCursor();
            BeginMode3D(camera_shooting_shooting); 
                draw_cubes(); 
                draw_enemy();
                DrawGrid(100, 1.0f);                               
            EndMode3D();

            DrawCircle(screenWidth/2,screenHeight/2,2.5f,WHITE);

            DrawText(TextFormat("Killed: %d", shot_dead), 0, 50, 30, ORANGE);

            if(shooting_game) DrawText("Conquered MAZE~",0,80,30,GREEN);
}

void unload_shooting()
{
    UnloadSound(shootingSound);
    UnloadSound(deadSound);
    StopMusicStream(bgm_shooting);
    UnloadMusicStream(bgm_shooting);
}
