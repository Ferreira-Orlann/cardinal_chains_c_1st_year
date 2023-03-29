#ifndef H_GAMECONSTS
    #define H_GAMECONSTS
    #ifndef RAYLIB_H
        #include "raylib.h"
    #endif

    typedef enum {
        STATE_START,
        STATE_BOARD,
        STATE_WIN,
        STATE_LEVEL_FINISH
    } GameState;

    typedef struct {
        Rectangle rect;
        unsigned char value;
        unsigned char x;
        unsigned char y;
        unsigned char chain;
        unsigned char type;
    } Cell;

    typedef enum {
        CELLTYPE_NONE = (1 << 1),
        CELLTYPE_RIGHT = (1 << 2),
        CELLTYPE_LEFT = (1 << 3),
        CELLTYPE_TOP = (1 << 4),
        CELLTYPE_BOTTOM = (1 << 5),
    } CellType;

    #ifndef SCRREN_WIDTH
        #define SCRREN_WIDTH 1080
    #endif
    #ifndef SCREEN_HEIGHT
        #define SCREEN_HEIGHT 720
    #endif
#endif
