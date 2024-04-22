#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 15
#define LENGTH 6
#define WISE 7
#define BLACKtem 10
#define WHITEtem 20
#define BLACK 11
#define WHITE 21
#define BLACKpre 12

int Board[LENGTH][WISE];
int m, n;								//黑子，白子胜负判定
int sum[2][LENGTH][WISE];		//黑白子位置得分，0层存黑子，1层存白子
//int ImagineBoard[LENGTH][WISE];		//想象某个位置落子后的棋盘
int summing[LENGTH][WISE] = { {0} };	//预测某个位置落子后棋盘上各个位置得分
int top[WISE];

void InitBoardArray();					//初始化空棋盘
void DisplayBoard();					//显示棋盘
int choose();							//选择先后手
//void player1();						//输入黑子
void player2();							//输入白子
void computer1();						//给出黑子
//void computer2();						//给出白子
void FindBlackPoint(int* x, char* y);	//寻找黑子最佳落子位置
void FindWhitePoint(int* x, char* y);	//寻找白子最佳落子位置
void grade();							//计算位置得分
int judge(int x, char y);				//判断四子连珠
int JudgeDisplay(int a, int b);			//输出胜负结果
int draw();								//棋盘已满，判平
void pregrade(int j, int i);			//输入高分坐标（j, i），预测这一子落下后各位置得分
int IsSolid(int j, int i, int colour, char dir);
										//输入高分坐标(j, i)，判断是否有效（有可能四子成珠）,colour来分辨高分坐标颜色，dir指示连珠方向

int main()
{
	InitBoardArray();
	DisplayBoard();
	for (int layer = 0; layer < 2; layer++) {
		for (int len = 0; len < LENGTH; len++) {
			for (int wi = 0; wi < WISE; wi++)
				sum[layer][len][wi] = 0;
		}
	}
	for (int j = 0; j < WISE; j++)
		top[j] = LENGTH - 1;

	switch (choose()) {
		/*
	case 1:							//人人对战
		while (draw()) {
			player1();
			if (JudgeDisplay(m, n))
				break;
			player2();
			if (JudgeDisplay(m, n))
				break;
		}
		break;
		*/

	case 2:							//人机对战先输入（游戏中后手）
		while (draw()) {
			player2();
			if (JudgeDisplay(m, n))
				break;
			computer1();
			if (JudgeDisplay(m, n))
				break;
		}
		break;

	case 1:							//人机对战后输入（游戏中先手）
		Board[LENGTH - 1][WISE / 2] = BLACKtem;
		top[WISE / 2] -= 1;
		DisplayBoard();
		Board[LENGTH - 1][WISE / 2] = BLACK;
		printf("黑子已落在：d1\n");
		while (draw()) {
			player2();
			if (JudgeDisplay(m, n))
				break;
			computer1();
			if (JudgeDisplay(m, n))
				break;
		}
		break;
	}

	printf("游戏结束。重新开始游戏输入1，退出游戏输入0:");
	int i;
	scanf_s("%d", &i);
	if (i == 1)
		main();
	else if (i == 0)
		return 0;
}

void InitBoardArray()
{
	int j, i;

	Board[0][0] = 1;
	Board[0][WISE - 1] = 2;
	Board[LENGTH - 1][WISE - 1] = 3;
	Board[LENGTH - 1][0] = 4;

	for (j = 1; j <= LENGTH - 2; j++)
	{
		Board[j][0] = 5;
	}

	for (i = 1; i <= WISE - 2; i++)
	{
		Board[0][i] = 6;
	}

	for (j = 1; j <= LENGTH - 2; j++)
	{
		Board[j][WISE - 1] = 7;
	}

	for (i = 1; i <= WISE - 2; i++)
	{
		Board[LENGTH - 1][i] = 8;
	}

	for (j = 1; j <= LENGTH - 2; j++)
	{
		for (i = 1; i <= WISE - 2; i++)
		{
			Board[j][i] = 9;
		}
	}

}

void DisplayBoard()
{
	int i, j;
	int line;
	char ary;

	system("cls");   //清屏

	for (j = 0, line = LENGTH; j <= LENGTH - 1; j++)
	{
		printf("%2d", line);
		line -= 1;
		for (i = 0; i <= WISE - 1; i++)
		{
			switch (Board[j][i])
			{
			case 1:
				printf("┏");
				break;

			case 2:
				printf("┓");
				break;

			case 3:
				printf("┛");
				break;

			case 4:
				printf("┗");
				break;

			case 5:
				printf("┠");
				break;

			case 6:
				printf("┯");
				break;

			case 7:
				printf("┨");
				break;

			case 8:
				printf("┷");
				break;

			case 9:
				printf("┼");
				break;

			case BLACKtem:      // 黑子上一步
				printf("▲");
				break;

			case WHITEtem:      //白子上一步
				printf("△");
				break;

			case BLACK:      //黑子通常
				printf("●");
				break;

			case WHITE:
				printf("○");  //白子通常
				break;
			}
			if (i == WISE - 1)
			{
				printf("\n");
			}

		}
	}

	printf("  abcdefg");

	printf("\n");
}

int choose()
{
	printf("请输入对应数字选择游戏模式：\n");
	//printf("1:??????\n");
	printf("1:黑子先手\n");
	printf("2:黑子后手\n");

	int choice;
	scanf_s("%d", &choice);
	getchar();

	return choice;
}


void player2()
{
	char y;
	int x;

	printf("请输入白子所在列（小写字母）\n");
	scanf_s("%c", &y, 1);
	getchar();
	x = top[y - 'a'];

	//坐标在棋盘外
	while (x < 0 || x >= LENGTH || (y - 'a') < 0 || (y - 'a') >= WISE) {
		printf("请将棋子落在棋盘上\n");
		printf("请重新输入白子所在列（小写字母）\n");
		scanf_s("%c", &y, 1);
		getchar();
		x = top[y - 'a'];
	}

	Board[x][y - 'a'] = WHITEtem;
	top[y - 'a'] -= 1;
	DisplayBoard();
	Board[x][y - 'a'] = WHITE;

	n = judge(x, y);
}

void computer1()
{
	char y;
	int x;

	FindBlackPoint(&x, &y);
	Board[x][y - 'a'] = BLACKtem;
	top[y - 'a'] -= 1;
	DisplayBoard();
	printf("黑子已落在：%c%d\n", y, LENGTH - x);
	for (int i = 0; i < WISE; i++)
		printf("%d ", top[i]);
	printf("\n");
	printf("每个点得分（落子前）：\n");
	for (int q = 0; q < WISE; q++) {
		int p = top[q];
		if (q == y - 'a')
			printf("%d ", sum[0][p + 1][q]);
		else if (p >= 0)
			printf("%d ", sum[0][p][q]);
		else
			printf("x ");
	}
	printf("\n");
	Board[x][y - 'a'] = BLACK;

	m = judge(x, y);
}


void FindBlackPoint(int* x, char* y)
{
	int i, j;
	int t = 0, k;
	int r, c = 0;
	int blank = 0;
	int a, b = 0;
	int sumed[LENGTH * WISE][2] = { {0} };

	//找到黑子最大得分
	grade();
	for (j = 0; j < WISE; j++) {
		i = top[j];
		if (i >= 0 && sum[0][i][j] > t)
			t = sum[0][i][j];

	}

	//记录黑子得分取最大且空着的坐标，白子得分尽可能大，且得分有效
//found1:
	//黑子得分取最大的点中，白子得分最大的点
	printf("t=%d\n", t);
	k = 0;
	for (j = 0; j < WISE; j++) {
		i = top[j];
		if (i >= 0 && sum[0][i][j] == t && sum[1][i][j] > k) {
			k = sum[1][i][j];
		}

	}

	for (j = 0; j < WISE; j++) {
		i = top[j];
		if (i >= 0 && sum[0][i][j] == t && sum[1][i][j] == k) {
			sumed[c][0] = i;
			sumed[c][1] = j;
			c++;
		}

	}

	srand((unsigned)time(NULL));
	r = rand() % c;

	/*
	//检查是否还有有效分值坐标
	for (j = 0; j < WISE; j++) {
		i = top[j];
		if (i >= 0 && IsSolid(i, j, BLACK))
			blank++;

	}
	if (blank > 0) {
		for (a = 0; a < c; a++) {
			if (IsSolid(sumed[a][0], sumed[a][1], BLACK))
				b++;
		}
		if (b == 0) {
			t--;
			goto found1;
		}
		else if (b > 0) {
			while (IsSolid(sumed[c][0], sumed[c][1], BLACK) == 0) {
				srand((unsigned)time(NULL));
				r = rand() % c;
			}
		}
	}*/
	*x = sumed[r][0];
	*y = 'a' + sumed[r][1];

}


void grade()
{
	int p, q;
	int i;

	int a1, b1, c1, d1;		//记录黑子四个方向连续相同棋子数
	int a2, b2, c2, d2;		//记录白子四个方向连续相同棋子数

	for (p = 0; p < LENGTH; p++) {
		for (q = 0; q < WISE; q++) {
			//黑子a1,b1,c1,d1	
			a1 = 0;
			b1 = 0;
			c1 = 0;
			d1 = 0;
			//横向
			for (i = 1; i < 4; i++) {
				if (q + i < WISE && (Board[p][q + i] / 10) == 1)
					a1++;
				else
					break;
			}
			for (i = 1; i < 4; i++) {
				if (q - i >= 0 && (Board[p][q - i] / 10) == 1)
					a1++;
				else
					break;
			}
			//纵向
			for (i = 1; i < 4; i++) {
				if (p + i < LENGTH && (Board[p + i][q] / 10) == 1)
					b1++;
				else
					break;
			}
			for (i = 1; i < 4; i++) {
				if (p - i >= 0 && (Board[p - i][q] / 10) == 1)
					b1++;
				else
					break;
			}
			//左上-右下
			for (i = 1; i < 4; i++) {
				if (q + i < WISE && p + i < LENGTH && (Board[p + i][q + i] / 10) == 1)
					c1++;
				else
					break;
			}
			for (i = 1; i < 4; i++) {
				if (q - i >= 0 && p - i >= 0 && (Board[p - i][q - i] / 10) == 1)
					c1++;
				else
					break;
			}
			//右上-左下
			for (i = 1; i < 4; i++) {
				if (q + i < WISE && p - i >= 0 && (Board[p - i][q + i] / 10) == 1)
					d1++;
				else
					break;
			}
			for (i = 1; i < 4; i++) {
				if (q - i >= 0 && p + i < LENGTH && (Board[p + i][q - i] / 10) == 1)
					d1++;
				else
					break;
			}
			//白子a2,b2,c2,d2
			a2 = 0;
			b2 = 0;
			c2 = 0;
			d2 = 0;
			//横向
			for (i = 1; i < 4; i++) {
				if (q + i < WISE && (Board[p][q + i] / 10) == 2)
					a2++;
				else
					break;
			}
			for (i = 1; i < 4; i++) {
				if (q - i >= 0 && (Board[p][q - i] / 10) == 2)
					a2++;
				else
					break;
			}
			//纵向
			for (i = 1; i < 4; i++) {
				if (p + i < LENGTH && (Board[p + i][q] / 10) == 2)
					b2++;
				else
					break;
			}
			for (i = 1; i < 4; i++) {
				if (p - i >= 0 && (Board[p - i][q] / 10) == 2)
					b2++;
				else
					break;
			}
			//左上-右下
			for (i = 1; i < 4; i++) {
				if (q + i < WISE && p + i < LENGTH && (Board[p + i][q + i] / 10) == 2)
					c2++;
				else
					break;
			}
			for (i = 1; i < 4; i++) {
				if (q - i >= 0 && p - i >= 0 && (Board[p - i][q - i] / 10) == 2)
					c2++;
				else
					break;
			}
			//右上-左下
			for (i = 1; i < 4; i++) {
				if (q + i < WISE && p - i >= 0 && (Board[p - i][q + i] / 10) == 2)
					d2++;
				else
					break;
			}
			for (i = 1; i < 4; i++) {
				if (q - i >= 0 && p + i < LENGTH && (Board[p + i][q - i] / 10) == 2)
					d2++;
				else
					break;
			}

			//检查每个方向是否有效
			a1 = a1 & IsSolid(p, q, BLACK, 'a');
			b1 = b1 & IsSolid(p, q, BLACK, 'b');
			c1 = c1 & IsSolid(p, q, BLACK, 'c');
			d1 = d1 & IsSolid(p, q, BLACK, 'd');
			a2 = a2 & IsSolid(p, q, WHITE, 'a');
			b2 = b2 & IsSolid(p, q, WHITE, 'b');
			c2 = c2 & IsSolid(p, q, WHITE, 'c');
			d2 = d2 & IsSolid(p, q, WHITE, 'd');

			//根据四个方向连续同色棋子个数判断得分
			if (a1 >= 3 || b1 >= 3 || c1 >= 3 || d1 >= 3)
				sum[0][p][q] = 10000;	//如果能四子连珠，直接结束比赛
			switch (a1)
			{
			case 2:
				sum[0][p][q] += 100;
				break;
			case 1:
				sum[0][p][q] += 10;
				break;
			case 0:
				break;
			}
			switch (b1)
			{
			case 2:
				sum[0][p][q] += 100;
				break;
			case 1:
				sum[0][p][q] += 10;
				break;
			case 0:
				break;
			}
			switch (c1)
			{
			case 2:
				sum[0][p][q] += 100;
				break;
			case 1:
				sum[0][p][q] += 10;
				break;
			case 0:
				break;
			}
			switch (d1)
			{
			case 2:
				sum[0][p][q] += 100;
				break;
			case 1:
				sum[0][p][q] += 10;
				break;
			case 0:
				break;
			}

			switch (a2)
			{
			case 2:
				sum[1][p][q] += 100;
				break;
			case 1:
				sum[1][p][q] += 10;
				break;
			case 0:
				break;
			default:	//a2>=3
				sum[1][p][q] += 1000;
				break;
			}
			switch (b2)
			{
			case 2:
				sum[1][p][q] += 100;
				break;
			case 1:
				sum[1][p][q] += 10;
				break;
			case 0:
				break;
			default:	//b2>=3
				sum[1][p][q] += 1000;
				break;
			}
			switch (c2)
			{
			case 2:
				sum[1][p][q] += 100;
				break;
			case 1:
				sum[1][p][q] += 10;
				break;
			case 0:
				break;
			default:	//c2>=3
				sum[1][p][q] += 1000;
				break;
			}
			switch (d2)
			{
			case 2:
				sum[1][p][q] += 100;
				break;
			case 1:
				sum[1][p][q] += 10;
				break;
			case 0:
				break;
			default:	//d2>=3
				sum[1][p][q] += 1000;
				break;
			}
		}
		//记得删printf("\n\n");
	}

	//白子即将四子连珠（白子给自己垫脚）
	for (q = 0; q < WISE; q++) {
		p = top[q];
		if (p >= 0 && sum[1][p][q] >= 1000) {
			sum[0][p][q] += sum[1][p][q];
		}
	}
	//若落子后给白子四子连珠垫脚，分数为负
	for (q = 0; q < WISE; q++) {
		p = top[q];
		if (p > 0 && sum[1][p - 1][q] >= 1000) {
			sum[0][p][q] = -1;
		}
	}
	
}

int judge(int x, char y)
{
	int a = 1, b = 1, c = 1, d = 1;	//记录四个方向连续相同棋子数
	int i;

	//横向
	for (i = 1; i < 4; i++) {
		if ((y - 'a' + i) < WISE && (Board[x][y - 'a' + i] / 10) == (Board[x][y - 'a'] / 10))
			a++;
		else
			break;
	}
	for (i = 1; i < 4; i++) {
		if ((y - 'a' - i) >= 0 && (Board[x][y - 'a' - i] / 10) == (Board[x][y - 'a'] / 10))
			a++;
		else
			break;
	}
	if (a >= 4)
		return 1;

	//纵向
	for (i = 1; i < 4; i++) {
		if ((x + i) < LENGTH && (Board[x + i][y - 'a'] / 10) == (Board[x][y - 'a'] / 10))
			b++;
		else
			break;
	}
	for (i = 1; i < 4; i++) {
		if ((x - i) >= 0 && (Board[x - i][y - 'a'] / 10) == (Board[x][y - 'a'] / 10))
			b++;
		else
			break;
	}
	if (b >= 4)
		return 1;

	//左上-右下
	for (i = 1; i < 4; i++) {
		if ((y - 'a' + i) < WISE && (x + i) < LENGTH && (Board[x + i][y - 'a' + i] / 10) == (Board[x][y - 'a'] / 10))
			c++;
		else
			break;
	}
	for (i = 1; i < 4; i++) {
		if ((y - 'a' - i) >= 0 && (x - i) >= 0 && (Board[x - i][y - 'a' - i] / 10) == (Board[x][y - 'a'] / 10))
			c++;
		else
			break;
	}
	if (c >= 4)
		return 1;

	//右上-左下
	for (i = 1; i < 4; i++) {
		if ((y - 'a' + i) < WISE && (x - i) >= 0 && (Board[x - i][y - 'a' + i] / 10) == (Board[x][y - 'a'] / 10))
			d++;
		else
			break;
	}
	for (i = 1; i < 4; i++) {
		if ((y - 'a' - i) >= 0 && (x + i) < LENGTH && (Board[x + i][y - 'a' - i] / 10) == (Board[x][y - 'a'] / 10))
			d++;
		else
			break;
	}
	if (d >= 4)
		return 1;


	return 0;						//没有四子连珠
}

int JudgeDisplay(int a, int b)
{
	if (m == 1) {
		printf("黑子获胜\n");
		m = 0;
		return 1;
	}
	if (n == 1) {
		printf("白子获胜\n");
		n = 0;
		return 1;
	}

	return 0;
}

int draw()
{
	int p, q;
	int blank = 0;

	for (q = 0; q < WISE; q++) {
		p = top[q];
		if (p >= 0)
			blank++;

	}
	if (blank == 0) {
		printf("棋盘已满，双方平局。");
		return 0;
	}
	else
		return 1;
}


int IsSolid(int j, int i, int colour, char dir)
{
	int s = 1;	//四个方向可连续落子数
	int r;
	int opst;

	if (colour / 10 == 1) {//要落黑子
		opst = 2;
	}
	else//要落白子
		opst = 1;

	if (dir == 'b') {
		//纵向
		for (r = 1; j + r < LENGTH && (Board[j + r][i] / 10) != opst; r++) {
			s++;
		}
		for (r = 1; j - r >= 0 && (Board[j - r][i] / 10) != opst; r++) {
			s++;
		}
	}
	else if (dir == 'a') {
		//横向
		for (r = 1; i + r < WISE && (Board[j][i + r] / 10) != opst; r++) {
			s++;
		}
		for (r = 1; i - r >= 0 && (Board[j][i - r] / 10) != opst; r++) {
			s++;
		}
	}
	else if (dir == 'c') {
		//左上-右下
		for (r = 1; j + r < LENGTH && i + r < WISE && (Board[j + r][i + r] / 10) != opst; r++) {
			s++;
		}
		for (r = 1; j - r >= 0 && i - r >= 0 && (Board[j - r][i - r] / 10) != opst; r++) {
			s++;
		}
	}
	else if (dir == 'd') {
		//右下-左上
		for (r = 1; j - r >= 0 && i + r < WISE && (Board[j - r][i + r] / 10) != opst; r++) {
			s++;
		}
		for (r = 1; j + r < LENGTH && i - r >= 0 && (Board[j + r][i - r] / 10) != opst; r++) {
			s++;
		}
	}

	if (s < 4)
		return 0;
	else
		return 7;	//二进制表示为111
}