#include "raylib.h"
#include <stdio.h>
#include "gameconsts.h"
#include "main.h"

void DrawLevel();
void DrawBoard();
void DrawCell(Cell*);

void DrawFrame(GameState state) {
	BeginDrawing();
		ClearBackground(RAYWHITE);
		switch (state) {
			case STATE_START:
                DrawText("Start", (GetScreenWidth()/2)-60, (GetRenderHeight()/2)-60, 60, RED);
				break;
			case STATE_BOARD:
                DrawLevel();
				DrawBoard();
				break;
			case STATE_WIN:
                DrawLevel();
				DrawBoard();
				DrawText("YOU WON ALL THE LEVELS !!", (GetScreenWidth()/2)-400, (GetRenderHeight()/2)-250, 60, PURPLE);
				break;
			default:
				break;
		}
		DrawText(TextFormat("%d FPS", GetFPS()), 10, 10, 20, RED);
	EndDrawing();
}

void DrawLevel() {
    char text[12];
    sprintf(text, "Niveau %d", GetLevel());
    Vector2 textSize =  MeasureTextEx(GetFontDefault(), text, 30, 1);
    DrawText(text, (GetScreenWidth()/2)-(textSize.x/2), 20, 30, BLACK);
}

void DrawBoard() {
	Cell* board = GetBoard();
	int size = GetBoardSize();
	for (int x = 0; x < size; x++) {
        DrawCell(board+x);
	}
}

void DrawCell(Cell* cell) {
    if (cell->value >= 0) {
        Rectangle rect = cell->rect;
        Color color = *GetChainColor(cell->chain);
        DrawRectangleRec(rect, color);
        DrawRectangleLinesEx(rect, 2.0f, BLACK);
        if (cell->value == 0) {
            char* text = "X";   
            Vector2 textSize = MeasureTextEx(GetFontDefault(), text, 20, 1);
            DrawText(text, (rect.x + (rect.width/2)) - (textSize.x/2), (rect.y + (rect.height/2)) - (textSize.y/2), 20, BLACK);
        } else {
            const char* text = TextFormat("%d", cell->value);
            Vector2 textSize = MeasureTextEx(GetFontDefault(), text, 20, 1);
            DrawText(text, (rect.x + (rect.width/2)) - (textSize.x/2), (rect.y + (rect.height/2)) - (textSize.y/2), 20, BLACK);
        }
    }
}