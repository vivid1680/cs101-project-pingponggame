#include <raylib.h>
#include <iostream>
#include <fstream>

using namespace std;

// Custom colors
Color Green = Color{38, 185, 154, 255};
Color Dark_Green = Color{20, 160, 133, 255};
Color Light_Green = Color{129, 204, 184, 255};
Color Yellow = Color{243, 213, 91, 255};

// Global variables
int player_score = 0, cpu_score = 0;
int player1_score = 0, player2_score = 0;
int user_wins = 0, cpu_wins = 0;

// Ball class
class Ball {
 public:
    float x, y;
    int speed_x, speed_y;
    int radius;

    void Draw() {
        DrawCircle(x, y, radius, Yellow);
    }

    void Update() {
        x += speed_x;
        y += speed_y;

        if (y + radius >= GetScreenHeight() || y - radius <= 0) {
            speed_y *= -1;
        }

        if (x + radius >= GetScreenWidth()) {
            cpu_score++;
            ResetBall();
        }

        if (x - radius <= 0) {
            player_score++;
            ResetBall();
        }
    }

    void ResetBall() {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speed_choices[2] = {-1, 1};
        speed_x *= speed_choices[GetRandomValue(0, 1)];
        speed_y *= speed_choices[GetRandomValue(0, 1)];
    }
};

// Paddle class
class Paddle {
 protected:
    void LimitMovement() {
        if (y <= 0) y = 0;
        if (y + height >= GetScreenHeight()) y = GetScreenHeight() - height;
    }

 public:
    float x, y;
    float width, height;
    int speed;

    void Draw() {
        DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, WHITE);
    }

    void Update(bool is_player) {
        if (is_player) {
            if (IsKeyDown(KEY_UP)) y -= speed;
            if (IsKeyDown(KEY_DOWN)) y += speed;
        } else {
            if (IsKeyDown(KEY_W)) y -= speed;
            if (IsKeyDown(KEY_S)) y += speed;
        }
        LimitMovement();
    }
};

// CPU paddle class
class CpuPaddle : public Paddle {
 public:
    void Update(int ball_y) {
        if (y + height / 2 > ball_y) y -= speed;
        if (y + height / 2 < ball_y) y += speed;
        LimitMovement();
    }
};

// Functions to save and load stats
void SaveStats(int user_wins, int cpu_wins) {
    ofstream outfile("highscores.txt");
    if (outfile.is_open()) {
        outfile << "User Wins: " << user_wins << "\n";
        outfile << "CPU Wins: " << cpu_wins << "\n";
        outfile.close();
        cout << "Stats saved successfully!" << endl;
    } else {
        cerr << "Error saving stats!" << endl;
    }
}

void LoadStats(int &user_wins, int &cpu_wins) {
    ifstream infile("highscores.txt");
    if (infile.is_open()) {
        infile.ignore(256, ':');
        infile >> user_wins;
        infile.ignore(256, ':');
        infile >> cpu_wins;
        infile.close();
    }
}

int main() {
    // Initialize game
    const int screen_width = 1280;
    const int screen_height = 800;
    InitWindow(screen_width, screen_height, "Pong Game - Single & Multiplayer!");
    SetTargetFPS(60);

    LoadStats(user_wins, cpu_wins);

    // Initialize ball
    Ball* ball = new Ball();
    ball->radius = 20;
    ball->x = screen_width / 2;
    ball->y = screen_height / 2;
    ball->speed_x = 7;
    ball->speed_y = 7;

    // Initialize paddles
    Paddle* player = new Paddle();
    player->width = 25;
    player->height = 120;
    player->x = screen_width - player->width - 10;
    player->y = screen_height / 2 - player->height / 2;
    player->speed = 6;

    Paddle* player2 = new Paddle();
    player2->width = 25;
    player2->height = 120;
    player2->x = 10;
    player2->y = screen_height / 2 - player2->height / 2;
    player2->speed = 6;

    CpuPaddle* cpu = new CpuPaddle();
    cpu->width = 25;
    cpu->height = 120;
    cpu->x = 10;
    cpu->y = screen_height / 2 - cpu->height / 2;
    cpu->speed = 6;

    // Game state variables
    bool game_over = false;
    bool player_wins = false;
    bool show_menu = true;
    bool show_instructions = false;
    bool show_stats = false;
    bool multiplayer_mode = false;

    // Main game loop
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(Dark_Green);

        if (show_menu) {
            DrawText("Pong Game", screen_width / 2 - MeasureText("Pong Game", 50) / 2, 100, 50, Light_Green);
            DrawText("1. Press ENTER for Single Player", screen_width / 2 - MeasureText("1. Press ENTER for Single Player", 30) / 2, 300, 30, WHITE);
            DrawText("2. Press M for Multiplayer", screen_width / 2 - MeasureText("2. Press M for Multiplayer", 30) / 2, 350, 30, WHITE);
            DrawText("3. Press I for Instructions", screen_width / 2 - MeasureText("3. Press I for Instructions", 30) / 2, 400, 30, WHITE);
            DrawText("4. Press S for Stats", screen_width / 2 - MeasureText("4. Press S for Stats", 30) / 2, 450, 30, WHITE);

            if (IsKeyPressed(KEY_ENTER)) {
                show_menu = false;
                multiplayer_mode = false;
            }
            if (IsKeyPressed(KEY_M)) {
                show_menu = false;
                multiplayer_mode = true;
            }
            if (IsKeyPressed(KEY_I)) {
                show_menu = false;
                show_instructions = true;
            }
            if (IsKeyPressed(KEY_S)) {
                show_menu = false;
                show_stats = true;
            }
        } else if (show_instructions) {
            DrawText("Instructions:", screen_width / 2 - MeasureText("Instructions:", 40) / 2, 100, 40, Light_Green);
            DrawText("Use UP/DOWN arrows to control Player 1", screen_width / 2 - MeasureText("Use UP/DOWN arrows to control Player 1", 30) / 2, 200, 30, WHITE);
            DrawText("In Multiplayer: Use W/S to control Player 2", screen_width / 2 - MeasureText("In Multiplayer: Use W/S to control Player 2", 30) / 2, 250, 30, WHITE);
            DrawText("First to 5 points wins!", screen_width / 2 - MeasureText("First to 5 points wins!", 30) / 2, 300, 30, WHITE);
            DrawText("Press BACKSPACE to return to Menu", screen_width / 2 - MeasureText("Press BACKSPACE to return to Menu", 20) / 2, 500, 20, YELLOW);

            if (IsKeyPressed(KEY_BACKSPACE)) {
                show_menu = true;
                show_instructions = false;
            }
        } else if (show_stats) {
            DrawText("Player1 Wins:", screen_width / 2 - MeasureText("Player1 Wins:", 30) / 2, 200, 30, WHITE);
            DrawText(TextFormat("%d", user_wins), screen_width / 2, 240, 30, WHITE);
            DrawText("Player2 Wins:", screen_width / 2 - MeasureText("Player2 Wins:", 30) / 2, 300, 30, WHITE);
            DrawText(TextFormat("%d", cpu_wins), screen_width / 2, 340, 30, WHITE);
            DrawText("Press BACKSPACE to return to Menu", screen_width / 2 - MeasureText("Press BACKSPACE to return to Menu", 20) / 2, 500, 20, YELLOW);

            if (IsKeyPressed(KEY_BACKSPACE)) {
                show_stats = false;
                show_menu = true;
            }
        } else if (!game_over) {
            ball->Update();

            if (multiplayer_mode) {
                player->Update(true);
                player2->Update(false);
            } else {
                player->Update(true);
                cpu->Update(ball->y);
            }

            if (CheckCollisionCircleRec({ball->x, ball->y}, ball->radius, {player->x, player->y, player->width, player->height}) ||
                CheckCollisionCircleRec({ball->x, ball->y}, ball->radius, {player2->x, player2->y, player2->width, player2->height}) ||
                CheckCollisionCircleRec({ball->x, ball->y}, ball->radius, {cpu->x, cpu->y, cpu->width, cpu->height})) {
                ball->speed_x *= -1;
            }

            if (player_score >= 5 || player1_score >= 5) {
                game_over = true;
                player_wins = true;
                user_wins++;
            } else if (cpu_score >= 5 || player2_score >= 5) {
                game_over = true;
                player_wins = false;
                cpu_wins++;
            }

            DrawRectangle(screen_width / 2, 0, screen_width / 2, screen_height, Green);
            DrawCircle(screen_width / 2, screen_height / 2, 150, Light_Green);
            DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);
            ball->Draw();
            if (multiplayer_mode) player2->Draw();
            else cpu->Draw();
            player->Draw();
            DrawText(TextFormat("%i", cpu_score), screen_width / 4 - 20, 20, 80, WHITE);
            DrawText(TextFormat("%i", player_score), 3 * screen_width / 4 - 20, 20, 80, WHITE);
        } else {
            ClearBackground(Dark_Green);
            if (player_wins) {
                DrawText("Player1 WINS!", screen_width / 2 - MeasureText("Player1 WINS!", 80) / 2, screen_height / 2 - 40, 80, Light_Green);
            } else {
                DrawText("Player2 WINS!", screen_width / 2 - MeasureText("Player2 WINS!", 80) / 2, screen_height / 2 - 40, 80, Yellow);
            }

            DrawText("Press ESC to exit", screen_width / 2 - MeasureText("Press ESC to exit", 20) / 2, screen_height - 100, 20, WHITE);
            DrawText("Press M to return to Menu", screen_width / 2 - MeasureText("Press M to return to Menu", 20) / 2, screen_height - 50, 20, WHITE);

            if (IsKeyPressed(KEY_M)) {
                player_score = 0;
                cpu_score = 0;
                player1_score = 0;
                player2_score = 0;
                ball->ResetBall();
                show_menu = true;
                game_over = false;
            }

            if (IsKeyPressed(KEY_ESCAPE)) {
                SaveStats(user_wins, cpu_wins);
                break;
            }
        }

        EndDrawing();
    }

    SaveStats(user_wins, cpu_wins);
    delete ball;
    delete player;
    delete player2;
    delete cpu;
    CloseWindow();

    return 0;
}
