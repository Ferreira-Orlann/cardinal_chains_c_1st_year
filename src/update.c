#include "raylib.h"
#include "gameconsts.h"
#include "main.h"
#include "raygui.h"
#include "draw.h"
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include <unistd.h>
#include "save.h"
#include "styles/style_dark.h"
#include "styles/style_cyber.h"
#include "styles/style_terminal.h"

Cell* lastCollisionedCell = NULL;

void BoardUpdate();
void SleepUpdate();
Cell* GetCellByCollision(Vector2);

float sleepTime = 0;

void UpdateGame(GameState state) { //Updates the game state depending on the passed parameter.
    Vector2 mousePos = GetMousePosition();
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        Rectangle settingsButton = {.x = 990,.y = 25,.width = 64,.height = 64};
        if (CheckCollisionPointRec(mousePos, settingsButton)) {
            SetSettingsMenuOpened(!IsSettingsMenuOpened());
            return;
        }
    }
    switch (state) {
        case STATE_START:
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                char* text = "Start";
                Vector2 textStartPos = CenterText(text, SCRREN_WIDTH/2, SCREEN_HEIGHT/2,60, GuiGetFont());
                Vector2 textSize = MeasureTextEx(GuiGetFont(), text, 60, 1);
                Rectangle textRect = {
                    .width = textSize.x,
                    .height = textSize.y,
                    .x = textStartPos.x,
                    .y = textStartPos.y
                };
                if (CheckCollisionPointRec(mousePos, textRect) == true) {
                    ChangeGameState(STATE_BOARD);
                }
            }
            break;
        case STATE_BOARD:
            BoardUpdate();
            break;
        case STATE_LEVEL_FINISH:
            SleepUpdate();
            break;
        default:
            break;
    }
}

void UpdateLoadButton(bool clicked) {
    if (clicked) {
        if (LoadLevel() != false) {
            ChangeGameState(STATE_BOARD);
        }
    }
}

void UpdateSaveButton(bool clicked) {
    if (clicked) {
        if (SaveLevel() != false) {
            ChangeGameState(STATE_START);
        }
    }
}

void UpdateShowFps(int showFps) {
    if (showFps != ShouldShowFps()) {
        ShowFps();
    }
}

void SleepUpdate() { //Updates the sleep time counter until it reach 0
    sleepTime-=GetFrameTime();
    if (sleepTime <= 0.0f) {
        if (InitLevel(GetLevel()+1) == false) {
            ChangeGameState(STATE_WIN);
        } else {
            ChangeGameState(STATE_BOARD);
        }
    }
}

void UpdateSettingsMenu(bool closeButtonClick) {
    SetSettingsMenuOpened(!closeButtonClick);
}

void UpdateStyleComboBox(int style) {
    if (style == GetCurrentStyle()) {
        return;
    }
    SetCurrentStyle(style);
    switch (style) {
        case 0: GuiLoadStyleDefault(); break;
        case 1: GuiLoadStyleDark(); break;
        case 2: GuiLoadStyleCyber(); break;
        case 3: GuiLoadStyleTerminal(); break;
        default: break;
    }
}

void BoardUpdate() {
    int input = KEY_NULL;
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        input = MOUSE_BUTTON_LEFT;
    } else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        input = MOUSE_BUTTON_RIGHT;
    } else {
        return;
    }
    Cell* collosionedCell = GetCellByCollision(GetMousePosition());
    if (collosionedCell == NULL) {
        return;
    }
    int size = GetBoardSize();
    Cell* board = GetBoard();
    switch (input) {
        case MOUSE_BUTTON_LEFT:
        if (collosionedCell == NULL || collosionedCell->value < 0) { 
                return;
            }
            if (lastCollisionedCell == NULL) {
                lastCollisionedCell = collosionedCell;
            }
            if (collosionedCell->value == 0) {
                if (collosionedCell->chain == 0) {
                    *(GetLastsOfChains()+GetLastChainInt()) = collosionedCell;
                    NextLastChainInt();
                    collosionedCell->chain = GetLastChainInt();
                };
            }
            Cell** lastOfChain = GetLastsOfChains()+(lastCollisionedCell->chain-1);
            if (collosionedCell->chain == 0 && 
                lastCollisionedCell->chain != 0 && 
                collosionedCell->chain != lastCollisionedCell->chain && 
                collosionedCell->value >= lastCollisionedCell->value &&
                lastCollisionedCell == *lastOfChain)
            {
                printf("Conditions: %d - %d\n", abs((*lastOfChain)->x - collosionedCell->x) == 1, abs((*lastOfChain)->y - collosionedCell->y) == 1);
                int abs1 = abs((*lastOfChain)->x - collosionedCell->x);
                int abs2 = abs((*lastOfChain)->y - collosionedCell->y);
                // if (((abs((*lastOfChain)->x - collosionedCell->x) == 1) != (abs((*lastOfChain)->y - collosionedCell->y) == 1))) {
                if (((abs1 == 1) != (abs2 == 1)) && ((abs1 < 2) && (abs2 < 2))) {
                    collosionedCell->chain = lastCollisionedCell->chain;
                    printf("lastOfChain (x,y): %d - %d; collosionedCell(x,y) %d - %d; Conditions: %d - %d\n", (*lastOfChain)->x, (*lastOfChain)->y, collosionedCell->x, collosionedCell->y, abs((*lastOfChain)->x - collosionedCell->x) == 1, abs((*lastOfChain)->y - collosionedCell->y) == 1);
                    *lastOfChain = collosionedCell;
                    if (collosionedCell->y > lastCollisionedCell->y) {
                        collosionedCell->type = collosionedCell->type | CELLTYPE_TOP;
                        lastCollisionedCell->type = lastCollisionedCell->type | CELLTYPE_BOTTOM;
                    } else if (collosionedCell->y < lastCollisionedCell->y) {
                        collosionedCell->type = collosionedCell->type | CELLTYPE_BOTTOM;
                        lastCollisionedCell->type = lastCollisionedCell->type | CELLTYPE_TOP;
                    } else if (collosionedCell->x > lastCollisionedCell->x) {
                        collosionedCell->type = collosionedCell->type | CELLTYPE_LEFT;
                        lastCollisionedCell->type = lastCollisionedCell->type | CELLTYPE_RIGHT;
                    } else if (collosionedCell->x < lastCollisionedCell->x) {
                        collosionedCell->type = collosionedCell->type | CELLTYPE_RIGHT;
                        lastCollisionedCell->type = lastCollisionedCell->type | CELLTYPE_LEFT;
                    }
                }
            }
            if (collosionedCell != lastCollisionedCell) {
                lastCollisionedCell = collosionedCell;
            }
            for (int i = 0; i < size; i++) {
                Cell* cell = board+i;
                if (cell->chain == 0 && cell->value != -1) {
                    break;
                }
                if (i == (size-1)) {
                    ChangeGameState(STATE_LEVEL_FINISH);
                    sleepTime = 1.2f;
                }
            }
            break;
        case MOUSE_BUTTON_RIGHT:
            int chain = collosionedCell->chain;
            if (chain == 0 || collosionedCell->value != 0) {
                return;
            }
            for (int i = 0; i < size; i++) {
                Cell* cell2 = board+i;
                if (cell2->chain == chain) {
                    if(cell2->value != 0) {
                        cell2->chain = 0;
                    } else {
                        *(GetLastsOfChains()+(collosionedCell->chain-1)) = collosionedCell;
                    }
                    cell2->type = CELLTYPE_NONE;
                }
            }
            lastCollisionedCell = NULL;
            break;
        default:
            break;
    }  
}

Cell* GetCellByCollision(Vector2 pos) {
    int size = GetBoardSize();
    Cell* board = GetBoard();
    for (int i = 0; i < size; i++) {
        Cell* cell = board+i;
        if (CheckCollisionPointRec(pos, cell->rect) == true) {
            return cell;
        }
    }
    return NULL;
}
