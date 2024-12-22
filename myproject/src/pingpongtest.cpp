#include <raylib.h>
#include <iostream>
#include <fstream>

using namespace std;

Color Green = Color{38, 185, 154, 255};
Color Dark_Green = Color{20, 160, 133, 255};
Color Light_Green = Color{129, 204, 184, 255};
Color Yellow = Color{243, 213, 91, 255};

int player1_score = 0;
int player2_score = 0;
int player1_wins = 0;
int player2_wins = 0;

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
            player1_score++;
            ResetBall();
        }
        if (x - radius <= 0) {
            player2_score++;
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

class Paddle {
protected:
    void LimitMovement() {
        if (y <= 0) {
            y = 0;
        }
        if (y + height >= GetScreenHeight()) {
            y = GetScreenHeight() - height;
        }
    }

public:
    float x, y;
    float width, height;
    int speed;

    void Draw() {
        DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, WHITE);
    }

    void Update(bool isPlayer1) {
        if (isPlayer1) {
            if (IsKeyDown(KEY_W)) {
                y -= speed;
            }
            if (IsKeyDown(KEY_S)) {
                y += speed;
            }
        } else {
            if (IsKeyDown(KEY_UP)) {
                y -= speed;
            }
            if (IsKeyDown(KEY_DOWN)) {
                y += speed;
            }
        }
        LimitMovement();
    }
};

void SaveStats(int player1_wins, int player2_wins) {
    ofstream outfile("highscores.txt");
    if (outfile.is_open()) {
        outfile << "Player 1 Wins: " << player1_wins << "\n";
        outfile << "Player 2 Wins: " << player2_wins << "\n";
        outfile.close();
        cout << "Stats saved successfully!" << endl;
    } else {
        cerr << "Error saving stats!" << endl;
    }
}

void LoadStats(int &player1_wins, int &player2_wins) {
    ifstream infile("highscores.txt");
    if (infile.is_open()) {
        infile.ignore(256, ':');
        infile >> player1_wins;
        infile.ignore(256, ':');
        infile >> player2_wins;
        infile.close();
    }
}

void DrawDashedLine(int x, int screenHeight, int dashLength, int gapLength) {
    for (int y = 0; y < screenHeight; y += dashLength + gapLength) {
        DrawLine(x, y, x, y + dashLength, WHITE);
    }
}

int main() {
    cout << "Starting the game" << endl;
    const int screen_width = 1280;
    const int screen_height = 800;
    InitWindow(screen_width, screen_height, "My Pong Game!");
    SetTargetFPS(60);

    LoadStats(player1_wins, player2_wins);

    Ball* ball = new Ball();
    ball->radius = 20;
    ball->x = screen_width / 2;
    ball->y = screen_height / 2;
    ball->speed_x = 7;
    ball->speed_y = 7;

    Paddle* player1 = new Paddle();
    player1->width = 25;
    player1->height = 120;
    player1->x = 10;
    player1->y = screen_height / 2 - player1->height / 2;
    player1->speed = 6;

    Paddle* player2 = new Paddle();
    player2->width = 25;
    player2->height = 120;
    player2->x = screen_width - player2->width - 10;
    player2->y = screen_height / 2 - player2->height / 2;
    player2->speed = 6;

    bool game_over = false;
    bool player1_wins_game = false;
    bool show_menu = true;
    bool show_instructions = false;
    bool show_stats = false;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(Dark_Green);

        if (show_menu) {
            DrawText("Welcome to Pong Game!", screen_width / 2 - MeasureText("Welcome to Pong Game!", 50) / 2, 100, 50, Light_Green);
            DrawText("1. Press ENTER to Start Game", screen_width / 2 - MeasureText("1. Press ENTER to Start Game", 30) / 2, 300, 30, WHITE);
            DrawText("2. Press I for Instructions", screen_width / 2 - MeasureText("2. Press I for Instructions", 30) / 2, 350, 30, WHITE);
            DrawText("3. Press S to View Stats", screen_width / 2 - MeasureText("3. Press S to View Stats", 30) / 2, 400, 30, WHITE);

            if (IsKeyPressed(KEY_ENTER)) {
                show_menu = false;
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
            DrawText("Player 1: Use W to move up and S to move down", screen_width / 2 - MeasureText("Player 1: Use W to move up and S to move down", 30) / 2, 200, 30, WHITE);
            DrawText("Player 2: Use UP arrow to move up and DOWN arrow to move down", screen_width / 2 - MeasureText("Player 2: Use UP arrow to move up and DOWN arrow to move down", 30) / 2, 250, 30, WHITE);
            DrawText("First to 5 points wins!", screen_width / 2 - MeasureText("First to 5 points wins!", 30) / 2, 300, 30, WHITE);
            DrawText("Press BACKSPACE to return to menu", screen_width / 2 - MeasureText("Press BACKSPACE to return to menu", 20) / 2, 500, 20, YELLOW);
            if (IsKeyPressed(KEY_BACKSPACE)) {
                show_menu = true;
                show_instructions = false;
            }
        } else if (show_stats) {
            ClearBackground(Dark_Green);
            DrawText("Player 1 Wins:", screen_width / 2 - MeasureText("Player 1 Wins:", 30) / 2, 200, 30, WHITE);
            DrawText(TextFormat("%d", player1_wins), screen_width / 2, 240, 30, WHITE);
            DrawText("Player 2 Wins:", screen_width / 2 - MeasureText("Player 2 Wins:", 30) / 2, 300, 30, WHITE);
            DrawText(TextFormat("%d", player2_wins), screen_width / 2, 340, 30, WHITE);
            DrawText("Press BACKSPACE to return to menu", screen_width / 2 - MeasureText("Press BACKSPACE to return to menu", 20) / 2, 500, 20, YELLOW);
            if (IsKeyPressed(KEY_BACKSPACE)) {
                show_stats = false;
                show_menu = true;
            }
        } else if (!game_over) {
            ball->Update();
            player1->Update(true);
            player2->Update(false);

            if (CheckCollisionCircleRec({ball->x, ball->y}, ball->radius, {player1->x, player1->y, player1->width, player1->height})) {
                ball->speed_x *= -1;
            }

            if (CheckCollisionCircleRec({ball->x, ball->y}, ball->radius, {player2->x, player2->y, player2->width, player2->height})) {
                ball->speed_x *= -1;
            }

            if (player1_score >= 5) {
                game_over = true;
                player1_wins_game = true;
                player1_wins++;
            } else if (player2_score >= 5) {
                game_over = true;
                player1_wins_game = false;
                player2_wins++;
            }

            DrawRectangle(screen_width / 2, 0, screen_width / 2, screen_height, Green);
            DrawCircle(screen_width / 2, screen_height / 2, 150, Light_Green);
            DrawDashedLine(screen_width / 2, screen_height, 20, 10);
            ball->Draw();
            player1->Draw();
            player2->Draw();
            DrawText(TextFormat("%i", player1_score), screen_width / 4 - 20, 20, 80, WHITE);
            DrawText(TextFormat("%i", player2_score), 3 * screen_width / 4 - 20, 20, 80, WHITE);
        } else {
            ClearBackground(Dark_Green);
            if (player1_wins_game) {
                DrawText("PLAYER 1 WINS!", screen_width / 2 - MeasureText("PLAYER 1 WINS!", 80) / 2, screen_height / 2 - 40, 80, Light_Green);
            } else {
                DrawText("PLAYER 2 WINS!", screen_width / 2 - MeasureText("PLAYER 2 WINS!", 80) / 2, screen_height / 2 - 40, 80, Yellow);
            }
            DrawText("Press ESC to exit", screen_width / 2 - MeasureText("Press ESC to exit", 20) / 2, screen_height - 100, 20, WHITE);
            DrawText("Press M to return to Menu", screen_width / 2 - MeasureText("Press M to return to Menu", 20) / 2, screen_height - 50, 20, WHITE);

            if (IsKeyPressed(KEY_M)) {
                player1_score = 0;
                player2_score = 0;
                ball->ResetBall();
                show_menu = true;
                game_over = false;
            }

            if (IsKeyPressed(KEY_ESCAPE)) {
                SaveStats(player1_wins, player2_wins);
                break;
            }
        }

        EndDrawing();
    }

    SaveStats(player1_wins, player2_wins);
    delete ball;
    delete player1;
    delete player2;
    CloseWindow();
    return 0;
}
