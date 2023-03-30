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

    // Size: 21 Bytes / 168 Bits
    typedef struct {
        Rectangle rect; // 16 bytes (4 floats -> 4 * 4 Bytes) / 128 Bits
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

    #define SCRREN_WIDTH 1080
    #define SCREEN_HEIGHT 720
#endif
