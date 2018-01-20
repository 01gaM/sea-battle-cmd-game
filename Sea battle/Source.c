#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include "Header.h"

int main() {
	setlocale(LC_ALL, "RUS");
	printf_s("Условные обозначения:\n");
	printf_s("o - часть корабля\n");
	printf_s(". - пустая клетка\n");
	printf_s("x - повреждённая часть корабля\n");
	printf_s("* - потопленный корабль\n");
	enum squareStatus field[FIELD_SIZE][FIELD_SIZE];//поле игрока
	initField(field);
	printField(field, 1);

	shipsArrangement(field);//расставляем корабли

	enum squareStatus botField[FIELD_SIZE][FIELD_SIZE];//поле бота
	initField(botField);
	fillBotField(botField);//заполняем поле бота
	int playerShipsCount[] = { 4,3,2,1 };//оставшиеся корабли игрока
	int botShipsCount[] = { 4,3,2,1 };//оставшиеся корабли противника
	printGameSpace(field, botField, botShipsCount, playerShipsCount);

	gameLoop(playerShipsCount, botShipsCount, field, botField);//игровой цикл

	getResult(playerShipsCount);//определяем исход игры

	exitGame();//запрашиваем символ для выхода

	return 0;
}

