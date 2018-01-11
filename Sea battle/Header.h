#pragma once
#define FIELD_SIZE 10


struct Coordinate {
	int num;
	char ch;
};

void printBorder();

void printField(char field[FIELD_SIZE][FIELD_SIZE], int mode);

int getSumm(int arr[], int size);

void printShipsCount(int shipsCount[]);

int checkNeighbours(char map[FIELD_SIZE][FIELD_SIZE], int currI, int currJ);

int addShip(char map[FIELD_SIZE][FIELD_SIZE], struct Coordinate b, struct Coordinate e, int type);

void fillBotField(char map[FIELD_SIZE][FIELD_SIZE]);

int isKilled(struct Coordinate aim, char field[FIELD_SIZE][FIELD_SIZE]);

void destroyShip(struct Coordinate aim, char field[FIELD_SIZE][FIELD_SIZE], int shipCount[]);

int nextTurn(struct Coordinate aim, char field[FIELD_SIZE][FIELD_SIZE], int shipCount[]);

void printGameSpace(char player1[FIELD_SIZE][FIELD_SIZE], char player2[FIELD_SIZE][FIELD_SIZE], int botCount[], int playerCount[]);

int hitTarget(struct Coordinate target, char field[FIELD_SIZE][FIELD_SIZE], int shipCount[]);

int search(char field[FIELD_SIZE][FIELD_SIZE], int shipCount[], struct Coordinate* aim);

struct Coordinate botTurn(char enemyField[FIELD_SIZE][FIELD_SIZE], char field[FIELD_SIZE][FIELD_SIZE], int botShipCount[], int playerShipCount[], struct Coordinate currAim);

struct Coordinate getCoordinate(char field[FIELD_SIZE][FIELD_SIZE]);

int getType();
