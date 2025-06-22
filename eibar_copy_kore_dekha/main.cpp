#include "raylib.h"
#include "raymath.h"

int screenHeight,screenWidth;

#define WALL 150
#define ENEMY 20
#define ENEMY_AT_A_TIME 50
#define ENEMY_TIME 2
#define ENEMY_SIZE 1
#define CUBE_LENGTH 2
#define CUBE_HEIGHT 3
#define CUBE_WIDTH 2
#define PLAYER_HEIGHT 3.5

double game_start_time = GetTime();
double last_spawn_time = GetTime();

Vector3 prev_pos = {0};
Camera3D camera = { 0 };

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

bool check_obstacles(Vector3 playerPos)
{
    for(auto it : cubes)
    {
        float x_range = CUBE_LENGTH/2;
        float z_range = CUBE_WIDTH/2;

        if((playerPos.x >= it.pos.x-x_range && playerPos.x <= it.pos.x + x_range) && (playerPos.z >= it.pos.z - z_range && playerPos.z <=it.pos.z + z_range))
        {
            return true;
        }
    }

    return false;
}

int main(void)
{

    InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "3D Shooting");
    SetTargetFPS(144);
    ToggleFullscreen();

    InitAudioDevice();
    Sound shootSound = LoadSound("shoot.mp3");
    Sound deadSound = LoadSound("death.mp3");

    Music bgm = LoadMusicStream("shoot_bgm.mp3");
    PlayMusicStream(bgm);

    


    screenHeight = GetScreenHeight();
    screenWidth = GetScreenWidth();

    generate_cubes();
    
    camera.position = (Vector3){ -5.0f, PLAYER_HEIGHT, -5.0f }; 
    camera.target = (Vector3){ 0.0f, 1.0f, 0.0f };    
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };        
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    prev_pos = camera.position;

    generate_enemy();

    DisableCursor();

    while (!WindowShouldClose())    
    {
        UpdateCamera(&camera, CAMERA_FIRST_PERSON);  
        UpdateMusicStream(bgm);

        if(check_obstacles(camera.position)) camera.position = prev_pos;
        else prev_pos = camera.position;

        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            PlaySound(shootSound);
            Ray ray = GetMouseRay((Vector2){ (float)GetScreenWidth()/2, (float)GetScreenHeight()/2 }, camera);

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
        
            BeginDrawing();
            ClearBackground(BLACK);

            BeginMode3D(camera);
                // DrawCube((Vector3){ 0, 1, 0 }, 1, 2, 1, RED);    
                draw_cubes(); 
                draw_enemy();
                DrawGrid(100, 1.0f);                               
            EndMode3D();

            DrawCircle(screenWidth/2,screenHeight/2,2.5f,WHITE);

            DrawText(TextFormat("Killed: %d", shot_dead), 0, 50, 30, ORANGE);
            DrawFPS(10,20);
            EndDrawing();

    }

    UnloadSound(shootSound);
    UnloadSound(deadSound);
    StopMusicStream(bgm);
    UnloadMusicStream(bgm);
    CloseWindow(); 
    return 0;
}
