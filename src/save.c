#include <stdio.h>
#include "save.h"
#include "main.h"

/*

level id: int
GetLastChainInt : int
LastCells: int (x); int(y);
Cells: int (x); int(y) chain;

*/

Cell* GetCellByLocation(int x, int y);

bool SaveLevel() {
    FILE *file = fopen("data.save","wb");
    if (file == NULL) {
        return false;
    }
    int level = GetLevel();
    fwrite(&level,sizeof(int),1,file);
    int nbOfChains = GetNbOfChains();
    // fwrite(&nbOfChains,sizeof(int),1,file);
    int lastChain = GetLastChainInt();
    fwrite(&lastChain,sizeof(int),1,file);
    Cell** lastOfChains = GetLastsOfChains();
    for (int i = 0; i < nbOfChains; i++) {
        if (i >= lastChain) {
            break;
        }
        Cell* cell = *(lastOfChains + i);
        int buffer[2] = {cell->x, cell->y};
        fwrite(&buffer[0], sizeof(int), 2, file);
    }
    fclose(file);
}

bool LoadLevel() {
    printf("LoadLevel\n");
    FILE *file = fopen("data.save","rb");
    if (file == NULL) {
        return false;
    }
    int level = 0;
    fread(&level,sizeof(int),1,file);
    if (level != 0) {
        InitLevel(level);
    }
    int lastChain = 0;
    fread(&lastChain,sizeof(int),1,file);
    Cell** lastOfChains = GetLastsOfChains();
    for (int i = 0; i < lastChain; i++) {
        int x = -1;
        int y = -1;
        fread(&x,sizeof(int),1,file);
        fread(&y,sizeof(int),1,file);
        Cell* cell = GetCellByLocation(x,y);
        if (x != -1 && y != -1 && cell != NULL) {
            *(lastOfChains+i) = cell;
        }
    }

    ChangeGameState(STATE_BOARD);
    fclose(file);
}

Cell* GetCellByLocation(int x, int y) {


    return NULL;
}