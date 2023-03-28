#ifndef H_GAMECONSTS
    #define H_GAMECONSTS
    #ifndef RAYLIB_H
        #include "raylib.h"
    #endif

    typedef enum {
        STATE_START,
        STATE_BOARD,
        STATE_WIN
    } GameState;

    typedef struct {
        Rectangle rect;
        int value;
        int x;
        int y;
        int chain;
    } Cell;
    
    #ifndef SCRREN_WIDTH
        #define SCRREN_WIDTH 1080
    #endif
    #ifndef SCREEN_HEIGHT
        #define SCREEN_HEIGHT 720
    #endif

#endif
