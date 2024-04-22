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
int m, n;								//���ӣ�����ʤ���ж�
int sum[2][LENGTH][WISE];		//�ڰ���λ�õ÷֣�0�����ӣ�1������
//int ImagineBoard[LENGTH][WISE];		//����ĳ��λ�����Ӻ������
int summing[LENGTH][WISE] = { {0} };	//Ԥ��ĳ��λ�����Ӻ������ϸ���λ�õ÷�
int top[WISE];

void InitBoardArray();					//��ʼ��������
void DisplayBoard();					//��ʾ����
int choose();							//ѡ���Ⱥ���
//void player1();						//�������
void player2();							//�������
void computer1();						//��������
//void computer2();						//��������
void FindBlackPoint(int* x, char* y);	//Ѱ�Һ����������λ��
void FindWhitePoint(int* x, char* y);	//Ѱ�Ұ����������λ��
void grade();							//����λ�õ÷�
int judge(int x, char y);				//�ж���������
int JudgeDisplay(int a, int b);			//���ʤ�����
int draw();								//������������ƽ
void pregrade(int j, int i);			//����߷����꣨j, i����Ԥ����һ�����º��λ�õ÷�
int IsSolid(int j, int i, int colour, char dir);
										//����߷�����(j, i)���ж��Ƿ���Ч���п������ӳ��飩,colour���ֱ�߷�������ɫ��dirָʾ���鷽��

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
	case 1:							//���˶�ս
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

	case 2:							//�˻���ս�����루��Ϸ�к��֣�
		while (draw()) {
			player2();
			if (JudgeDisplay(m, n))
				break;
			computer1();
			if (JudgeDisplay(m, n))
				break;
		}
		break;

	case 1:							//�˻���ս�����루��Ϸ�����֣�
		Board[LENGTH - 1][WISE / 2] = BLACKtem;
		top[WISE / 2] -= 1;
		DisplayBoard();
		Board[LENGTH - 1][WISE / 2] = BLACK;
		printf("���������ڣ�d1\n");
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

	printf("��Ϸ���������¿�ʼ��Ϸ����1���˳���Ϸ����0:");
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

	system("cls");   //����

	for (j = 0, line = LENGTH; j <= LENGTH - 1; j++)
	{
		printf("%2d", line);
		line -= 1;
		for (i = 0; i <= WISE - 1; i++)
		{
			switch (Board[j][i])
			{
			case 1:
				printf("��");
				break;

			case 2:
				printf("��");
				break;

			case 3:
				printf("��");
				break;

			case 4:
				printf("��");
				break;

			case 5:
				printf("��");
				break;

			case 6:
				printf("��");
				break;

			case 7:
				printf("��");
				break;

			case 8:
				printf("��");
				break;

			case 9:
				printf("��");
				break;

			case BLACKtem:      // ������һ��
				printf("��");
				break;

			case WHITEtem:      //������һ��
				printf("��");
				break;

			case BLACK:      //����ͨ��
				printf("��");
				break;

			case WHITE:
				printf("��");  //����ͨ��
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
	printf("�������Ӧ����ѡ����Ϸģʽ��\n");
	//printf("1:??????\n");
	printf("1:��������\n");
	printf("2:���Ӻ���\n");

	int choice;
	scanf_s("%d", &choice);
	getchar();

	return choice;
}


void player2()
{
	char y;
	int x;

	printf("��������������У�Сд��ĸ��\n");
	scanf_s("%c", &y, 1);
	getchar();
	x = top[y - 'a'];

	//������������
	while (x < 0 || x >= LENGTH || (y - 'a') < 0 || (y - 'a') >= WISE) {
		printf("�뽫��������������\n");
		printf("������������������У�Сд��ĸ��\n");
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
	printf("���������ڣ�%c%d\n", y, LENGTH - x);
	for (int i = 0; i < WISE; i++)
		printf("%d ", top[i]);
	printf("\n");
	printf("ÿ����÷֣�����ǰ����\n");
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

	//�ҵ��������÷�
	grade();
	for (j = 0; j < WISE; j++) {
		i = top[j];
		if (i >= 0 && sum[0][i][j] > t)
			t = sum[0][i][j];

	}

	//��¼���ӵ÷�ȡ����ҿ��ŵ����꣬���ӵ÷־����ܴ��ҵ÷���Ч
//found1:
	//���ӵ÷�ȡ���ĵ��У����ӵ÷����ĵ�
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
	//����Ƿ�����Ч��ֵ����
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

	int a1, b1, c1, d1;		//��¼�����ĸ�����������ͬ������
	int a2, b2, c2, d2;		//��¼�����ĸ�����������ͬ������

	for (p = 0; p < LENGTH; p++) {
		for (q = 0; q < WISE; q++) {
			//����a1,b1,c1,d1	
			a1 = 0;
			b1 = 0;
			c1 = 0;
			d1 = 0;
			//����
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
			//����
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
			//����-����
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
			//����-����
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
			//����a2,b2,c2,d2
			a2 = 0;
			b2 = 0;
			c2 = 0;
			d2 = 0;
			//����
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
			//����
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
			//����-����
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
			//����-����
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

			//���ÿ�������Ƿ���Ч
			a1 = a1 & IsSolid(p, q, BLACK, 'a');
			b1 = b1 & IsSolid(p, q, BLACK, 'b');
			c1 = c1 & IsSolid(p, q, BLACK, 'c');
			d1 = d1 & IsSolid(p, q, BLACK, 'd');
			a2 = a2 & IsSolid(p, q, WHITE, 'a');
			b2 = b2 & IsSolid(p, q, WHITE, 'b');
			c2 = c2 & IsSolid(p, q, WHITE, 'c');
			d2 = d2 & IsSolid(p, q, WHITE, 'd');

			//�����ĸ���������ͬɫ���Ӹ����жϵ÷�
			if (a1 >= 3 || b1 >= 3 || c1 >= 3 || d1 >= 3)
				sum[0][p][q] = 10000;	//������������飬ֱ�ӽ�������
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
		//�ǵ�ɾprintf("\n\n");
	}

	//���Ӽ����������飨���Ӹ��Լ���ţ�
	for (q = 0; q < WISE; q++) {
		p = top[q];
		if (p >= 0 && sum[1][p][q] >= 1000) {
			sum[0][p][q] += sum[1][p][q];
		}
	}
	//�����Ӻ���������������ţ�����Ϊ��
	for (q = 0; q < WISE; q++) {
		p = top[q];
		if (p > 0 && sum[1][p - 1][q] >= 1000) {
			sum[0][p][q] = -1;
		}
	}
	
}

int judge(int x, char y)
{
	int a = 1, b = 1, c = 1, d = 1;	//��¼�ĸ�����������ͬ������
	int i;

	//����
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

	//����
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

	//����-����
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

	//����-����
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


	return 0;						//û����������
}

int JudgeDisplay(int a, int b)
{
	if (m == 1) {
		printf("���ӻ�ʤ\n");
		m = 0;
		return 1;
	}
	if (n == 1) {
		printf("���ӻ�ʤ\n");
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
		printf("����������˫��ƽ�֡�");
		return 0;
	}
	else
		return 1;
}


int IsSolid(int j, int i, int colour, char dir)
{
	int s = 1;	//�ĸ����������������
	int r;
	int opst;

	if (colour / 10 == 1) {//Ҫ�����
		opst = 2;
	}
	else//Ҫ�����
		opst = 1;

	if (dir == 'b') {
		//����
		for (r = 1; j + r < LENGTH && (Board[j + r][i] / 10) != opst; r++) {
			s++;
		}
		for (r = 1; j - r >= 0 && (Board[j - r][i] / 10) != opst; r++) {
			s++;
		}
	}
	else if (dir == 'a') {
		//����
		for (r = 1; i + r < WISE && (Board[j][i + r] / 10) != opst; r++) {
			s++;
		}
		for (r = 1; i - r >= 0 && (Board[j][i - r] / 10) != opst; r++) {
			s++;
		}
	}
	else if (dir == 'c') {
		//����-����
		for (r = 1; j + r < LENGTH && i + r < WISE && (Board[j + r][i + r] / 10) != opst; r++) {
			s++;
		}
		for (r = 1; j - r >= 0 && i - r >= 0 && (Board[j - r][i - r] / 10) != opst; r++) {
			s++;
		}
	}
	else if (dir == 'd') {
		//����-����
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
		return 7;	//�����Ʊ�ʾΪ111
}