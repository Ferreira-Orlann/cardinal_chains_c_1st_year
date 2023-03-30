#ifndef H_MAIN
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
#endif