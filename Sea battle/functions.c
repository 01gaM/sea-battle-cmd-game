#include "Header.h"
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>

void printBorder() {
	int stripeLen = 41;
	printf_s("\n  ");
	for (int i = 0; i < stripeLen; i++) {
		printf_s("-");
	}
}

void printField(char field[FIELD_SIZE][FIELD_SIZE], int mode) {

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
			if ((mode == 2) && (field[i][j] == 'o')) {
				printf_s("   |");
			}
			else {
				printf_s(" %c |", field[i][j]);
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



int checkNeighbours(char map[FIELD_SIZE][FIELD_SIZE], int currI, int currJ) {
	if (map[currI][currJ] != ' ') {
		return 0;
	}
	////////check top
	if ((map[currI - 1][currJ] != ' ') && (currI > 0)) {
		return 0;
	}
	////////check bot
	if ((map[currI + 1][currJ] != ' ') && (currI < 9)) {
		return 0;
	}
	////////check left
	if ((map[currI][currJ - 1] != ' ') && (currJ > 0)) {
		return 0;
	}
	////////check right
	if ((map[currI][currJ + 1] != ' ') && (currJ < 9)) {
		return 0;
	}
	///////////////диагонали

	////левый верхний угол
	if ((map[currI - 1][currJ - 1] != ' ') && (currI > 0) && (currJ > 0)) {
		return 0;
	}
	///левый нижний угол
	if ((map[currI + 1][currJ - 1] != ' ') && (currI < 9) && (currJ > 0)) {
		return 0;
	}
	////правый верхний угол
	if ((map[currI - 1][currJ + 1] != ' ') && (currI > 0) && (currJ < 9)) {
		return 0;
	}
	///правый нижний угол
	if ((map[currI + 1][currJ + 1] != ' ') && (currI < 9) && (currJ < 9)) {
		return 0;
	}

	return 1;//точка не пересекается с соседними

}


int addShip(char map[FIELD_SIZE][FIELD_SIZE], struct Coordinate b, struct Coordinate e, int type) {//добавляет корабль
	if (type == 1) {
		int num2 = b.ch - 'A';
		if (checkNeighbours(map, num2, b.num - 1) == 0) {
			return 0;
		}
		map[num2][b.num - 1] = 'o';
	}
	else {
		int num1 = b.ch - 'A';
		int num2 = e.ch - 'A';


		for (int i = num1; i <= num2; i++) {
			for (int j = b.num; j <= e.num; j++) {
				if (checkNeighbours(map, i, j - 1) == 0) {
					return 0;
				}
			}
		}

		for (int i = num1; i <= num2; i++) {
			for (int j = b.num; j <= e.num; j++) {
				map[i][j - 1] = 'o';
			}
		}
	}
	return 1;//success
}

int getShipLen(struct Coordinate b, struct Coordinate e) {//получаем длину корабля
	int s1 = e.ch - b.ch;
	int s2 = e.num - b.num;
	if (s1 > s2) {
		return s1;
	}
	return s2;

}



void fillBotField(char map[FIELD_SIZE][FIELD_SIZE]) {
	struct Coordinate begin;
	struct Coordinate end;
	srand(time(NULL));//инициализация rand
	int botShipsCount[] = { 4,3,2,1 };//o - 4, oo - 3, ooo - 2, oooo - 1
	while (getSumm(botShipsCount, 4) != 0) {
		for (int currType = 3; currType >= 0; currType--) {
			while (botShipsCount[currType] > 0) {//если корабли этого типа не закончились
				begin.ch = rand() % 10 + 'A';//'A'-'J'
				begin.num = rand() % 10 + 1;//1-10
				end.ch = '-';
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

int isKilled(struct Coordinate aim, char field[FIELD_SIZE][FIELD_SIZE]) {
	int c1 = aim.ch - 'A';
	int c2 = aim.num - 1;
	int checkCoordinate1;
	int checkCoordinate2;

	//проверяем верх
	checkCoordinate1 = c1 - 1;

	while ((checkCoordinate1 >= 0) && (field[checkCoordinate1][c2] != ' ') && (field[checkCoordinate1][c2] != '.')) {//пока не дошли до конца поля или до конца текушего корабля
		if (field[checkCoordinate1][c2] == 'o') {
			return 0;//найдена неповреждённая часть корабля
		}
		checkCoordinate1--;
	}


	//проверяем низ
	checkCoordinate1 = c1 + 1;

	while ((checkCoordinate1 <= 9) && (field[checkCoordinate1][c2] != ' ') && (field[checkCoordinate1][c2] != '.')) {//пока не дошли до конца поля или до конца текушего корабля
		if (field[checkCoordinate1][c2] == 'o') {
			return 0;//найдена неповреждённая часть корабля
		}
		checkCoordinate1++;
	}


	//проверяем лево
	checkCoordinate2 = c2 - 1;

	while ((checkCoordinate2 >= 0) && (field[c1][checkCoordinate2] != ' ') && (field[c1][checkCoordinate2] != '.')) {//пока не дошли до конца поля или до конца текушего корабля
		if (field[c1][checkCoordinate2] == 'o') {
			return 0;//найдена неповреждённая часть корабля
		}
		checkCoordinate2--;
	}

	//проверяем право
	checkCoordinate2 = c2 + 1;

	while ((checkCoordinate2 <= 9) && (field[c1][checkCoordinate2] != ' ') && (field[c1][checkCoordinate2] != '.')) {//пока не дошли до конца поля или до конца текушего корабля
		if (field[c1][checkCoordinate2] == 'o') {
			return 0;//найдена неповреждённая часть корабля
		}
		checkCoordinate2++;
	}
	return 1;//корабль убит
}

void destroyShip(struct Coordinate aim, char field[FIELD_SIZE][FIELD_SIZE], int shipCount[]) {
	int c1 = aim.ch - 'A';
	int c2 = aim.num - 1;
	int checkCoordinate1;
	int checkCoordinate2;
	int isHorizontal = 1;
	int shipLen = 0;//длина корабля - 1
	field[c1][c2] = '*';

	//проверяем верх
	checkCoordinate1 = c1 - 1;

	while ((checkCoordinate1 >= 0) && (field[checkCoordinate1][c2] != ' ') && (field[checkCoordinate1][c2] != '.')) {//пока не дошли до конца поля или до конца текушего корабля
		if (field[checkCoordinate1][c2] == 'x') {
			isHorizontal = 0;
			field[checkCoordinate1][c2] = '*';
			if (c2 - 1 >= 0) {
				field[checkCoordinate1][c2 - 1] = '.';
			}
			if (c2 + 1 <= 9) {
				field[checkCoordinate1][c2 + 1] = '.';
			}
			if ((checkCoordinate1 - 1 >= 0) && ((field[checkCoordinate1 - 1][c2] == ' ') || (field[checkCoordinate1 - 1][c2] == '.'))) {
				field[checkCoordinate1 - 1][c2] = '.';
				if (c2 - 1 >= 0) {
					field[checkCoordinate1 - 1][c2 - 1] = '.';
				}
				if (c2 + 1 <= 9) {
					field[checkCoordinate1 - 1][c2 + 1] = '.';
				}
			}
			shipLen++;
		}
		checkCoordinate1--;
	}



	//проверяем низ
	checkCoordinate1 = c1 + 1;

	while ((checkCoordinate1 <= 9) && (field[checkCoordinate1][c2] != ' ') && (field[checkCoordinate1][c2] != '.')) {//пока не дошли до конца поля или до конца текушего корабля
		if (field[checkCoordinate1][c2] == 'x') {
			isHorizontal = 0;
			field[checkCoordinate1][c2] = '*';
			if (c2 - 1 >= 0) {
				field[checkCoordinate1][c2 - 1] = '.';
			}
			if (c2 + 1 <= 9) {
				field[checkCoordinate1][c2 + 1] = '.';
			}
			if ((checkCoordinate1 + 1 <= 9) && ((field[checkCoordinate1 + 1][c2] == ' ') || (field[checkCoordinate1 + 1][c2] == '.'))) {
				field[checkCoordinate1 + 1][c2] = '.';
				if (c2 - 1 >= 0) {
					field[checkCoordinate1 + 1][c2 - 1] = '.';
				}
				if (c2 + 1 <= 9) {
					field[checkCoordinate1 + 1][c2 + 1] = '.';
				}
			}
			shipLen++;
		}
		checkCoordinate1++;
	}

	if (isHorizontal == 1) {
		//проверяем лево
		checkCoordinate2 = c2 - 1;

		while ((checkCoordinate2 >= 0) && (field[c1][checkCoordinate2] != ' ') && (field[c1][checkCoordinate2] != '.')) {//пока не дошли до конца поля или до конца текушего корабля
			if (field[c1][checkCoordinate2] == 'x') {
				field[c1][checkCoordinate2] = '*';
				if (c1 - 1 >= 0) {
					field[c1 - 1][checkCoordinate2] = '.';
				}
				if (c1 + 1 <= 9) {
					field[c1 + 1][checkCoordinate2] = '.';
				}
				if ((checkCoordinate2 - 1 >= 0) && ((field[c1][checkCoordinate2 - 1] == ' ') || (field[c1][checkCoordinate2 - 1] == '.'))) {
					field[c1][checkCoordinate2 - 1] = '.';
					if (c1 - 1 >= 0) {
						field[c1 - 1][checkCoordinate2 - 1] = '.';
					}
					if (c1 + 1 <= 9) {
						field[c1 + 1][checkCoordinate2 - 1] = '.';
					}
				}
				shipLen++;
			}
			checkCoordinate2--;
		}

		//проверяем право
		checkCoordinate2 = c2 + 1;

		while ((checkCoordinate2 <= 9) && (field[c1][checkCoordinate2] != ' ') && (field[c1][checkCoordinate2] != '.')) {//пока не дошли до конца поля или до конца текушего корабля
			if (field[c1][checkCoordinate2] == 'x') {
				field[c1][checkCoordinate2] = '*';
				if (c1 - 1 >= 0) {
					field[c1 - 1][checkCoordinate2] = '.';
				}
				if (c1 + 1 <= 9) {
					field[c1 + 1][checkCoordinate2] = '.';
				}
				if ((checkCoordinate2 + 1 <= 9) && ((field[c1][checkCoordinate2 + 1] == ' ') || (field[c1][checkCoordinate2 + 1] == '.'))) {
					field[c1][checkCoordinate2 + 1] = '.';
					if (c1 - 1 >= 0) {
						field[c1 - 1][checkCoordinate2 + 1] = '.';
					}
					if (c1 + 1 <= 9) {
						field[c1 + 1][checkCoordinate2 + 1] = '.';
					}
				}
				shipLen++;
			}
			checkCoordinate2++;
		}
	}
	shipCount[shipLen]--;


	//отмечаем смежные с убитым кораблём клетки как пустые
	if (isHorizontal == 1) {
		if (c1 - 1 >= 0) {
			field[c1 - 1][c2] = '.';
		}
		if (c1 + 1 <= 9) {
			field[c1 + 1][c2] = '.';
		}
		if ((c2 - 1 >= 0) && (field[c1][c2 - 1] != '*')) {
			field[c1][c2 - 1] = '.';
			if (c1 - 1 >= 0) {
				field[c1 - 1][c2 - 1] = '.';
			}
			if (c1 + 1 <= 9) {
				field[c1 + 1][c2 - 1] = '.';
			}
		}
		if ((c2 + 1 <= 9) && (field[c1][c2 + 1] != '*')) {
			field[c1][c2 + 1] = '.';
			if (c1 - 1 >= 0) {
				field[c1 - 1][c2 + 1] = '.';
			}
			if (c1 + 1 <= 9) {
				field[c1 + 1][c2 + 1] = '.';
			}
		}
	}
	else {
		if (c2 - 1 >= 0) {
			field[c1][c2 - 1] = '.';
		}
		if (c2 + 1 <= 9) {
			field[c1][c2 + 1] = '.';
		}
		if ((c1 - 1 >= 0) && (field[c1 - 1][c2] != '*')) {
			field[c1 - 1][c2] = '.';
			if (c2 - 1 >= 0) {
				field[c1 - 1][c2 - 1] = '.';
			}
			if (c2 + 1 <= 9) {
				field[c1 - 1][c2 + 1] = '.';
			}
		}
		if ((c1 + 1 <= 9) && (field[c1 + 1][c2] != '*')) {
			field[c1 + 1][c2] = '.';
			if (c2 - 1 >= 0) {
				field[c1 + 1][c2 - 1] = '.';
			}
			if (c2 + 1 <= 9) {
				field[c1 + 1][c2 + 1] = '.';
			}
		}
	}
}



int nextTurn(struct Coordinate aim, char field[FIELD_SIZE][FIELD_SIZE], int shipCount[]) {
	///если в соседних клетках не все части корабля повреждены, то заменяем клетку на "x"
	//если в соседних клетках все части корабля повреждены(или он типа 1), то заменяем все клетки корабля на "*"

	int c1 = aim.ch - 'A';
	int c2 = aim.num - 1;

	if (field[c1][c2] == ' ') {//попадание мимо
		field[c1][c2] = '.';
		return 0;
	}
	if (field[c1][c2] != 'o') {//повторный выбор клетки для атаки
		return -1;
	}

	if (isKilled(aim, field) == 0) {
		field[c1][c2] = 'x';
		return 1;//корабль 'ранен'
	}
	else {
		///заменить все клетки корабля на *
		destroyShip(aim, field, shipCount);
	}
	if (getSumm(shipCount, 4) == 0) {
		return 3;//game over
	}
	return 2;//корабль 'убит'
}



void printGameSpace(char player1[FIELD_SIZE][FIELD_SIZE], char player2[FIELD_SIZE][FIELD_SIZE], int botCount[], int playerCount[]) {
	printf_s("Ваше поле:\n");
	printField(player1, 1);// 1 - игрок 1
	printShipsCount(playerCount);
	printf_s("\nПоле противника:\n");
	printField(player2, 2);//2 - игрок 2
	printShipsCount(botCount);
}



int hitTarget(struct Coordinate target, char field[FIELD_SIZE][FIELD_SIZE], int shipCount[]) {

	//верх
	struct Coordinate aim;
	aim.num = target.num;
	aim.ch = target.ch - 1;
	int x;//результат хода

	if ((aim.ch - 'A' >= 0) && ((field[aim.ch - 'A'][aim.num - 1] == 'o') || (field[aim.ch - 'A'][aim.num - 1] == ' '))) {
		x = nextTurn(aim, field, shipCount);
		printf_s("\n%d%c\n", aim.num, aim.ch);
		while ((x == 1) && (aim.ch - 'A' - 1 >= 0)) {
			aim.ch--;
			x = nextTurn(aim, field, shipCount);
			printf_s("\n%d%c\n", aim.num, aim.ch);
		}
		if (x == 2) {
			return 1;//success
		}
		return -1;//корабль не полностью разрушен
	}

	//низ
	aim.ch = target.ch + 1;
	if ((aim.ch - 'A' <= 9) && ((field[aim.ch - 'A'][aim.num - 1] == 'o') || (field[aim.ch - 'A'][aim.num - 1] == ' '))) {
		x = nextTurn(aim, field, shipCount);
		printf_s("\n%d%c\n", aim.num, aim.ch);
		while ((x == 1) && (aim.ch - 'A' + 1 <= 9)) {
			aim.ch++;
			x = nextTurn(aim, field, shipCount);
			printf_s("\n%d%c\n", aim.num, aim.ch);
		}
		if (x == 2) {
			return 1;//success
		}
		return -1;//корабль не полностью разрушен
	}
	//лево
	aim.ch = target.ch;
	aim.num = target.num - 1;
	if ((aim.num > 0) && ((field[aim.ch - 'A'][aim.num - 1] == 'o') || (field[aim.ch - 'A'][aim.num - 1] == ' '))) {//>=0
		x = nextTurn(aim, field, shipCount);
		printf_s("\n%d%c\n", aim.num, aim.ch);
		while ((x == 1) && (aim.num - 1 > 0)) {//aim.num - 2 >= 0
			aim.num--;
			x = nextTurn(aim, field, shipCount);
			printf_s("\n%d%c\n", aim.num, aim.ch);
		}
		if (x == 2) {
			return 1;//success
		}
		return -1;//корабль не полностью разрушен
	}
	//право
	aim.num = target.num + 1;
	if ((aim.num <= 10) && ((field[aim.ch - 'A'][aim.num - 1] == 'o') || (field[aim.ch - 'A'][aim.num - 1] == ' '))) {//<= 9
		x = nextTurn(aim, field, shipCount);
		printf_s("\n%d%c\n", aim.num, aim.ch);
		while ((x == 1) && (aim.num + 1 <= 10)) {
			aim.num++;
			x = nextTurn(aim, field, shipCount);
			printf_s("\n%d%c\n", aim.num, aim.ch);
		}
		if (x == 2) {
			return 1;//success
		}
		return -1;//корабль не полностью разрушен
	}
}

int search(char field[FIELD_SIZE][FIELD_SIZE], int shipCount[], struct Coordinate* aim) {
	int currX = 3;
	int x;
	int currY = 0;
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

	currX = 1;
	currY = 0;
	while (currY < 10) {//вторые диагонали (поиск кораблей длины 3 и 2)
		aim->ch = currX + 'A';
		aim->num = currY + 1;
		x = nextTurn(*aim, field, shipCount);
		if (x != -1) {
			printf_s("\n%d%c\n", aim->num, aim->ch);
		}
		if ((x != -1) && (x != 2)) {//продолжаем поиск, если убили корабль или посетили уже проверенную клетку

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
	for (int i = 0; i < 10; i++) {//поиск кораблей длины 1
		for (int j = 0; j < 10; j++) {
			if ((field[i][j] == 'o') || (field[i][j] == ' ')) {
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

struct Coordinate botTurn(char enemyField[FIELD_SIZE][FIELD_SIZE], char field[FIELD_SIZE][FIELD_SIZE], int botShipCount[], int playerShipCount[], struct Coordinate currAim) {
	int y = 0;//результат добивания корабля
	int x;
	if ((currAim.ch == '0') && (currAim.num == 0)) {//если нет текущей цели
		x = search(enemyField, playerShipCount, &currAim);//находим её
	}
	else {
		x = 1;
	}

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


struct Coordinate getCoordinate(char field[FIELD_SIZE][FIELD_SIZE]) {
	struct Coordinate c;
	char ch = '0';//символ после координат
	int x = scanf_s("%d%c", &c.num, &c.ch);
	if (c.ch != '\n') {//если было введено не только число
		ch = getchar();//получаем символ после введённых координат
	}
	while ((c.ch < 'A') || (c.ch > 'J') || (c.num < 1) || (c.num > 10) || (ch != '\n') || (x != 2)) {
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
	}
	if ((field[c.ch - 'A'][c.num - 1] != 'o') && (field[c.ch - 'A'][c.num - 1] != ' ')) {
		printf_s("\nДанная точка уже проверена. Введите новые координаты: ");
		return getCoordinate(field);
	}
	return c;
}


int getType() {
	int currType;
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
	return currType;
}