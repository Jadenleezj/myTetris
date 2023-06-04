#include "Tetris.h"
#include <stdlib.h>
#include <time.h>
#include "Block.h"
#include <iostream>
#include <conio.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib") //���ӿ�

const int SPEED_NORMAL = 500; //ms
const int SPEED_QUICK = 50;


Tetris::Tetris(int rows, int cols, int left, int top, int blockSize):rows(rows), cols(cols),
																	leftMargin(left), topMargin(top), 
																	blockSize(blockSize)
{
	//��ά�����ʼ��
	//0����ʾû���κη���
	//5: ��ʾ��5�ֶ���˹����
	for (int i = 0; i < rows; i++) {
		std::vector<int> mapRow(cols, 0);
		map.push_back(mapRow);
	}
}

void Tetris::init()
{
	DELAY = SPEED_NORMAL;

	//�����������
	srand(time(NULL));

	//������Ϸ����
	initgraph(938, 896);

	//���ر���ͼƬ
	loadimage(&imgBg, "res/bg2.png");
	 
	//���³�ʼ����Ϸ������
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			map[i][j] = 0;
		}
	}

	score = 0; //�÷ֳ�ʼ��Ϊ0
}

void Tetris::play()
{
	init(); //��Ϸ��ʼ��

	nextBlock = new Block; //������Ԥ�淽��
	curBlock = nextBlock;
	nextBlock = new Block;

	int timer = 0; //��Ⱦʱ����
	while (1) {
		//�����û�����
		keyEvent();

		timer += getDelay();
		if (timer > DELAY) {
			timer = 0;
			drop(); //�����½�

			//��Ⱦ��Ϸ����
			update = true;
		}

		if (update) {
			update = false;

			//������Ϸ����
			updateWindow();

			//������Ϸ����
			clearLine();
		}
	}
}

void Tetris::keyEvent()
{
	unsigned char ch; //0~255
	bool rotateFlag = false;
	int dx = 0;//����ƫ����

	//�ж��Ƿ��а�������
	if (_kbhit()) {
		ch = _getch(); //��ȡ���룬����Ҫ���س�
		//std::cout << ch << std::endl;
		//���·���������Զ����������ַ�
		//�������Ϸ�������Ⱥ󷵻أ�224��72
		//���£�224��80
		//����224��75
		//���ң�224��77

		if (ch == 224) {
			ch = _getch();
			switch (ch) {
			case 72:
				//��ת����
				rotateFlag = true;
				break;
			case 80:
				//�����½�
				DELAY = SPEED_QUICK;
				break;
			case 75:
				//����
				dx = -1;
				break;
			case 77:
				//����
				dx = 1;
				break;
			default:
				break;
			}
		}
		if (rotateFlag) {
			//��ת
			rotate();
			//rotateFlag = false;
			update = true;
		}

		if (dx != 0) {
			//�����ƶ�
			moveLeftRight(dx);
			update = true;
		}
	}
	
}

void Tetris::updateWindow()
{
	putimage(0, 0, &imgBg); //���Ʊ���ͼƬ

	IMAGE** imgs = Block::getImages();
	BeginBatchDraw(); //��ͼ���ڴ��н��У�����ÿ�λ�ͼ��ˢ�� 

	//��Ⱦ���ײ��Ĺ̻��ķ���
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

	drawScore();//���Ʒ���

	EndBatchDraw(); //��֮ǰ���ڴ��л�ͼ�������һ��չʾ
}

//��һ�ε���ֱ�ӷ���0��֮�󷵻ؾ�����һ�ε��ü������ms
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
		//�ѷ���̻�
		bakBlock.solidify(map);
		delete curBlock;
		curBlock = nextBlock;
		nextBlock = new Block;
	}

	//���ڼ����½�֮��Ļ�ԭ
	DELAY = SPEED_NORMAL;
}

void Tetris::moveLeftRight(int offset)
{	
	bakBlock = *curBlock; //���ݺϷ�����
	curBlock->moveLeftRight(offset);
	if (!curBlock->blockInMap(map)) {
		*curBlock = bakBlock;
	}
}

void Tetris::clearLine()
{
	int lines = 0;  //����������
	int k = rows - 1; //�洢���ݵ�����
	for (int i = rows - 1; i >= 0; i--) {
		//����i���Ƿ���
		int count = 0;
		for (int j = 0; j < cols; j++) {
			if (map[i][j]) count++;
			map[k][j] = map[i][j]; 
		}
		//���ĳһ�������˻ᱻ�����и���
		//������
		if (count < cols) {
			k--;
		}
		else lines++;
	}

	if (lines > 0) {
		//�÷�
		score += points[lines - 1];
		 
		//������Ч
		mciSendString("play res/xiaochu1.mp3", 0, 0, 0);
		update = true;
	}
}

void Tetris::rotate()
{
	//��������ת������
	if (curBlock->getBlcokType() == 7) return;

	bakBlock = *curBlock; //����
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
	LOGFONT f;//��������
	gettextstyle(&f);
	f.lfHeight = 60;
	f.lfWidth = 30;
	f.lfQuality = ANTIALIASED_QUALITY; //����Բ��
	strcpy_s(f.lfFaceName, sizeof(f.lfFaceName), _T("Segoe UI Black"));
	settextstyle(&f);
	setbkmode(TRANSPARENT); //���屳������͸��
	outtextxy(670, 727, scoreText);
}
