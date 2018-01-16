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
	enum squareStatus field[FIELD_SIZE][FIELD_SIZE];
	initField(field);
	printField(field, 1);
	int shipsCount[] = { 4,3,2,1 };//o - 4, oo - 3, ooo - 2, oooo - 1

	while (getSumm(shipsCount, 4) != 0) {////////////////цикл расстановки кораблей
		printShipsCount(shipsCount);
		enum shipType currType;//тип текущего корабля
		struct Coordinate beginning;//координаты начала корабля
		struct Coordinate ending;//координаты конца корабля
		printf_s("Выберите тип корабля: ");
		currType = getType(field, shipsCount);
		
		if (currType == TYPE1) {//запрашиваем одну координату для TYPE1
			printf_s("Укажите его координаты: ");
			beginning = getCoordinate(field);
		}
		else {//запрашиваем 2 координаты для остальных типов
			get2Coordinates(&beginning, &ending, field, currType);			
		}

		////////проверка на соприкасание кораблей
		if (addShip(field, beginning, ending, currType) == 0) {
			printf_s("Корабль пересекается с соседними. Выберите другой корабль.\n");
		}
		else {
			shipsCount[currType - 1]--;
		}
		printField(field, 1);
	}

	enum squareStatus botField[FIELD_SIZE][FIELD_SIZE];
	initField(botField);
	fillBotField(botField);//заполняем поле бота
	struct Coordinate aim;
	struct Coordinate botAim;
	botAim.ch = '0';//обнуляем текущую цель бота 
	botAim.num = 0;
	int playerShipsCount[] = { 4,3,2,1 };//оставшиеся корабли игрока
	int botShipsCount[] = { 4,3,2,1 };//оставшиеся корабли противника
	printGameSpace(field, botField, botShipsCount, playerShipsCount);
	//игровой цикл
	while ((getSumm(playerShipsCount, 4) != 0) && (getSumm(botShipsCount, 4) != 0)) {
		//ход игрока
		printf_s("\nВведите координаты цели: ");
		aim = getCoordinate(botField);
		int t = nextTurn(aim, botField, botShipsCount);//результат хода игрока
		while ((t == 1) || (t == 2)) {//если игрок попал по цели, то следующий ход переходит ему
			printGameSpace(field, botField, botShipsCount, playerShipsCount);
			printf_s("\nВведите координаты цели: ");
			aim = getCoordinate(botField);
			t = nextTurn(aim, botField, botShipsCount);
		}

		printGameSpace(field, botField, botShipsCount, playerShipsCount);
		if (t == 3) {//если все корабли противника убиты
			break;
		}

		//ход бота
		printf_s("\nХод противника\n");
		botAim = botTurn(field, botField, botShipsCount, playerShipsCount, botAim);

	}
	if (getSumm(playerShipsCount, 4) == 0) {
		printf_s("\nВы проиграли!");
	}
	else {
		printf_s("\nВы выйграли!");
	}

	int x = getchar();
	while (x != '\n') {//сбрасываем данные, введённые ранее
		x = getchar();
	}
	printf_s("\nВведите любой символ для выхода: ");//считываем новые данные
	getchar();
	return 0;
}

