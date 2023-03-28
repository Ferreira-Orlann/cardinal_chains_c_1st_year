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
    

#endif
