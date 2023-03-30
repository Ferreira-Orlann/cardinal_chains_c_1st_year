#include "raylib.h"

Vector2 CenterText(char* text, int x, int y, int fontSize) {
    Vector2 textSize =  MeasureTextEx(GetFontDefault(), text, fontSize, 1);
    Vector2 startPoint = {
        .x = x-(textSize.x/2),
        .y = y-(textSize.y/2)
    };
    return startPoint;
}

Vector2 CenterTextVec(char* text, Vector2 pos, int fontSize) {
    return CenterText(text, pos.x, pos.y, fontSize);
}