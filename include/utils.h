#ifndef H_UTILS
    Vector2 CenterTextVec(const char*, Vector2, int, Font);
    Vector2 CenterText(const char*, int, int, int, Font);
    void DrawCenteredText(const char* text, int x, int y, int fontSize, Color color);
    void DrawCenteredText(const char*, int, int, int, Color); //Draws a centered text;
#endif