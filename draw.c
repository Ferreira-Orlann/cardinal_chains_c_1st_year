#include "raylib.h"
#include <stdio.h>
#include "gameconsts.h"
#include "main.h"

void DrawBoard();

void UpdateDrawFrame(GameState state) {
	BeginDrawing();
		ClearBackground(RAYWHITE);
		switch (state) {
			case STATE_START:
				DrawText("Start", (GetScreenWidth()/2)-21, (GetRenderHeight()/2)-10, 20, RED);
				break;
			case STATE_BOARD:
				DrawBoard();
				break;
			case STATE_WIN:
				DrawBoard();
				DrawText("YOU WON!", (GetScreenWidth()/2), (GetRenderHeight()/2), 50, PURPLE);
				break;
			default:
				break;
		}
		DrawText(TextFormat("%d FPS", GetFPS()), 10, 10, 20, RED);
	EndDrawing();
}

void DrawBoard() {
	Cell* board = GetBoard();
	int size = GetBoardSize();
	for (int x = 0; x < size; x++) {
		Cell* cell = board+x;
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
				char* text = TextFormat("%d", cell->value);
				Vector2 textSize = MeasureTextEx(GetFontDefault(), text, 20, 1);
				DrawText(text, (rect.x + (rect.width/2)) - (textSize.x/2), (rect.y + (rect.height/2)) - (textSize.y/2), 20, BLACK);
			}
		}
	}
}