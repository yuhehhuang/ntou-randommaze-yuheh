#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#define MAX 51 //最大51
int field[MAX][MAX];
int count;//地圖的大小
void Initialize(int count);//初始化地圖
int IsHaveNeighbor(int X_index, int Y_index);//判斷現在的位置是否有鄰居(有0)
void Creat(int X_index, int Y_index);
typedef struct node { //定義型態名稱node叫Q_node，內容是地圖座標，以及步數
	int x;
	int y;
	int stp;
	struct node* next;
}Q_node;
Q_node* front = NULL;  //型態Q_node 的front是放位置的 並把NULL放入
Q_node* tail = NULL;    //型態Q_node 的tail是放位置的 並把NULL放入
typedef struct prelocation { //定義型態名稱prelocation叫record，內容是現在位置的前一個位置是誰(父節點)
	int prex;
	int prey;
}record;
record pre[51][51]; //資料型態record的二維矩陣pre，待會拿來放父節點
void push(int a, int b, int c) {
	Q_node* new_add_node; //存位址是資料型態Q_node的，所以找到的資料是有三個int跟一個指標
	new_add_node = (Q_node*)malloc(sizeof(struct node)); //指標 = (資料型態 *) malloc (所需要的記憶體空間，他的bytes數);
	new_add_node->x = a;
	new_add_node->y = b;
	new_add_node->stp = c;
	new_add_node->next = NULL;//我輸入的data所在的結構其內部的指標先設成NULL;

	if (tail == NULL)
		front = new_add_node; //把一開始的data位址放進front(new add node的值等同第一筆資料的位址)
	else
		tail->next = new_add_node; //從第一個資料裡的結構中指標NULL改成下一個資料的位址

	tail = new_add_node; //PS(一開始tail front的值都是第一個data的位址)，把新加入的data位址設為tail
}
int pop() {
	Q_node* pt = front;
	int i = front->x;
	int o = front->y;
	int u = front->stp;
	front = front->next;
	free(pt); //free(void 釋放分配出去的記憶體空間，他對應的記憶體位址);
	return i, o, u;
}
int isEmpty() {
	if (front == NULL)
		return 1;
	else
		return 0;
}
int getFront() {
	return front->x, front->y, front->stp;
}
int IsHaveNeighbor(int X_index, int Y_index)
{

	int i, j, flag = 0;
	if ((X_index >= 3 && field[X_index - 2][Y_index] == 0) || (X_index < count - 3 && field[X_index + 2][Y_index] == 0) || (Y_index >= 3 && field[X_index][Y_index - 2] == 0) || (Y_index < count - 3 && field[X_index][Y_index + 2] == 0))
		return 1; //上下左右

	else return 0;


}
void Creat(int X_index, int Y_index)
{
	int rand_position, x, y, flag = 0;
	x = X_index;
	y = Y_index;
	//如果四個方向都没有了，返回上一步，否則，繼續
	while (1)
	{
		flag = 0;
		flag = IsHaveNeighbor(X_index, Y_index);
		if (flag == 0) //表示附近沒有鄰居使我拓展通道
		{
			return;
		}
		else
		{
			field[X_index][Y_index] = 5;
			x = X_index;
			y = Y_index;
			while (1)
			{
				rand_position = rand() % 4;
				if (rand_position == 0 && X_index >= 3 && field[X_index - 2][Y_index] == 0)//上
				{
					X_index = X_index - 2;
				}
				else if (rand_position == 1 && (X_index <= (count - 2) && field[X_index + 2][Y_index] == 0))//下
				{
					X_index = X_index + 2;;
				}
				else if (rand_position == 2 && (Y_index >= 3 && field[X_index][Y_index - 2] == 0))//左
				{
					Y_index = Y_index - 2;
				}
				else if (rand_position == 3 && (Y_index <= (count - 2) && field[X_index][Y_index + 2] == 0))//右
				{
					Y_index = Y_index + 2;
				}
				field[(x + X_index) / 2][(y + Y_index) / 2] = 5;
				field[X_index][Y_index] = 5;
				Creat(X_index, Y_index);
				break;
			}
		}
	}
}
void Initialize(int a)
{
	int i, j;
	if (a % 2 == 0)
		count++;
	for (i = 0; i < count; i++)	//將地圖先初始為牆壁
		for (j = 0; j < count; j++)
			field[i][j] = 1;
	//將所需地圖以外設為-1(表用不到),其餘通道跟牆壁間格
	for (i = 0; i < MAX; i++)
	{
		for (j = 0; j < MAX; j++)
		{
			if (i > count || j > count)
				field[i][j] = -1;
			if (i != 0 && j != 0 && i != count + 1 && j != count + 1) //最外牆內的奇數位置訂為通道
			{
				if (i % 2 == 1)
					if (j % 2 == 1)
						field[i][j] = 0;
			}
		}
	}

	Creat(1, 1);
	for (i = 1; i <= count; i++)
	{
		for (j = 1; j <= count; j++)
			if (field[i][j] == 5)
				field[i][j] = 0;
	}

}
int main(void)
{
	printf("請輸入迷宮的大小(一個奇數):\n");
	printf("(提示:請用一個數字表示且不可大於等於50，若輸入為偶數會自動+1為奇數)\n");
	scanf("%d", &count);
	while (count < 50) {
		srand((unsigned)time(NULL));
		Initialize(count);
		int changex[2500];
		for (int i = 0; i < 2500; i++) { //因為我的輸出是從尾巴推到起頭 我要建一個陣列先放那些父節點，用於輸出時從頭開始推到尾
			changex[i] = 0;
		}
		int changey[2500];
		for (int i = 0; i < 2500; i++) {
			changey[i] = 0;
		}

		for (int i = 1; i <= count; i++) {
			for (int j = 1; j <= count; j++) {
				if (field[i][j] == 0) //通道
					printf("■");
				if (field[i][j] == 1) //牆
					printf("⊙");

			}
			printf("\n");

		}
		for (int i = 0; i < 51; i++) {
			for (int j = 0; j < 51; j++) {
				pre[i][j].prex = 9; //清空
				pre[i][j].prex = 9;
			}
		}

		printf("\nPS(■:可通行，⊙:牆壁)\n");
		int initial[2], end[2];

		int checked[50][50] = { 0 };

		int dx[8] = { -1,-1,0,1,1,1,0,-1 }; //方向，上，右上，右，右下，下，左下，左，左上，我們看座標跟電腦相反
		int dy[8] = { 0,1,1,1,0,-1,-1,-1 };
		printf("請輸入迷宮的起點與終點(我的迷宮最左上為(1,1)，最左下是(1，%d),最右上是(%d,1),最右下是(%d，%d)\n", count, count, count, count);
		printf("輸入起點座標(以空白鍵將兩個整數間格，輸入完後請按Enter):\n");
		for (int i = 1; i >= 0; i--) { //我們看座標跟電腦相反
			scanf("%d", &initial[i]);
		}
		printf("輸入終點座標(以空白鍵將兩個整數間格，輸入完後請按Enter):\n");
		for (int i = 1; i >= 0; i--) {
			scanf("%d", &end[i]);
		}
		if (field[initial[0]][initial[1]] == 0 && field[end[0]][end[1]] == 0) { //起點終點都可以走
			//pre[initial[0]][initial[1]].prex = 0; //把初始位置的父節點設定
			//pre[initial[0]][initial[1]].prey = 0;
			push(initial[0], initial[1], 1);
			checked[initial[0]][initial[1]] = 3;//表示已入列
			while (isEmpty() == 0) { //queue不為空(我們訂1的時候是空)
				int x = front->x, y = front->y;
				if (x == end[0] && y == end[1]) {
					printf("\n最短路徑長:%d(提示:這個迷宮可以斜著走，步數也跟直走相同，且起點處算第一步)\n", front->stp);

					break;
				}
				for (int k = 0; k < 8; k++) { //方向，上，右上，右，右下，下，左下，左，左上
					int tx, ty;
					tx = x + dx[k];
					ty = y + dy[k];
					if (field[tx][ty] == 0 && checked[tx][ty] == 0) { //把這8個方向的確認是否能走(為0)，且還未放到佇列
						struct node tmp;
						tmp.x = tx;
						tmp.y = ty;
						tmp.stp = front->stp + 1;
						pre[tx][ty].prex = x;
						pre[tx][ty].prey = y;
						push(tx, ty, tmp.stp);
						checked[tx][ty] = 3;

					}
				}
				pop();
			}

			int target0 = end[0];
			int target1 = end[1];
			int start = 0;
			while (!(target0 == initial[0] && target1 == initial[1])) { //找到的父節點是起點後BREAK出FOR迴圈，此時的
				for (int i = 50; i >= 0; i--) {                     //target0 == initial[0] && target1 == initial[1],所以我跳出while的條件也要假設相同
					for (int j = 50; j >= 0; j--) {
						if (target0 == initial[0] && target1 == initial[1])
							break;
						else if (i == target0 && j == target1)
						{

							target0 = pre[i][j].prex; //父節點變成下一個蒐尋對象
							target1 = pre[i][j].prey;
							changex[start] = target0;//第一個放進去change的是結尾的父節點，所以我在最後還要補結尾的座標
							changey[start] = target1;
							start++;

						}
						else
							continue;


					}
				}
			}
			printf("路徑是:\n");
			for (int i = 2499; i >= 0; i--) {    //一開始初始陣列裡都是0，第一個不是0的就是我第一放進去的值，也就是結尾的父親
				if (changex[i] != 0 && changey[i] != 0)
					printf("(%d,%d)\n", changey[i], changex[i]);
			}

			printf("(%d,%d)\n", end[1], end[0]);
		}


		else
			printf("起點或終點是牆壁，沒有辦法走出迷宮\n");
		system("pause");
		return 0;

	}
	printf("********輸入的數字超過50(不符規定)**********");
	system("pause");
	return 0;
}