#ifndef H_MAIN
    #ifndef H_GAMECONSTS
        #include "gameconsts.h"
    #endif
    #ifndef _STDBOOL_H
        #include <stdbool.h>
    #endif
    #define H_MAIN
    bool InitLevel(int);
    int GetLevel();
    void ChangeGameState(int);
    GameState GetCurrentGameState();
    Color* GetChainColor(int);
    Cell* GetBoard();
    int GetBoardSize();
    Cell** GetLastsOfChains();
    int GetLastChainInt();
    void NextLastChainInt();
    bool IsSettingsMenuOpened();
    int GetNbOfChains();
    void SetSettingsMenuOpened(bool);
    Texture2D* GetTextures();
#endif