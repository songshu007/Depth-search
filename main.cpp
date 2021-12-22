#include <graphics.h>
#include <Windows.h>
#include <math.h>
#include <vector>
#include <iostream>

using namespace std;

#define WIDTH 1080//分辨率
#define HEIGHT 720
#define BLOCK_XY 10//小方格边长
const int BLOCK_WIDTH = WIDTH / (BLOCK_XY);//小方格x轴数量
const int BLOCK_HEIGHT = HEIGHT / (BLOCK_XY);//小方格y轴数量

#define kongqi COLORREF RGB(255,255,255)//空气颜色
#define qiangbi COLORREF RGB(50,50,50)//墙壁颜色
#define xuanzhong COLORREF RGB(255,0,0)//选中颜色

struct block{
public:
	int x;
	int y;
	block(int x, int y) {
		this->x = x;
		this->y = y;
	}
};

int map[BLOCK_WIDTH][BLOCK_HEIGHT];
vector<block*> openlist;//待选列表,此列表记录所有的路

vector<block*> list;//临时的列表
vector<block*> list2;//临时的列表

/*函数声明*/
void clearscreen(COLORREF RGB);//清空地图为RGB颜色(带有网格)
void gamedrawpixal(int x, int y, COLORREF RGB);//在x，y位置画颜色为RGB的方格
bool init();//初始化
void display();//刷新屏幕
bool prim_main();//prim主循环
void through(int x, int y);//选中x,y四周一个随机的为墙壁的路点加入待选列表将其变成路,并与x,y打通
bool check(int x, int y);//检查x，y周围有没有墙，有墙返回1，无墙返回0
/*********/

bool init() {
	openlist.clear();
	/*地图初始化为墙*/
	for (int y = 0; y < BLOCK_HEIGHT; y++) {
		for (int x = 0; x < BLOCK_WIDTH; x++) {
			map[x][y] = 1;
		}
	}
	/*随机选一个路点，将它变成路*/
	map[40][40] = 0;
	/*将该点加入待选列表*/
	block* s = new block(40, 40);
	openlist.push_back(s);
	/*开始主循环*/
	if(prim_main() == 1) return 1;//如果迷宫走完返回1
}
bool prim_main() {
	while (openlist.size() > 0){
		/*从待选列表选最后一个路点A*/
		int AIndexea = openlist.size() - 1;
		block* A = openlist[AIndexea];
		/*检查是否进入了死胡同,如果进入了死胡同,则回溯*/
		while (!check(A->x, A->y)) {
			A = openlist[AIndexea];
			if (AIndexea == 0) return 1;//如果迷宫走完返回1
			--AIndexea;
		}
		/*将A与它四周一个随机的为墙壁的路点打通,并将选中路点变为路,并加入待选列表*/
		through(A->x, A->y);

		gamedrawpixal(A->x, A->y, xuanzhong);
		FlushBatchDraw();
		display();
	}
}
void through(int x, int y) {
	list.clear();
	list2.clear();
	//上
	if (y - 2 >= 0 && map[x][y - 2] == 1) {
		block* a = new block(x, y - 1);
		block* aa = new block(x, y - 2);
		list.push_back(a);
		list2.push_back(aa);
	}
	//下
	if (y + 2 <= BLOCK_HEIGHT - 1 && map[x][y + 2] == 1) {
		block* b = new block(x, y + 1);
		block* bb = new block(x, y + 2);
		list.push_back(b);
		list2.push_back(bb);
	}
	//左
	if (x - 2 >= 0 && map[x - 2][y] == 1) {
		block* c = new block(x - 1, y);
		block* cc = new block(x - 2, y);
		list.push_back(c);
		list2.push_back(cc);
	}
	//右
	if (x + 2 <= BLOCK_WIDTH - 1 && map[x + 2][y] == 1) {
		block* d = new block(x + 1, y);
		block* dd = new block(x + 2, y);
		list.push_back(d);
		list2.push_back(dd);
	}
	if (list.size() != 0) {
		int BIndexea = rand() % list.size();
		block* B = list[BIndexea];
		block* BB = list2[BIndexea];
		/*将x，y与B打通*/
		map[B->x][B->y] = 0;
		/*将选中路点变为路，并加入待选列表*/
		map[BB->x][BB->y] = 0;
		openlist.push_back(BB);

	}
}
bool check(int x, int y) {
	bool temp = 0;
	//上
	if (y - 2 >= 0 && map[x][y - 2] == 1) {
		temp = 1;
	}
	//下
	if (y + 2 <= BLOCK_HEIGHT - 1 && map[x][y + 2] == 1) {
		temp = 1;
	}
	//左
	if (x - 2 >= 0 && map[x - 2][y] == 1) {
		temp = 1;
	}
	//右
	if (x + 2 <= BLOCK_WIDTH - 1 && map[x + 2][y] == 1) {
		temp = 1;
	}
	return temp;
}
void display() {
	for (int y = 0; y < BLOCK_HEIGHT; y++) {
		for (int x = 0; x < BLOCK_WIDTH; x++) {
			if (map[x][y] == 1) {
				gamedrawpixal(x, y, qiangbi);
			}
			else if (map[x][y] == 0) {
				gamedrawpixal(x, y, kongqi);
			}
		}
	}
	FlushBatchDraw();
}
void clearscreen(COLORREF RGB) {
	for (int y = 0; y < BLOCK_HEIGHT; y++) {
		for (int x = 0; x < BLOCK_WIDTH; x++) {
			gamedrawpixal(x, y, RGB);
		}
	}
}
void gamedrawpixal(int x, int y, COLORREF RGB) {
	setfillcolor(RGB); setlinecolor(COLORREF RGB(0, 0, 0));
	fillrectangle(x * BLOCK_XY, y * BLOCK_XY, BLOCK_XY + x * BLOCK_XY, BLOCK_XY + y * BLOCK_XY);
}
int main() {
	srand((unsigned)time(NULL));	//随机数种子
	initgraph(WIDTH, HEIGHT);//创建绘图窗口
	BeginBatchDraw();//开始批量绘图
	cleardevice();//清空显示

	init();//开始深搜
	display();

	openlist.clear();
	list.clear();
	list2.clear();
	system("pause");
}
