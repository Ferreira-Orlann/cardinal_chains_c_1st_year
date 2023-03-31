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
                DrawCenteredText("Game created by Orlann Ferreira and Tommy Brisset",SCRREN_WIDTH/3.8, SCREEN_HEIGHT/1.10, 20, BLACK);
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
        if (ShouldShowFps()) {
            DrawFPS(10,10);
        }
        if (IsSettingsMenuOpened()) {
            DrawSettingsMenu();
        }

	EndDrawing();
}

void DrawSettingsMenu() {
    if (!IsSettingsMenuOpened()) {
        return;
    }
    Rectangle windowsRect = { 790, 30, 200, 300 };
    bool closeButtonClick = GuiWindowBox(windowsRect, "Settings");
    UpdateStyleComboBox(GuiComboBox((Rectangle){ windowsRect.x + 5, windowsRect.y + 29, (float)windowsRect.width-10, 20}, "Style Defaut;Style Dark;Style Cyber;Style Terminal", GetCurrentStyle()));
    UpdateShowFps(GuiCheckBox((Rectangle){ windowsRect.x + 5, windowsRect.y + 54, (float)20, 20}, "Affichier les PFS", ShouldShowFps()));
    if (GetCurrentGameState() == STATE_START) {
        UpdateLoadButton(GuiButton((Rectangle){ windowsRect.x + 5, windowsRect.y + 79, (float)windowsRect.width-10, 20}, "Load Level"));
    } else {
        UpdateSaveButton(GuiButton((Rectangle){ windowsRect.x + 5, windowsRect.y + 79, (float)windowsRect.width-10, 20}, "Save Level"));
    }
    if (closeButtonClick) {
        UpdateSettingsMenu(closeButtonClick);
    }
}

void DrawUtilitaries() {}

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
        Color color = GetChainColor(cell->chain);
        DrawRectangleRec(rect, GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        int lineThick = 1.5f;
        if ((lineThick > rect.width) || (lineThick > rect.height)) {
            if (rect.width > rect.height) lineThick = rect.height/2;
            else if (rect.width < rect.height) lineThick = rect.width/2;
        }
        if (!(cell->type & CELLTYPE_TOP)) {
            Rectangle top = { rect.x - lineThick, rect.y - lineThick, rect.width + lineThick*2.0f, lineThick*2.0f };
            DrawRectangleRec(top, BLACK);
        }
        if (!(cell->type & CELLTYPE_BOTTOM)) {
            Rectangle bottom = { rect.x - lineThick, rect.y + rect.height - lineThick, rect.width + lineThick*2.0f, lineThick*2.0f };
            DrawRectangleRec(bottom, BLACK);
        }
        if (!(cell->type & CELLTYPE_RIGHT)) {
            Rectangle right = { rect.x + rect.width - lineThick, rect.y - lineThick, lineThick*2.0f, rect.height + lineThick*2.0f };
            DrawRectangleRec(right, BLACK);
        }
        if (!(cell->type & CELLTYPE_LEFT)) {
            Rectangle left = { rect.x - lineThick, rect.y - lineThick, lineThick*2.0f, rect.height + lineThick*2.0f };
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
