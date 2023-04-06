#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "raylib.h"
#include "gameconsts.h"
#include "draw.h"
#include "update.h"
#include "main.h"
#include "jsmn.h"
#include "raygui.h"

void LoadTextures();
void ChangeColorSet(Color*);

static GameState currentGameState = STATE_START;   //Stores the current state of the game.
static Color colors[4][8] = { //An array of colors used to represent each chain of different gui style.
    {RAYWHITE, RED, GREEN, BLUE,YELLOW,PURPLE,ORANGE,PINK},{RAYWHITE, GREEN, RED, YELLOW,BLUE,PURPLE,ORANGE,PINK},{RAYWHITE, RED, GREEN, BLUE,YELLOW,PURPLE,ORANGE,PINK},{RAYWHITE, RED, LIME,PURPLE, BLUE,MAGENTA,ORANGE,PINK}
};
static Color chainColors[8] = { //An array of colors used to represent each chain.
    RAYWHITE, RED, GREEN, BLUE, YELLOW,PURPLE,ORANGE,PINK
};
static Cell* board = NULL;  //A ,dynamically allocated array of Cell structs representing the game board.
static int boardSize = 0;  //The number of cells in the game board.
static Cell** lastsOfChains;  //An array of pointers to the last cell in each chain.
static int nbOfChains = 0;  //The number of chains in the game.
static int level; //The current level.
static int lastChain = 0; //The ID of the last chain.
static bool settingsMenuOpened = false;
static Texture2D* texturesList = NULL;
static int currentStyle = 0;
static bool showFps = false;

void ShowFps() {  //Toggles the display of frames per second (FPS)
    showFps = !showFps;
}

bool ShouldShowFps() { //Returns whether or not to display the FPS
    return showFps;
}

int GetCurrentStyle() { //Returns the current style of the game
    return currentStyle;
}

void SetCurrentStyle(int style) { //Sets the current style of the game to the given value
    currentStyle = style;
    
    switch (style) {
        case 0: ChangeColorSet(colors[0]); break;
        case 1: ChangeColorSet(colors[1]); break;
        case 2: ChangeColorSet(colors[2]); break;
        case 3: ChangeColorSet(colors[3]); break;
        default: break;
    }
}

void ChangeColorSet(Color* colors) {
    for (int i = 0; i < 9; i++) {
        if (i == 0) {
            chainColors[i] = GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR));
            continue;
        }
        chainColors[i] = *(colors+i);
    }
}

Texture2D* GetTextures() { //Returns the list of textures
    return texturesList;
}

void LoadTextures() { //Loads the game textures
    texturesList = malloc(sizeof(Texture2D)*1);
    Image settingsIconImg = LoadImage("assets/settings.png");
    *texturesList = LoadTextureFromImage(settingsIconImg);
    UnloadImage(settingsIconImg);
}

bool IsSettingsMenuOpened() { //Returns whether or not the settings menu is open
    return settingsMenuOpened;
}

void SetSettingsMenuOpened(bool val) { //Sets the state of the settings menu
    settingsMenuOpened = val;
}

int GetNbOfChains() { //Returns the number of chains in the game
    return nbOfChains;
}

Color GetChainColor(int chain) { //Returns a pointer to the color of the specified chain.
    return chainColors[chain];
}

Cell* GetBoard() { //Returns a pointer to the game board array.
    return board;
}

int GetBoardSize() { //Returns the number of cells in the game board.
    return boardSize;
}

void ChangeGameState(int state) { //Changes the current game state to the specified state.
    currentGameState = state;
}

GameState GetCurrentGameState() { //Returns the current game state.
    return currentGameState;
}

int GetLevel() { //Returns the current level being.
    return level;
}

Cell** GetLastsOfChains() { //Returns a pointer to the array of pointers to the last cell in each chain.
    return lastsOfChains;
}

bool InitLevel(int name) { //Initializes the game board by a specified level.
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
    if (lastsOfChains != NULL) {
        free(lastsOfChains);
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
                .chain = 0,
                .type = CELLTYPE_NONE
            };
            if (val == 0) {
                localNbOfChains++;
            }
            board[offset] = cell;
            offset++;
        }
    }
    level = name;
    lastChain = 0;
    nbOfChains = localNbOfChains;
    lastsOfChains = malloc(sizeof(Cell*)*nbOfChains);
    for (int i = 0; i < nbOfChains; i++) {
        *(lastsOfChains+i) = NULL;
    }
    return true;
}

int GetLastChainInt() { //Returns the ID of the last chain created.
    return lastChain;
}

void NextLastChainInt() { //Increments the ID of the last chain created.
    lastChain++;
}

int main() { //The main function of the program that initializes the game window, loads the game icon, initializes the first level, updates the game state, and draws the game frame.
    InitWindow(SCRREN_WIDTH, SCREEN_HEIGHT, "Cardinal Chains");
    Image icon = LoadImage("./assets/icon.png");
    SetWindowIcon(icon);
    UnloadImage(icon);
    SetTargetFPS(60);
    LoadTextures();
    InitLevel(32);
    while (!(WindowShouldClose() && !IsKeyDown(KEY_ESCAPE))) {
        UpdateGame(currentGameState);
        DrawFrame(currentGameState);
    }
    CloseWindow();
    free(lastsOfChains);
    free(board);
    return 0;
}