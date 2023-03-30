#include "raylib.h"

Vector2 CenterText(const char* text, int x, int y, int fontSize) {
    Vector2 textSize =  MeasureTextEx(GetFontDefault(), text, fontSize, 1);
    Vector2 startPoint = {
        .x = x-(textSize.x/2),
        .y = y-(textSize.y/2)
    };
    return startPoint;
}

Vector2 CenterTextVec(const char* text, Vector2 pos, int fontSize) {
    return CenterText(text, pos.x, pos.y, fontSize);
}

void DrawCenteredText(const char* text, int x, int y, int fontSize, Color color) {
    Vector2 pos = {
        .x = x,
        .y = y
    };
    pos = CenterTextVec(text, pos, fontSize);
    DrawText(text, pos.x, pos.y, fontSize, color);
}