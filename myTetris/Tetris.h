#pragma once
#include <vector>
#include <graphics.h>
#include "Block.h"
class Tetris
{
public:
	//构造函数
	Tetris(int rows, int cols, int left, int top, int blockSize);
	void init(); //初始化
	void play(); //开始游戏

private:
	void keyEvent();
	void updateWindow();

	 //返回距离上一次调用该函数，间隔可多少毫秒
	//第一次调用返回0
	int getDelay();
	void drop();
	void moveLeftRight(int offset);
	void clearLine();
	void rotate(); //旋转
	void drawScore(); //绘制分数

private:
	int DELAY;
	bool update; //渲染界面标志位
	std::vector<std::vector<int>> map;
	int rows;
	int cols;
	int leftMargin;
	int topMargin;
	int blockSize;
	IMAGE imgBg;

	Block* curBlock;
	Block* nextBlock; //预告方块
	Block bakBlock; //备份方块，当前方块降落过程中记录上一个方块位置
	
	int score; //记录得分
	const int points[4] = { 10, 30, 50, 100 };
};

