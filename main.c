#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gameconsts.h"
#include "draw.h"
#include "update.h"
#include "main.h"
#include <math.h>
#include "jsmn.h"

int screenWidth = 1080;
int screenHeight = 720;
GameState currentGameState = STATE_START;
Color chainColors[] = {
    RAYWHITE, RED, GREEN, BLUE
};
Cell* board = NULL;
int boardSize = 0;
Cell** lastsOfChains;
int nbOfChains = 0;
int level;
int lastChain = 0;

Color* GetChainColor(int chain) {
    return &chainColors[chain];
}

Cell* GetBoard() {
    return board;
}

int GetBoardSize() {
    return boardSize;
}

void ChangeGameState(int state) {
    currentGameState = state;
}

GameState GetCurrentGameState() {
    return currentGameState;
}

int GetLevel() {
    return level;
}

Cell** GetLastsOfChains() {
    return lastsOfChains;
}

bool InitLevel(int name) {
    char nb[2];
    itoa(name,nb,10);
    FILE* ptr;
    char path[50] = "levels/";
    strcat(path, nb);
    strcat(path, ".json");
    char json[256];
    ptr = fopen(path, "r");
    if (NULL == ptr) {
        return false;
    }
    fgets(json, 256, ptr);
    fclose(ptr);
    jsmn_parser parser;
    jsmn_init(&parser);
    jsmntok_t tokens[256];
    int jsonSize = jsmn_parse(&parser, json, strlen(json), tokens, 256)-1;
    Vector2 center = {
        .x = GetScreenWidth()/2,
        .y = GetScreenHeight()/2
    };
    int collumSize = (int) ceilf(sqrtf((float) jsonSize));
    if (board != NULL) {
        free(board);
    }
    boardSize = 0;
    for (int i = 0; i < jsonSize; i++) {
        char charVal[2];
        sprintf(charVal, "%.*s\n", tokens[i+1].end - tokens[i+1].start, json + tokens[i+1].start);
        int val = atoi(charVal);
        if (val >= 0) {
            boardSize++;
            continue;
        }
    }
    board = malloc(sizeof(Cell)*boardSize);
    int startX = center.x-((50*collumSize)/2);
    int startY = center.y-((50*collumSize)/2);
    int position = 0;
    int offset = 0;
    int localNbOfChains = 0;
    for (int i = 0; i < collumSize; i++) {
        for (int b = 0; b < collumSize; b++) {
            char charVal[2];
            sprintf(charVal, "%.*s\n", tokens[position+1].end - tokens[position+1].start, json + tokens[position+1].start);
            int val = atoi(charVal);
            position++;
            if (val < 0) {
                continue;
            }
            if (val == 0) {
                localNbOfChains++;
            }
            Cell cell = {
                .rect = {
                    .height = 50,
                    .width = 50,
                    .x = startX+(b*50),
                    .y = startY+(i*50),
                },
                .value = val,
                .x = b,
                .y = i,
                .chain = 0
            };
            board[offset] = cell;
            offset++;
        }
    }
    for (int i = 0; i < nbOfChains; i++) {
        *(lastsOfChains+i) = NULL;
    }
    level = name;
    lastChain = 0;
    nbOfChains = localNbOfChains;
    lastsOfChains = malloc(sizeof(Cell*)*nbOfChains);
    return true;
}

int GetLastChainInt() {
    return lastChain;
}

void NextLastChainInt() {
    lastChain++;
}

int main() {
    InitWindow(screenWidth, screenHeight, "Cardinal Chains");
    Image icon = LoadImage("./assets/icon.png");
    SetWindowIcon(icon);
    UnloadImage(icon);
    SetTargetFPS(60);

    InitLevel(1);

    while (!WindowShouldClose()) {
        UpdateGame(currentGameState);
        DrawFrame(currentGameState);
    }

    CloseWindow();
    return 0;
}