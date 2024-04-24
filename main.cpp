#include <iostream>
#include <raylib.h>

using namespace std;

//colors
//Color DarkGreen = Color{34,139,34,255};
Color LightGreen = Color{50,205,50,255};
Color Green = Color{0,128,0,255};

//score
int player_score = 0;
int cpu_score = 0;

//creating the ball
class Ball {
    public:
    float x, y;
    int speed_x, speed_y;
    int radius;

    //draw ball
    void Draw (){
        DrawCircle(x, y, radius, WHITE);
        }

    //ball movement
    void MoveBall() {
        x += speed_x;
        y += speed_y;

        // edge collisions
        if (y + radius >= GetScreenHeight() || y - radius <= 0){
            speed_y *= -1;
            }
        if(x + radius >= GetScreenWidth()){
            cpu_score++;
            Reset();
           }
        if(x - radius <= 0){
            player_score++;
            Reset();
        }
    }
    void Reset() {
        x = GetScreenWidth()/2;
        y = GetScreenHeight()/2;

        int speed_choices[2] = {-1,1};
        speed_x *= speed_choices[GetRandomValue(0,1)];
        speed_y *= speed_choices[GetRandomValue(0,1)];
    }
};
// ball object
Ball ball;

//pad class
class Paddle {

    protected:
        void LimitMovement(){
            if(y <= 0) {
            y = 0;
            }
            if(y + hei >= GetScreenHeight()){
            y = GetScreenHeight() - hei;
            }
        }
    public:
    float x, y;
    float wid, hei;
    int speed;

    //draw paddle
    void Draw(){
        DrawRectangleRounded(Rectangle{x,y,wid,hei},0.8, 0, WHITE);
        }

    //moving paddle
    void MovePad(){
        if(IsKeyDown(KEY_UP)){
            y -= speed;
            }
        if(IsKeyDown(KEY_DOWN)){
            y += speed;
            }
        LimitMovement();
        }
};

// paddle object
Paddle paddle;


//computer paddle
class CpuPaddle: public Paddle{
    public:
    void MoveCPU(int ball_y){
        if(y + hei/2 > ball_y){
            y -=  speed;
        }
        if(y + hei/2 <= ball_y){
            y += speed;
        }
        LimitMovement();
    }

};

//cpu paddle object
CpuPaddle cpuPaddle;

//main function
int main(){
    //setting the window
    cout << "Starting the game!" << endl;
    const int screen_h = 800;
    const int screen_w = 1280;
    InitWindow(screen_w, screen_h, "PONG!");
    SetTargetFPS(60);

    //setting ball's attributes
    ball.radius = 20;
    ball.x = screen_w/2;
    ball.y = screen_h/2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    //setting paddle attributes
    paddle.wid = 25;
    paddle.hei = 120;
    paddle.x = screen_w - paddle.wid - 10;
    paddle.y = screen_h/2 - paddle.hei/2;
    paddle.speed = 6;

    //cpuPaddle attributes
    cpuPaddle.wid = 25;
    cpuPaddle.hei = 120;
    cpuPaddle.x = 10;
    cpuPaddle.y = screen_h/2 - cpuPaddle.hei/2;
    cpuPaddle.speed = 6;

    //game loop
    while (WindowShouldClose() == false){
        //drawing
        BeginDrawing();

        //moving elements
        ball.MoveBall();
        paddle.MovePad();
        cpuPaddle.MoveCPU(ball.y);

        //check collision
        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{paddle.x, paddle.y, paddle.wid, paddle.hei})){
            ball.speed_x *=-1;
        }
        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpuPaddle.x, cpuPaddle.y, cpuPaddle.wid, cpuPaddle.hei})){
            ball.speed_x *=-1;
        }
        //clearing
        ClearBackground(Green);
        DrawRectangle(screen_w/2,0,screen_w/2,screen_h,Green);
        DrawCircle(screen_w/2, screen_h/2, 150, LightGreen);

        //line
        DrawLine(screen_w/2,0,screen_w/2, screen_h, WHITE);

        //ball
        ball.Draw();

        //pads
        paddle.Draw();
        cpuPaddle.Draw();
        DrawText(TextFormat("%i",cpu_score), screen_w/4, 20, 80, Fade(WHITE, 0.5));
        DrawText(TextFormat("%i",player_score), 3*screen_w/4 -20, 20, 80, Fade(WHITE, 0.5));
        EndDrawing();
    }

    CloseWindow();

    return 0;
};
