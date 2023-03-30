#include <stdio.h>
#include "save.h"
#include "main.h"

/*

level id: int
GetLastChainInt : int
LastCells: unsigned char (x); unsigned char(y);
Cells: unsigned char (chain); 

*/

Cell* GetCellByLocation(unsigned char x, unsigned char y);

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
    // Last of chains
    Cell** lastOfChains = GetLastsOfChains();
    for (int i = 0; i < nbOfChains; i++) {
        if (i >= lastChain) {
            break;
        }
        Cell* cell = *(lastOfChains + i);
        unsigned char buffer[2] = {cell->x, cell->y};
        fwrite(&buffer[0], sizeof(unsigned char), 2, file);
    }
    // Last of chains & type
    int size = GetBoardSize();
    Cell* board = GetBoard();
    for (int i = 0; i < size; i++) {
        Cell* cell = board+i;
        unsigned char chain = cell->chain;
        fwrite(&chain, sizeof(unsigned char), 1, file);
        unsigned char type = cell->type;
        fwrite(&type, sizeof(unsigned char), 1, file);
    }
    fclose(file);

    return true;
}

bool LoadLevel() {
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
    // Last of chains
    Cell** lastOfChains = GetLastsOfChains();
    for (int i = 0; i < lastChain; i++) {
        unsigned char x = 255;
        unsigned char y = 255;
        fread(&x,sizeof(unsigned char),1,file);
        fread(&y,sizeof(unsigned char),1,file);
        Cell* cell = GetCellByLocation(x,y);
        if (x != 255 && y != 255 && cell != NULL) {
            *(lastOfChains+i) = cell;
        }
    }
    // Chains & type
    int size = GetBoardSize();
    Cell* board = GetBoard();
    for (int i = 0; i < size; i++) {
        Cell* cell = board+i;
        unsigned char chain = -1;
        unsigned char type = CELLTYPE_NONE;
        fread(&chain,sizeof(unsigned char),1,file);
        fread(&type,sizeof(unsigned char),1,file);
        if (chain != -1) {
            cell->chain = chain;
            cell->type = type;
        }
    }

    ChangeGameState(STATE_BOARD);
    fclose(file);
    return true;
}

Cell* GetCellByLocation(unsigned char x, unsigned char y) {
    int size = GetBoardSize();
    Cell* board = GetBoard();
    for (int i = 0; i < size; i++) {
        Cell* cell = board+i;
        if (cell->x == x && cell->y == y) {
            return cell;
        }
    }
    return NULL;
}