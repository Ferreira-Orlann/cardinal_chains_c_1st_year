#include "raylib.h"
#include <stdio.h>
#include "gameconsts.h"
#include "main.h"
#include "utils.h"

void DrawLevel();
void DrawBoard();
void DrawCell(Cell*);
void DrawCenteredText(const char*, int, int, int, Color);

void DrawFrame(GameState state) {
	BeginDrawing();
		ClearBackground(RAYWHITE);
		switch (state) {
			case STATE_START:
                DrawCenteredText("Start",SCRREN_WIDTH/2, SCREEN_HEIGHT/2, 60, RED);
				break;
			case STATE_BOARD:
                DrawLevel();
				DrawBoard();
				break;
			case STATE_WIN:
                DrawLevel();
				DrawBoard();
                DrawCenteredText("YOU WON ALL THE LEVELS !!",SCRREN_WIDTH/2, SCREEN_HEIGHT/2, 60, PURPLE);
				break;
			default:
				break;
		}
        DrawFPS(10,10);
	EndDrawing();
}

void DrawCenteredText(const char* text, int x, int y, int fontSize, Color color) {
    Vector2 pos = {
        .x = x,
        .y = y
    };
    pos = CenterTextVec(text, pos, fontSize);
    DrawText(text, pos.x, pos.y, fontSize, color);
}

void DrawLevel() {
    char text[12];
    sprintf(text, "Niveau %d", GetLevel());
    DrawCenteredText(text, SCRREN_WIDTH/2, 20, 30, BLACK);
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
        int lineThick = 2.0f;
        if ((lineThick > rect.width) || (lineThick > rect.height)) {
            if (rect.width > rect.height) lineThick = rect.height/2;
            else if (rect.width < rect.height) lineThick = rect.width/2;
        }
        if (!(cell->type & CELLTYPE_TOP)) {
            Rectangle top = { rect.x, rect.y, rect.width, 2.0f };
            DrawRectangleRec(top, BLACK);
        }
        if (!(cell->type & CELLTYPE_BOTTOM)) {
            // Rectangle bottom = { rect.x, rect.y - lineThick + rect.height, rect.width, lineThick };
            Rectangle bottom = { rect.x, rect.y - lineThick + rect.height, rect.width + 1, lineThick };
            DrawRectangleRec(bottom, BLACK);
        }
        if (!(cell->type & CELLTYPE_RIGHT)) {
            // Rectangle right = { rect.x - lineThick + rect.width, rect.y + lineThick, lineThick, rect.height - lineThick*2.0f};
            Rectangle right = { rect.x - lineThick + rect.width, rect.y + lineThick, lineThick, rect.height - lineThick*2.0f};
            DrawRectangleRec(right, BLACK);
        }
        if (!(cell->type & CELLTYPE_LEFT)) {
            Rectangle left = { rect.x, rect.y + lineThick, lineThick, rect.height - lineThick*2.0f};
            DrawRectangleRec(left, BLACK);
        }
        if (cell->value == 0) {
            const char* text = "X";   
            DrawCenteredText(text, rect.x + (rect.width/2), rect.y + (rect.height/2), 20, BLACK);
        } else {
            const char* text = TextFormat("%d", cell->value);
            DrawCenteredText(text, rect.x + (rect.width/2), rect.y + (rect.height/2), 20, BLACK);
        }    
    }
}