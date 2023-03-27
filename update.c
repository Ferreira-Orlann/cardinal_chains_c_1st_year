#include "raylib.h"
#include "gameconsts.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>

Cell* lastCollisionedCell = NULL;

void UpdateGame(GameState state) {
    switch (state) {
        case STATE_START:
            Vector2 textSize =  MeasureTextEx(GetFontDefault(), "Start", 20, 1);
            Rectangle textRect = {
                .width = textSize.x,
                .height = textSize.y,
                .x = (GetScreenWidth()/2)-21,
                .y = (GetRenderHeight()/2)-10
            };
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), textRect) == true) {
                ChangeGameState(STATE_BOARD);
            }
            break;
        case STATE_BOARD:
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
                                }
                            }
                            lastCollisionedCell = NULL;
                        }
                    }
                }
            }
            if (collosionedCell == NULL || collosionedCell->value < 0) { 
                break;
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
            if (collosionedCell->chain == 0 && 
                lastCollisionedCell->chain != 0 && 
                collosionedCell->chain != lastCollisionedCell->chain && 
                collosionedCell->value >= lastCollisionedCell->value)
            {
                Cell** lastOfChain = GetLastsOfChains()+(lastCollisionedCell->chain-1);
                if (((abs((*lastOfChain)->x - collosionedCell->x) == 1) != (abs((*lastOfChain)->y - collosionedCell->y) == 1))) {
                    collosionedCell->chain = lastCollisionedCell->chain;
                    *lastOfChain = collosionedCell;
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
                    if (InitLevel(GetLevel()+1) == false) {
                        ChangeGameState(STATE_WIN);
                    }
                }
            }
            break;
        default:
            break;
    }
}