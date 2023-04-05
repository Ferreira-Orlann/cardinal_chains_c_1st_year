#include "raylib.h"
#include "raygui.h"

Vector2 CenterText(const char* text, int x, int y, int fontSize, Font font) { //Calculates the starting point for centering text based on the given position, font size, and text content
    Vector2 textSize =  MeasureTextEx(font, text, fontSize, 1);
    Vector2 startPoint = {
        .x = x-(textSize.x/2),
        .y = y-(textSize.y/2)
    };
    return startPoint;
}

Vector2 CenterTextVec(const char* text, Vector2 pos, int fontSize, Font font) { //Wrapper function for CenterText that takes a Vector2 position argument
    return CenterText(text, pos.x, pos.y, fontSize, font);
}

void DrawCenteredText(const char* text, int x, int y, int fontSize, Color color) { //Draws text centered on the given position using the GuiLabel function from the raygui library
    int fontSizeTemp = GuiGetStyle(DEFAULT, TEXT_SIZE);
    GuiSetStyle(DEFAULT, TEXT_SIZE, fontSize);
    fontSize = (float)GuiGetStyle(DEFAULT, TEXT_SIZE);
    Vector2 pos = CenterText(text, x, y, fontSize, GuiGetFont());
    Vector2 textSize =  MeasureTextEx(GuiGetFont(), text, fontSize, 1);
    GuiLabel((Rectangle){pos.x, pos.y, textSize.x, textSize.y}, text);
    GuiSetStyle(DEFAULT, TEXT_SIZE, fontSizeTemp);
}