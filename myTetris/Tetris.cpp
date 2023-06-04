#include "Tetris.h"
#include <stdlib.h>
#include <time.h>
#include "Block.h"
#include <iostream>
#include <conio.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib") //链接库

const int SPEED_NORMAL = 500; //ms
const int SPEED_QUICK = 50;


Tetris::Tetris(int rows, int cols, int left, int top, int blockSize):rows(rows), cols(cols),
																	leftMargin(left), topMargin(top), 
																	blockSize(blockSize)
{
	//二维数组初始化
	//0：表示没有任何方块
	//5: 表示第5种俄罗斯方块
	for (int i = 0; i < rows; i++) {
		std::vector<int> mapRow(cols, 0);
		map.push_back(mapRow);
	}
}

void Tetris::init()
{
	DELAY = SPEED_NORMAL;

	//配置随机种子
	srand(time(NULL));

	//创建游戏窗口
	initgraph(938, 896);

	//加载背景图片
	loadimage(&imgBg, "res/bg2.png");
	 
	//重新初始化游戏区数据
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			map[i][j] = 0;
		}
	}

	score = 0; //得分初始化为0
}

void Tetris::play()
{
	init(); //游戏初始化

	nextBlock = new Block; //先生成预告方块
	curBlock = nextBlock;
	nextBlock = new Block;

	int timer = 0; //渲染时间间隔
	while (1) {
		//接受用户输入
		keyEvent();

		timer += getDelay();
		if (timer > DELAY) {
			timer = 0;
			drop(); //方块下降

			//渲染游戏画面
			update = true;
		}

		if (update) {
			update = false;

			//更新游戏画面
			updateWindow();

			//更新游戏数据
			clearLine();
		}
	}
}

void Tetris::keyEvent()
{
	unsigned char ch; //0~255
	bool rotateFlag = false;
	int dx = 0;//左右偏移量

	//判断是否有按键输入
	if (_kbhit()) {
		ch = _getch(); //获取输入，不需要按回车
		//std::cout << ch << std::endl;
		//按下方向键，会自动返回两个字符
		//按下向上方向键，先后返回：224，72
		//向下：224，80
		//向左：224，75
		//向右：224，77

		if (ch == 224) {
			ch = _getch();
			switch (ch) {
			case 72:
				//旋转变形
				rotateFlag = true;
				break;
			case 80:
				//快速下降
				DELAY = SPEED_QUICK;
				break;
			case 75:
				//向左
				dx = -1;
				break;
			case 77:
				//向右
				dx = 1;
				break;
			default:
				break;
			}
		}
		if (rotateFlag) {
			//旋转
			rotate();
			//rotateFlag = false;
			update = true;
		}

		if (dx != 0) {
			//左右移动
			moveLeftRight(dx);
			update = true;
		}
	}
	
}

void Tetris::updateWindow()
{
	putimage(0, 0, &imgBg); //绘制背景图片

	IMAGE** imgs = Block::getImages();
	BeginBatchDraw(); //绘图在内存中进行，不必每次绘图都刷新 

	//渲染到底部的固化的方块
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (map[i][j] == 0) continue;
			int x = j * blockSize + leftMargin;
			int y = i * blockSize + topMargin;
			putimage(x, y, imgs[map[i][j] - 1]);
		}
	}

	curBlock->draw(leftMargin, topMargin);
	nextBlock->draw(689, 150);

	drawScore();//绘制分数

	EndBatchDraw(); //将之前在内存中绘图的最后结果一次展示
}

//第一次调用直接返回0，之后返回距离上一次调用间隔多少ms
int Tetris::getDelay()
{
	static unsigned long long lastTime = 0;
	unsigned long long  currentTime = GetTickCount();
	if (lastTime == 0) {
		lastTime = currentTime;
		return 0;
	}
	else {
		int ret = currentTime - lastTime;
		lastTime = currentTime;
		return ret;
	}
}

void Tetris::drop()
{
	bakBlock = *curBlock;
	curBlock->drop();

	if (!curBlock->blockInMap(map)) {
		//把方块固化
		bakBlock.solidify(map);
		delete curBlock;
		curBlock = nextBlock;
		nextBlock = new Block;
	}

	//用于加速下降之后的还原
	DELAY = SPEED_NORMAL;
}

void Tetris::moveLeftRight(int offset)
{	
	bakBlock = *curBlock; //备份合法方块
	curBlock->moveLeftRight(offset);
	if (!curBlock->blockInMap(map)) {
		*curBlock = bakBlock;
	}
}

void Tetris::clearLine()
{
	int lines = 0;  //消掉的行数
	int k = rows - 1; //存储数据的行数
	for (int i = rows - 1; i >= 0; i--) {
		//检查第i行是否满
		int count = 0;
		for (int j = 0; j < cols; j++) {
			if (map[i][j]) count++;
			map[k][j] = map[i][j]; 
		}
		//如果某一行满行了会被不满行覆盖
		//不满行
		if (count < cols) {
			k--;
		}
		else lines++;
	}

	if (lines > 0) {
		//得分
		score += points[lines - 1];
		 
		//播放音效
		mciSendString("play res/xiaochu1.mp3", 0, 0, 0);
		update = true;
	}
}

void Tetris::rotate()
{
	//田字形旋转无意义
	if (curBlock->getBlcokType() == 7) return;

	bakBlock = *curBlock; //备份
	curBlock->rotate();
	if (!curBlock->blockInMap(map)) {
		*curBlock = bakBlock;
	}
}

void Tetris::drawScore()
{
	char scoreText[32];
	sprintf_s(scoreText, sizeof(scoreText), "%d", score);

	setcolor(RGB(180, 180, 180));
	LOGFONT f;//设置字体
	gettextstyle(&f);
	f.lfHeight = 60;
	f.lfWidth = 30;
	f.lfQuality = ANTIALIASED_QUALITY; //字体圆滑
	strcpy_s(f.lfFaceName, sizeof(f.lfFaceName), _T("Segoe UI Black"));
	settextstyle(&f);
	setbkmode(TRANSPARENT); //字体背景设置透明
	outtextxy(670, 727, scoreText);
}
