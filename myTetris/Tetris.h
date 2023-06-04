#pragma once
#include <vector>
#include <graphics.h>
#include "Block.h"
class Tetris
{
public:
	//���캯��
	Tetris(int rows, int cols, int left, int top, int blockSize);
	void init(); //��ʼ��
	void play(); //��ʼ��Ϸ

private:
	void keyEvent();
	void updateWindow();

	 //���ؾ�����һ�ε��øú���������ɶ��ٺ���
	//��һ�ε��÷���0
	int getDelay();
	void drop();
	void moveLeftRight(int offset);
	void clearLine();
	void rotate(); //��ת
	void drawScore(); //���Ʒ���

private:
	int DELAY;
	bool update; //��Ⱦ�����־λ
	std::vector<std::vector<int>> map;
	int rows;
	int cols;
	int leftMargin;
	int topMargin;
	int blockSize;
	IMAGE imgBg;

	Block* curBlock;
	Block* nextBlock; //Ԥ�淽��
	Block bakBlock; //���ݷ��飬��ǰ���齵������м�¼��һ������λ��
	
	int score; //��¼�÷�
	const int points[4] = { 10, 30, 50, 100 };
};

