#include <iostream>
#include <fstream>
#include <string>
#include "logic.h"

using std::cout, std::endl, std::ifstream, std::string;

/**
 * TODO: Student implement this function
 * Load representation of the dungeon level from file into the 2D map.
 * Calls createMap to allocate the 2D array.
 * @param   fileName    File name of dungeon level.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object by reference to set starting position.
 * @return  pointer to 2D dynamic array representation of dungeon map with player's location., or nullptr if loading fails for any reason
 * @updates  maxRow, maxCol, player
 */
char** loadLevel(const string& fileName, int& maxRow, int& maxCol, Player& player) {
    ifstream thefile;
    thefile.open(fileName);

    if (!thefile.is_open()) {
        return nullptr;
    }
    
    int height, width;
    thefile >> height>> width;
    if(thefile.fail()){
        return nullptr;
    }
    

    if((width <= 1 && height <= 1) ||(width < 1 || height < 1)){
        return nullptr;
    }
    


    int totalcells = 0;
    for(int i = 0; i < height; i++){
        if(INT32_MAX - width < totalcells){
            return nullptr;
        }
        else{
            totalcells += width;
        }
    }
    if(width > 999999 || height > 999999){
        return nullptr;
    }
    //assigns refrence if stream states pass
    maxRow = height;
    maxCol = width;

    //assigns player position
    int start_row, start_col;
    thefile >> start_row>> start_col;
    if(thefile.fail()){
        return nullptr;
    }
    if(start_row >= maxRow ||start_row < 0|| start_col >= maxCol  || start_col < 0){
        return nullptr;
    }
   
    player.row = start_row;
    player.col = start_col;
    char** map = createMap(maxRow,maxCol);
    for(int r = 0; r < maxRow; r++){
        for(int c = 0; c < maxCol; c++){
            char temp;
            thefile >> temp;
            if(thefile.fail()){
                return nullptr;
            }
            if(temp != TILE_AMULET && temp != TILE_DOOR && temp != TILE_EXIT && temp != TILE_MONSTER && temp != TILE_OPEN && temp != TILE_PILLAR && temp != TILE_TREASURE){
                return nullptr;
            }
            map[r][c] = temp;
        }
    }
    map[start_row][start_col] = TILE_PLAYER;

    bool hasDOOR = false;

    for(int r = 0; r < maxRow; r++){
        for(int c = 0; c < maxCol; c++){
            if(map[r][c] == TILE_DOOR || map[r][c] == TILE_EXIT){
               hasDOOR = true; 
            }
            
        }
    }

    if(hasDOOR == false){
        return nullptr;
    }


    char overlimit;
    if(thefile >> overlimit){
        return nullptr;
    }
    thefile.close();

    return map;
    return nullptr; 

}

/**
 * TODO: Student implement this function
 * Translate the character direction input by the user into row or column change.
 * That is, updates the nextRow or nextCol according to the player's movement direction.
 * @param   input       Character input by the user which translates to a direction.
 * @param   nextRow     Player's next row on the dungeon map (up/down).
 * @param   nextCol     Player's next column on dungeon map (left/right).
 * @updates  nextRow, nextCol
 */
void getDirection(char input, int& nextRow, int& nextCol) {
    if(input == MOVE_UP){
        nextRow -= 1;
    }
    else if(input == MOVE_DOWN){
        nextRow += 1;
    }
    else if(input == MOVE_LEFT){
        nextCol -= 1;
    }
    else if(input == MOVE_RIGHT){
        nextCol +=1;
    }
}

/**
 * TODO: [suggested] Student implement this function
 * Allocate the 2D map array.
 * Initialize each cell to TILE_OPEN.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @return   2D map array for the dungeon level, holds char type.
 */
char** createMap(int maxRow, int maxCol) {
    //does it have good dimensions
    if(maxRow <= 0 || maxCol <= 0){
        return nullptr;
    }
    //alocate memory
    char** matrix = new char*[maxRow];

    for(int y = 0; y < maxRow; y++){
        matrix[y] = new char[maxCol];
    }

    //initialize matrix with tileopen
    for(int y = 0; y < maxRow; y++){
        for(int x = 0; x < maxCol; x++){
            matrix[y][x] = TILE_OPEN;
        }
    }
    return matrix;
}

/**
 * TODO: Student implement this function
 * Deallocates the 2D map array.
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @return None
 * @update map, maxRow
 */
void deleteMap(char**& map, int& maxRow) {
    if( map != nullptr){
        for(int i=0; i<maxRow; i++){
            delete[] map[i];
        }
        
        delete[] map;
        map= nullptr;
        maxRow = 0;
    }
    else{
        maxRow = 0;
    }
}

/**
 * TODO: Student implement this function
 * Resize the 2D map by doubling both dimensions.
 * Copy the current map contents to the right, diagonal down, and below.
 * Do not duplicate the player, and remember to avoid memory leaks!
 * You can use the STATUS constants defined in logic.h to help!
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height), to be doubled.
 * @param   maxCol      Number of columns in the dungeon table (aka width), to be doubled.
 * @return  pointer to a dynamically-allocated 2D array (map) that has twice as many columns and rows in size.
 * @update maxRow, maxCol
 */
char** resizeMap(char** map, int& maxRow, int& maxCol) {
    char** bigger = createMap(2*maxRow, 2*maxCol);
  //check dimensions  
    if (map == NULL || maxRow > 999999/2 || maxRow <= 0 || maxCol > 999999/2 || maxCol <= 0) {
    return nullptr;
}
    
    //finda player position
    int playerRow;
    int playerCol;
    for(int r = 0; r < maxRow; r++){
        for(int c = 0; c < maxCol; c++){
            if(map[r][c] == TILE_PLAYER){
                playerRow = r;
                playerCol = c;
                map[r][c] = TILE_OPEN;
            }
        }
    }
    //put in data
    for(int r = 0; r < maxRow; r++){
        for(int c = 0; c < maxCol; c++){
            bigger[r][c] = map[r][c];
            bigger[r + maxRow][c] = map[r][c];
            bigger[r][c + maxCol] = map[r][c];
            bigger[r + maxRow][c + maxCol] = map[r][c];
        }
    }
    bigger[playerRow][playerCol] = TILE_PLAYER;
    
    for(int i = 0; i < maxRow; i++) {
        delete[] map[i];
    }
    delete[] map;
    maxRow = maxRow * 2;
    maxCol = maxCol * 2;

    return bigger;
    return nullptr;
}

/**
 * TODO: Student implement this function
 * Checks if the player can move in the specified direction and performs the move if so.
 * Cannot move out of bounds or onto TILE_PILLAR or TILE_MONSTER.
 * Cannot move onto TILE_EXIT without at least one treasure. 
 * If TILE_TREASURE, increment treasure by 1.
 * Remember to update the map tile that the player moves onto and return the appropriate status.
 * You can use the STATUS constants defined in logic.h to help!
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object to by reference to see current location.
 * @param   nextRow     Player's next row on the dungeon map (up/down).
 * @param   nextCol     Player's next column on dungeon map (left/right).
 * @return  Player's movement status after updating player's position.
 * @update map contents, player
 */
int doPlayerMove(char** map, int maxRow, int maxCol, Player& player, int nextRow, int nextCol) {
    
    if(player.row >= maxRow || player.col >= maxCol) {
        nextRow = player.row;
        nextCol = player.col;
        return STATUS_STAY; 
    }
    //Determines if player should move if row and column values are 0
    if (player.row == 0 && nextRow < 0) {
        nextRow = player.row;
        nextCol = player.col;
        return STATUS_STAY;     
    }
    if (player.col == 0 && nextCol < 0) {
        nextRow = player.row;
        nextCol = player.col;
        return STATUS_STAY; 
    }
    
    //Checks edge
    if (nextRow >= maxRow || nextCol >= maxCol || map[nextRow][nextCol] == TILE_PILLAR || map[nextRow][nextCol] == TILE_MONSTER || (map[nextRow][nextCol] == TILE_EXIT && player.treasure == 0)) {
        nextRow = player.row;
        nextCol = player.col;
        return STATUS_STAY;
    }
    int tile= map[nextRow][nextCol];
    // open
    if(tile == TILE_OPEN) {
       map[player.row][player.col] = TILE_OPEN; 
       player.row = nextRow;
       player.col = nextCol;
       map[nextRow][nextCol] = TILE_PLAYER;
       
       return STATUS_MOVE;
    }
    //door
    if(tile== TILE_DOOR) {
        map[player.row][player.col] = TILE_OPEN;
        player.row = nextRow;
        player.col = nextCol;
        map[nextRow][nextCol] = TILE_PLAYER;
        
        return STATUS_LEAVE;
    }
    //treasure
    if(tile == TILE_TREASURE) {
        map[player.row][player.col] = TILE_OPEN;
        player.row = nextRow;
        player.col = nextCol;
        player.treasure = player.treasure + 1;
        map[nextRow][nextCol] = TILE_PLAYER;
        
        return STATUS_TREASURE;
    }
    //Check to move if an amulet
    if(tile == TILE_AMULET) {
        map[player.row][player.col] = TILE_OPEN;
        player.row = nextRow;
        player.col = nextCol;
        map[nextRow][nextCol] = TILE_PLAYER;
        
        return STATUS_AMULET;     
    }
    // exit
    if(tile == TILE_EXIT && player.treasure >= 1) {
        map[player.row][player.col] = TILE_OPEN;
        player.row = nextRow;
        player.col = nextCol;
        map[nextRow][nextCol] = TILE_PLAYER;
        
        return STATUS_ESCAPE;
    }
    return 0;

}

/**
 * TODO: Student implement this function
 * Update monster locations:
 * We check up, down, left, right from the current player position.
 * If we see an obstacle, there is no line of sight in that direction, and the monster does not move.
 * If we see a monster before an obstacle, the monster moves one tile toward the player.
 * We should update the map as the monster moves.
 * At the end, we check if a monster has moved onto the player's tile.
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object by reference for current location.
 * @return  Boolean value indicating player status: true if monster reaches the player, false if not.
 * @update map contents
 */
bool doMonsterAttack(char** map, int maxRow, int maxCol, const Player& player) {
    
    //up
    for(int i = player.row-1; i >= 0 ; i--){
        if(map[i][player.col] == TILE_PILLAR){
            break;
        }
        else if(map[i][player.col] == TILE_MONSTER){
            map[i][player.col] = TILE_OPEN;
            map[i+1][player.col] = TILE_MONSTER;
        }
    }
    //down
    for(int i = player.row+1; i < maxRow ; i++){
        if(map[i][player.col] == TILE_PILLAR){
            break;
        }
        else if(map[i][player.col] == TILE_MONSTER){
            map[i][player.col] = TILE_OPEN;
            map[i-1][player.col] = TILE_MONSTER;
        }
    }
    //left
    for(int i = player.col-1; i >= 0 ; i--){
        if(map[player.row][i] == TILE_PILLAR){
            break;
        }
        else if(map[player.row][i] == TILE_MONSTER){
            map[player.row][i] = TILE_OPEN;
            map[player.row][i+1] = TILE_MONSTER; 
        }
    }
    //right
    for(int i = player.col+1; i < maxCol; i++){
        if(map[player.row][i] == TILE_PILLAR){
            break;
        }
        else if(map[player.row][i] == TILE_MONSTER){
            map[player.row][i] = TILE_OPEN;
            map[player.row][i-1] = TILE_MONSTER;
        }
    }
    //check eaten
    if(map[player.row][player.col] == TILE_MONSTER){
        return true;
    }
    return false;
}
