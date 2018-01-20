#pragma once
#define FIELD_SIZE 10


struct Coordinate {
	int num;
	char ch;
};

enum shipType {
	TYPE1 = 1,
	TYPE2,
	TYPE3,
	TYPE4
};

enum  squareStatus{
	EMPTY,    //' '
	CHECKED,  //'.'
	SHIP_PART,//'o'
	DAMAGED,  //'x'
	DESTROYED //'*'
};
void initField(enum squareStatus field[FIELD_SIZE][FIELD_SIZE]);

void printBorder();

void printField(enum squareStatus field[FIELD_SIZE][FIELD_SIZE], int mode);

int getSumm(int arr[], int size);

void printShipsCount(int shipsCount[]);

int isOnField(int checkCoordinate1, int checkCoordinate2);

int checkNeighbours(enum squareStatus map[FIELD_SIZE][FIELD_SIZE], int currI, int currJ);

int addShip(enum squareStatus map[FIELD_SIZE][FIELD_SIZE], struct Coordinate b, struct Coordinate e, enum shipType type);

void fillBotField(enum squareStatus map[FIELD_SIZE][FIELD_SIZE]);

void coordinateToInt(struct Coordinate c, int* c1, int* c2);

int isEmpty(enum squareStatus field[FIELD_SIZE][FIELD_SIZE], int c1, int c2);

int checkCoordinate(int c1, int c2, int horizontal, int vertical, enum squareStatus field[FIELD_SIZE][FIELD_SIZE]);

int isKilled(struct Coordinate aim, enum squareStatus field[FIELD_SIZE][FIELD_SIZE]);

void markAsChecked(enum squareStatus field[FIELD_SIZE][FIELD_SIZE], int checkCoordinate1, int checkCoordinate2, int isHorizontal, int isVertical, int horizontal, int vertical, int* shipLen);

int destroyLine(int c1, int c2, int vertical, int horizontal, enum squareStatus field[FIELD_SIZE][FIELD_SIZE], int isHorizontal, int* shipLen);

void destroyShip(struct Coordinate aim, enum squareStatus field[FIELD_SIZE][FIELD_SIZE], int shipCount[]);

int nextTurn(struct Coordinate aim, enum squareStatus field[FIELD_SIZE][FIELD_SIZE], int shipCount[]);

void printGameSpace(enum squareStatus player1[FIELD_SIZE][FIELD_SIZE], enum squareStatus player2[FIELD_SIZE][FIELD_SIZE], int botCount[], int playerCount[]);

int hitLine(struct Coordinate aim, enum squareStatus field[FIELD_SIZE][FIELD_SIZE], int shipCount[], int vertical, int horizontal);

int hitTarget(struct Coordinate target, enum squareStatus field[FIELD_SIZE][FIELD_SIZE], int shipCount[]);

int search(enum squareStatus field[FIELD_SIZE][FIELD_SIZE], int shipCount[], struct Coordinate* aim);

struct Coordinate botTurn(enum squareStatus enemyField[FIELD_SIZE][FIELD_SIZE], enum squareStatus field[FIELD_SIZE][FIELD_SIZE], int botShipCount[], int playerShipCount[], struct Coordinate currAim);

struct Coordinate getCoordinate(enum squareStatus field[FIELD_SIZE][FIELD_SIZE]);

int getType(enum squareStatus field[FIELD_SIZE][FIELD_SIZE], int shipsCount[]);

void get2Coordinates(struct Coordinate* beginning, struct Coordinate* ending, enum squareStatus field[FIELD_SIZE][FIELD_SIZE], int currType);

void shipsArrangement(enum squareStatus field[FIELD_SIZE][FIELD_SIZE]);

void gameLoop(int playerShipsCount[], int botShipsCount[], enum squareStatus field[FIELD_SIZE][FIELD_SIZE], enum squareStatus botField[FIELD_SIZE][FIELD_SIZE]);

void getResult(int playerShipsCount[]);

void exitGame();