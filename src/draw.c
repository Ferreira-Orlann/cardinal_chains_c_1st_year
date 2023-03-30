#include "raylib.h"
#include <stdio.h>
#include "gameconsts.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "utils.h"
#include "main.h"
#include "update.h"

void DrawLevel(); //Draws the current level of the game.
void DrawBoard(); // Draws the board with all the cells;
void DrawCell(Cell*); //Draws each individual cell of the board;
void DrawUtilitaries();
void DrawSettingsMenu();

void DrawFrame(GameState state) { //Draws the main frame for the game;
	BeginDrawing();
		ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        Texture2D* textures = GetTextures();
        DrawTexture(*textures, 990, 25, WHITE);
		switch (state) {
			case STATE_START:
                DrawCenteredText("WELCOME TO ",SCRREN_WIDTH/2, SCREEN_HEIGHT/6, 60, ORANGE);
                DrawCenteredText("CARDINAL CHAINS GAME",SCRREN_WIDTH/2.1, SCREEN_HEIGHT/3.8, 60, ORANGE);
                DrawCenteredText("Start",SCRREN_WIDTH/2, SCREEN_HEIGHT/2, 60, RED);
                DrawCenteredText("Game created by Orlann Ferreira and Tommy Brisset",SCRREN_WIDTH/3.8, SCREEN_HEIGHT/1.08, 20, BLACK);
                GuiButton((Rectangle) {.x = 880,.y = 640,.width = 120,.height = 24}, "Open Level");
                break;
			case STATE_BOARD:
                DrawLevel();
				DrawBoard();
                DrawUtilitaries();
				break;
			case STATE_WIN:
                DrawLevel();
				DrawBoard();
                DrawCenteredText("YOU WON ALL THE LEVELS !!",SCRREN_WIDTH/2.3, SCREEN_HEIGHT/6, 60, PURPLE);
                DrawCenteredText("NEXT LEVELS COMING SOON ",SCRREN_WIDTH/2.1, SCREEN_HEIGHT/1.1, 40, BLACK);
                break;
            case STATE_LEVEL_FINISH:
                DrawLevel();
				DrawBoard();
                DrawCenteredText("LEVEL FINISH ",SCRREN_WIDTH/2, SCREEN_HEIGHT/6, 60, BLACK);
                DrawUtilitaries();
                break;
			default:
				break;
		}
        DrawFPS(10,10);
        if (IsSettingsMenuOpened()) {
            DrawSettingsMenu();
        }

	EndDrawing();
}

void DrawSettingsMenu() {
    if (!IsSettingsMenuOpened()) {
        return;
    }
    Rectangle windowsRect = { 740, 30, 250, 300 };
    bool closeButtonClick = GuiWindowBox(windowsRect, "Settings");
    UpdateStyleComboBox(GuiComboBox((Rectangle){ windowsRect.x + 5, windowsRect.y + 29, (float)windowsRect.width-10, 20}, "Style Defaut;Style Dark;Style Cyber;Style Terminal", GetCurrentStyle()));
    if (closeButtonClick) {
        UpdateSettingsMenu(closeButtonClick);
    }
}

void DrawUtilitaries() {
    GuiButton((Rectangle) {.x = 880,.y = 640,.width = 120,.height = 24}, "Save Level");
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
        DrawRectangleRec(rect, GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
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