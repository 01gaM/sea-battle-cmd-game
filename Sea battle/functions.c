#include "Header.h"
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>

void initField(enum squareStatus field [FIELD_SIZE][FIELD_SIZE]) {
	for (int i = 0; i < FIELD_SIZE; i++) {
		for (int j = 0; j < FIELD_SIZE; j++) {
			field[i][j] = EMPTY;//"обнуляем" поле
		}
	}
}

void printBorder() {
	int stripeLen = 41;
	printf_s("\n  ");
	for (int i = 0; i < stripeLen; i++) {
		printf_s("-");
	}
}

void printField(enum squareStatus field[FIELD_SIZE][FIELD_SIZE], int mode) {
	char ch = 'A';
	printf_s("  ");
	for (int i = 1; i <= FIELD_SIZE; i++) {
		printf_s("  %d ", i);
	}
	printBorder();
	printf_s("\n");
	for (int i = 0; i < FIELD_SIZE; i++) {
		printf_s("%c |", ch);
		for (int j = 0; j < FIELD_SIZE; j++) {
			if ((mode == 2) && (field[i][j] == SHIP_PART)) {
				printf_s("   |");
			}
			else {
				switch (field[i][j]) {
				case EMPTY:
					printf_s("   |");
					break;
				case CHECKED:
					printf_s(" . |");
					break;
				case SHIP_PART:
					printf_s(" o |");
					break;
				case DAMAGED:
					printf_s(" x |");
					break;
				case DESTROYED:
					printf_s(" * |");
					break;
				}
			}
		}
		printBorder();
		printf_s("\n");
		ch++;
	}
}

int getSumm(int arr[], int size) {
	int s = 0;
	for (int i = 0; i < size; i++) {
		s += arr[i];
	}
	return s;
}

void printShipsCount(int shipsCount[]) {
	printf_s("Оставшиеся корабли:\n");
	for (int i = 0; i < 4; i++) {
		printf_s("Тип %d: %d - ", i + 1, shipsCount[i]);
		for (int j = 0; j <= i; j++) {
			printf_s("o");
		}
		printf_s("\n");
	}
}

int isOnField(int checkCoordinate1, int checkCoordinate2) {
	return ((checkCoordinate1 >= 0) && (checkCoordinate1 <= 9) && (checkCoordinate2 >= 0) && (checkCoordinate2 <= 9)) ? 1 : 0;//1 - координата на поле, 0 - координата не на поле
}

int checkNeighbours(enum squareStatus map[FIELD_SIZE][FIELD_SIZE], int currI, int currJ) {	
	for (int i = currI-1; i <= currI+1; i++) {
		for (int j = currJ-1; j <= currJ+1; j++) {
			if ((isOnField(i, j) == 1) && (map[i][j] != EMPTY)) {
				return 0;
			}
		}
	}
	return 1;//точка не пересекается с соседними
}

int addShip(enum squareStatus map[FIELD_SIZE][FIELD_SIZE], struct Coordinate b, struct Coordinate e, enum shipType type) {//добавляет корабль
	if (type == TYPE1) {
		int num2 = b.ch - 'A';
		if (checkNeighbours(map, num2, b.num - 1) == 0) {
			return 0;//нельзя поставить корабль в этом месте
		}
		map[num2][b.num - 1] = SHIP_PART;//добавляем корабль
	}
	else {
		int num1 = b.ch - 'A';
		int num2 = e.ch - 'A';

		for (int i = num1; i <= num2; i++) {
			for (int j = b.num; j <= e.num; j++) {
				if (checkNeighbours(map, i, j - 1) == 0) {
					return 0;//нельзя поставить корабль в этом месте
				}
			}
		}
		for (int i = num1; i <= num2; i++) {
			for (int j = b.num; j <= e.num; j++) {
				map[i][j - 1] = SHIP_PART;//добавляем корабль
			}
		}
	}
	return 1;//корабль был добавлен на поле
}

int getShipLen(struct Coordinate b, struct Coordinate e) {//получаем длину корабля
	int s1 = e.ch - b.ch;
	int s2 = e.num - b.num;
	return (s1 > s2) ? s1 : s2;
}

void fillBotField(enum squareStatus map[FIELD_SIZE][FIELD_SIZE]) {
	struct Coordinate begin;
	struct Coordinate end;
	srand(time(NULL));//инициализация rand
	int botShipsCount[] = { 4,3,2,1 };//o - 4, oo - 3, ooo - 2, oooo - 1
	while (getSumm(botShipsCount, 4) != 0) {
		for (int currType = 3; currType >= 0; currType--) {
			while (botShipsCount[currType] > 0) {//если корабли этого типа не закончились
				begin.ch = rand() % 10 + 'A';//значения от 'A' до 'J'
				begin.num = rand() % 10 + 1;//значения от 1 до 10
				end.ch = '-';//"обнуляем координаты конца"
				end.num = -1;
				int currI = begin.ch - 'A';
				if (currI + currType < 9) {
					end.ch = begin.ch + currType;//вертикальное расположение корабля
					end.num = begin.num;
				}
				else {
					if (begin.num + currType < 9) {
						end.num = begin.num + currType;
						end.ch = begin.ch;
					}
				}
				if ((end.ch != '-') && (end.num != -1)) {//если корабль входит в поле
														 //пытаемся добавить корабль на поле
					if (addShip(map, begin, end, currType + 1) == 1) {//если удачно
						botShipsCount[currType]--;//убираем корабль из списка доступных
					}
				}
			}
		}
	}

}

void coordinateToInt(struct Coordinate c, int* c1, int* c2) {//переводим координаты в значения от 0 до 9
	*c1 = c.ch - 'A';
	*c2 = c.num - 1;
}


int isEmpty(enum squareStatus field[FIELD_SIZE][FIELD_SIZE], int c1, int c2) {
	return (field[c1][c2] == EMPTY) || (field[c1][c2] == CHECKED) ? 1 : 0;//1 - клетка пустая, 0 - клетка не пустая
}

int checkCoordinate(int c1, int c2,  int vertical, int horizontal, enum squareStatus field[FIELD_SIZE][FIELD_SIZE]) {
	int checkCoordinate1 = c1 + vertical;
	int checkCoordinate2 = c2 + horizontal;
	while ((isOnField(checkCoordinate1, checkCoordinate2) == 1) && (isEmpty(field, checkCoordinate1, checkCoordinate2) == 0)) {//пока не дошли до конца поля или до конца текушего корабля
		if (field[checkCoordinate1][checkCoordinate2] == SHIP_PART) {
			return 0;//найдена неповреждённая часть корабля
		}
		checkCoordinate1 += vertical;
		checkCoordinate2 += horizontal;
	}
	return -1;//не найдена неповреждённая часть корабля
}

int isKilled(struct Coordinate aim, enum squareStatus field[FIELD_SIZE][FIELD_SIZE]) {
	int c1;
	int c2;
	coordinateToInt(aim, &c1, &c2);
	//проверяем верх, низ, лево и право на наличие неповреждённых частей корабля
	//1 - корабль "убит", 0 - корабль не полностью разрушен
	return ((checkCoordinate(c1, c2, -1, 0, field) == 0) || (checkCoordinate(c1, c2, 1, 0, field) == 0) || (checkCoordinate(c1, c2, 0, -1, field) == 0) || (checkCoordinate(c1, c2, 0, 1, field) == 0)) ? 0 : 1;
}

void markAsChecked(enum squareStatus field[FIELD_SIZE][FIELD_SIZE], int checkCoordinate1, int checkCoordinate2, int isHorizontal, int isVertical, int horizontal, int vertical, int* shipLen) {
	while ((isOnField(checkCoordinate1, checkCoordinate2) == 1) && (isEmpty(field, checkCoordinate1, checkCoordinate2) == 0)) {//пока не дошли до конца поля или до конца текушего корабля
		field[checkCoordinate1][checkCoordinate2] = DESTROYED;

		if ((isOnField(checkCoordinate1 - isHorizontal, checkCoordinate2 - isVertical) == 1) && (isEmpty(field, checkCoordinate1 - isHorizontal, checkCoordinate2 - isVertical) == 1)) {
			field[checkCoordinate1 - isHorizontal][checkCoordinate2 - isVertical] = CHECKED;//левая клетка для вертикального и верхняя для горизонтального
		}
		if ((isOnField(checkCoordinate1 + isHorizontal, checkCoordinate2 + isVertical) == 1) && (isEmpty(field, checkCoordinate1 + isHorizontal, checkCoordinate2 + isVertical) == 1)) {
			field[checkCoordinate1 + isHorizontal][checkCoordinate2 + isVertical] = CHECKED;//правая клетка для вертикального и нижняя для горизонтального
		}
		//если дошли до конца корабля, то отмечаем клетки по диагонали
		if ((isOnField(checkCoordinate1 + vertical, checkCoordinate2 + horizontal) == 1) && (isEmpty(field, checkCoordinate1 + vertical, checkCoordinate2 + horizontal) == 1)) {
			field[checkCoordinate1 + vertical][checkCoordinate2 + horizontal] = CHECKED;//для вертикального - клетка сверху или снизу, для горизонтального - клетка слева или справа (в зависимости от направления, заданного в параметрах функции)
			if (isOnField(checkCoordinate1 + vertical - isHorizontal, checkCoordinate2 + horizontal - isVertical) == 1) {
				field[checkCoordinate1 + vertical - isHorizontal][checkCoordinate2 + horizontal - isVertical] = CHECKED;//левая клетка для вертикального и верхняя для горизонтального
			}
			if (isOnField(checkCoordinate1 + vertical + isHorizontal, checkCoordinate2 + horizontal + isVertical) == 1) {
				field[checkCoordinate1 + vertical + isHorizontal][checkCoordinate2 + horizontal + isVertical] = CHECKED;//правая клетка для вертикального и нижняя для горизонтального
			}
		}
		*shipLen = *shipLen + 1;
		checkCoordinate1 += vertical;
		checkCoordinate2 += horizontal;
	}
}

int destroyLine(int c1, int c2, int vertical, int horizontal, enum squareStatus field[FIELD_SIZE][FIELD_SIZE], int isHorizontal, int* shipLen) {
	int checkCoordinate1 = c1;
	int checkCoordinate2 = c2;
	int isVertical = 0;
	if (isHorizontal == 0) {//указываем направление корабля
		isVertical = 1;
	}
	markAsChecked(field, checkCoordinate1, checkCoordinate2, isHorizontal, isVertical, horizontal, vertical, shipLen);//отмечаем соседние клетки, как проверенные пустые
}

void destroyShip(struct Coordinate aim, enum squareStatus field[FIELD_SIZE][FIELD_SIZE], int shipCount[]) {
	int c1;
	int c2;
	coordinateToInt(aim, &c1, &c2);
	int checkCoordinate1;
	int checkCoordinate2;
	int shipLen = -4;//длина корабля - 4 (т.к. проверяем исходную координату в 4-х направлениях)
	field[c1][c2] = DAMAGED;
	//разрушаем верх
	destroyLine(c1, c2, -1, 0, field, 0, &shipLen);
	//разрушаем низ
	destroyLine(c1, c2, 1, 0, field, 0, &shipLen);
	//разрушаем лево
	destroyLine(c1, c2, 0, -1, field, 1, &shipLen);
	//разрушаем право
	destroyLine(c1, c2, 0, 1, field, 1, &shipLen);
	shipCount[shipLen]--;//убираем корабль из списка оставшихся
}

int nextTurn(struct Coordinate aim, enum squareStatus field[FIELD_SIZE][FIELD_SIZE], int shipCount[]) {
	///если в соседних клетках не все части корабля повреждены, то заменяем клетку на "x"
	//если в соседних клетках все части корабля повреждены(или он типа 1), то заменяем все клетки корабля на "*"
	int c1;
	int c2;
	coordinateToInt( aim, &c1, &c2);

	if (field[c1][c2] == EMPTY) {//попадание мимо
		field[c1][c2] = CHECKED;
		return 0;
	}
	if (field[c1][c2] != SHIP_PART) {//повторный выбор клетки для атаки
		return -1;
	}

	if (isKilled(aim, field) == 0) {
		field[c1][c2] = DAMAGED;
		return 1;//корабль 'ранен'
	}
	else {
		destroyShip(aim, field, shipCount);///заменить все клетки корабля на *
	}
	return getSumm(shipCount, 4) == 0 ? 3 : 2;//3 - игра закончена, 2 - корабль убит
}

void printGameSpace(enum squareStatus player1[FIELD_SIZE][FIELD_SIZE], enum squareStatus player2[FIELD_SIZE][FIELD_SIZE], int botCount[], int playerCount[]) {
	printf_s("Ваше поле:\n");
	printField(player1, 1);// 1 - игрок 1
	printShipsCount(playerCount);
	printf_s("\nПоле противника:\n");
	printField(player2, 2);//2 - игрок 2
	printShipsCount(botCount);
}


int hitLine(struct Coordinate aim, enum squareStatus field[FIELD_SIZE][FIELD_SIZE], int shipCount[], int vertical, int horizontal) {
	int x;//результат хода
	int c1;
	int c2;
	coordinateToInt(aim, &c1, &c2);
	if ((isOnField(c1, c2) == 1) && ((field[c1][c2] == SHIP_PART) || (field[c1][c2] == EMPTY))) {
		x = nextTurn(aim, field, shipCount);
		printf_s("\n%d%c\n", aim.num, aim.ch);
		while ((x == 1) && (isOnField(c1 + vertical, c2 + horizontal) == 1)) {
			aim.ch += vertical;
			c1 += vertical;
			aim.num += horizontal;
			c2 += horizontal;
			x = nextTurn(aim, field, shipCount);
			printf_s("\n%d%c\n", aim.num, aim.ch);
		}
		return x == 2 ? 1 : -1;//1 - success, 2 - корабль не полностью разрушен
	}
	return 0;
}

int hitTarget(struct Coordinate target, enum squareStatus field[FIELD_SIZE][FIELD_SIZE], int shipCount[]) {
	struct Coordinate aim;
	aim.num = target.num;
	aim.ch = target.ch - 1;
	int x;//результат хода
	//верх
	x = hitLine(aim, field, shipCount, -1, 0);
	if (x != 0) {
		return x;
	}
	aim.ch = target.ch + 1;
	//низ
	x = hitLine(aim, field, shipCount, 1, 0);
	if (x != 0) {
		return x;
	}
	aim.num = target.num - 1;
	aim.ch = target.ch;
	//лево
	x = hitLine(aim, field, shipCount, 0, -1);
	if (x != 0) {
		return x;
	}
	aim.num = target.num + 1;
	//право
	x = hitLine(aim, field, shipCount, 0, 1);
	if (x != 0) {
		return x;
	}
}

int checkDiagonal(int currX, int currY, struct Coordinate* aim, enum squareStatus field[FIELD_SIZE][FIELD_SIZE], int shipCount[]) {
	int x;
	while (currY < 10) {//первые диагонали(поиск корабля длины 4)
		aim->ch = currX + 'A';
		aim->num = currY + 1;
		x = nextTurn(*aim, field, shipCount);
		if (x != -1) {
			printf_s("\n%d%c\n", aim->num, aim->ch);
		}
		if ((x != -1) && (x != 2)) {//продолжаем поиск, только если убили корабль или посетили уже проверенную клетку
			return x;
		}
		if (currX + 4 < 10) {
			currX += 4;
		}
		else {
			currX--;
			currX -= 4 * (int)(currX / 4);
			currY++;
		}
	}
	return -2;//выход из цикла
}

int search(enum squareStatus field[FIELD_SIZE][FIELD_SIZE], int shipCount[], struct Coordinate* aim) {
	int x;//результат хода
	x = checkDiagonal(3, 0, aim, field, shipCount);//первые диагонали(поиск корабля длины 4)
	if (x != -2) {
		return x;
	}
	x = checkDiagonal(1, 0, aim, field, shipCount); {//вторые диагонали (поиск кораблей длины 3 и 2)
		if (x != -2) {
			return x;
		}
		for (int i = 0; i < 10; i++) {//поиск кораблей длины 1
			for (int j = 0; j < 10; j++) {
				if ((field[i][j] == SHIP_PART) || (field[i][j] == EMPTY)) {
					aim->ch = i + 'A';
					aim->num = j + 1;
					x = nextTurn(*aim, field, shipCount);
					printf_s("\n%d%c\n", aim->num, aim->ch);
					if ((x != -1) && (x != 2)) {//продолжаем поиск, если убили корабль или посетили уже проверенную клетку
						return x;
					}

				}
			}
		}
	}
}

struct Coordinate botTurn(enum squareStatus enemyField[FIELD_SIZE][FIELD_SIZE], enum squareStatus field[FIELD_SIZE][FIELD_SIZE], int botShipCount[], int playerShipCount[], struct Coordinate currAim) {
	int y = 0;//результат добивания корабля
	int x;
	x = ((currAim.ch == '0') && (currAim.num == 0)) ? search(enemyField, playerShipCount, &currAim) : 1;//если нет текущей цели, то находим её

	if (x == 1) {//если есть попадание(есть цель)
		y = hitTarget(currAim, enemyField, playerShipCount);//пытаемся добить этот корабль
	}
	else {
		currAim.ch = '0';//обнуляем текущую цель, т.к. попадание мимо
		currAim.num = 0;
	}

	if (y == 1) {//если цель уничтожена
		currAim.ch = '0';//обнуляем текущую цель
		currAim.num = 0;
		return botTurn(enemyField, field, botShipCount, playerShipCount, currAim);//повтор хода т.к. попадание 
	}
	printGameSpace(enemyField, field, botShipCount, playerShipCount);
	//если не уничтожена, то сохраняем текущую цель без изменений
	return currAim;
}

struct Coordinate getCoordinate(enum squareStatus field[FIELD_SIZE][FIELD_SIZE]) {/////////////!!!!
	struct Coordinate c;
	char ch = '0';//символ после координат
	int x = scanf_s("%d%c", &c.num, &c.ch);
	if (c.ch != '\n') {//если было введено не только число
		ch = getchar();//получаем символ после введённых координат
	}
	int c1;
	int c2;
	coordinateToInt(c, &c1, &c2);
	while ((isOnField(c1, c2) == 0) || (ch != '\n') || (x != 2)) {
		printf_s("\nКоординаты корабля должны состоять из целого числа от 1 до 10 и буквы от 'A' до 'J'.\nВведите другие координаты: ");
		//сбрасываем введённые ранее данные
		if ((ch != '\n') && (c.ch != '\n') || (x != 2)) {//если было введено что-то кроме числа и буквы
			x = getchar();
			while (x != '\n') {
				x = getchar();
			}
		}
		//считываем новые данные
		x = scanf_s("%d%c", &c.num, &c.ch);
		if (c.ch != '\n') {//если было введено не только число
			ch = getchar();//получаем символ после введённых координат
		}
		else {
			ch = '0';//"обнуляем" символ
		}
		coordinateToInt(c, &c1, &c2);
	}
	if ((field[c1][c2] != SHIP_PART) && (field[c1][c2] != EMPTY)) {
		printf_s("\nДанная точка уже проверена. Введите новые координаты: ");
		return getCoordinate(field);
	}
	return c;
}

int getType(enum squareStatus field[FIELD_SIZE][FIELD_SIZE], int shipsCount[]) {
	enum Type currType;
	char ch;
	int x = scanf_s("%d%c", &currType, &ch);
	while ((x != 2) || (currType < 1) || (currType > 4) || (ch != '\n')) {//если число не является допустимым для типа или было введено не только число
		printf_s("Недопустимое значение типа. Введите число от 1 до 4: ");
		if ((ch != '\n') || (x != 2)) {//если ранее было введено не только одно число (неверный формат введённых данных)
			x = getchar();
			while (x != '\n') {//сбрасываем всё введённое ранее
				x = getchar();
			}
		}
		x = scanf_s("%d%c", &currType, &ch);//считываем новые данные
	}
	if (shipsCount[currType - 1] == 0) {
		printf_s("Корабли данного типа закончились. Выберите другой тип: ");
		return getType(field, shipsCount);
	}
	return currType;
}

void get2Coordinates(struct Coordinate* beginning, struct Coordinate* ending, enum squareStatus field[FIELD_SIZE][FIELD_SIZE], int currType) {//получение координаты начала и конца
	printf_s("Укажите координаты начала корабля: ");
	*beginning = getCoordinate(field);

	printf_s("Укажите координаты конца корабля: ");
	*ending = getCoordinate(field);

	//////координаты должны быть на одной линии
	if ((beginning -> ch != ending -> ch) && (beginning -> num != ending -> num)) {
		printf_s("Координаты начала и конца корабля должны быть на одной линии.\n");
		get2Coordinates(beginning, ending, field, currType);
	}
	/////////beginning<ending
	while ((beginning -> ch > ending -> ch) || (beginning -> num > ending -> num)) {
		printf_s("Координаты начала должны быть меньше координат конца корабля.\n");
		get2Coordinates(beginning, ending, field, currType);
	}
	//////////проверка длины корабля 
	while (getShipLen(*beginning, *ending) + 1 != currType) {
		printf_s("Длина должна соответствовать типу %d, укажите другие координаты.\n", currType);
		get2Coordinates(beginning, ending, field, currType);
	}
}

void shipsArrangement(enum squareStatus field[FIELD_SIZE][FIELD_SIZE]) {//расстановка кораблей на поле игрока
	int shipsCount[] = { 4, 3, 2, 1 };
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
}

void gameLoop(int playerShipsCount[], int botShipsCount[], enum squareStatus field[FIELD_SIZE][FIELD_SIZE], enum squareStatus botField[FIELD_SIZE][FIELD_SIZE]) {
	struct Coordinate aim;
	struct Coordinate botAim;
	botAim.ch = '0';//обнуляем текущую цель бота 
	botAim.num = 0;
	while ((getSumm(playerShipsCount, 4) > 0) && (getSumm(botShipsCount, 4) > 0)) {
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
}

void getResult(int playerShipsCount[]) {
	if (getSumm(playerShipsCount, 4) == 0) {
		printf_s("\nВы проиграли!");
	}
	else {
		printf_s("\nВы выйграли!");
	}
}

void exitGame() {
	printf_s("\nВведите любой символ для выхода: ");
	getchar();
}