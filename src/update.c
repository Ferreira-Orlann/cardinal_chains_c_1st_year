#include "raylib.h"
#include "gameconsts.h"
#include "main.h"
#include "draw.h"
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include <unistd.h>

Cell* lastCollisionedCell = NULL;

void BoardUpdate();
void SleepUpdate();

float sleepTime = 0;

void UpdateGame(GameState state) {
    switch (state) {
        case STATE_START:
            char* text = "Start";
            Vector2 textStartPos = CenterText(text, SCRREN_WIDTH/2, SCREEN_HEIGHT/2,60);
            Vector2 textSize = MeasureTextEx(GetFontDefault(), text, 60, 1);
            Rectangle textRect = {
                .width = textSize.x,
                .height = textSize.y,
                .x = textStartPos.x,
                .y = textStartPos.y
            };
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), textRect) == true) {
                ChangeGameState(STATE_BOARD);
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

void SleepUpdate() {
    sleepTime = sleepTime - GetFrameTime();
    if (sleepTime <= 0.0f) {
        if (InitLevel(GetLevel()+1) == false) {
            ChangeGameState(STATE_WIN);
        } else {
            ChangeGameState(STATE_BOARD);
        }
    }
}

void BoardUpdate() {
            Cell* collosionedCell = NULL;
            Cell* board = GetBoard();
            int size = GetBoardSize();
            {
                for (int i = 0; i < size; i++) {
                    Cell* cell = board+i;
                    if (CheckCollisionPointRec(GetMousePosition(), cell->rect) == true) {
                        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) == true) {
                            collosionedCell = cell;
                        }
                        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) == true) {
                            int chain = cell->chain;
                            if (chain == 0 || cell->value != 0) {
                                continue;
                            }
                            for (int i = 0; i < size; i++) {
                                Cell* cell2 = board+i;
                                if (cell2->chain == chain) {
                                    if(cell2->value != 0) {
                                        cell2->chain = 0;
                                    } else {
                                        *(GetLastsOfChains()+(cell->chain-1)) = cell;
                                    }
                                    cell2->type = CELLTYPE_NONE;
                                }
                            }
                            lastCollisionedCell = NULL;
                        }
                    }
                }
            }
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
}