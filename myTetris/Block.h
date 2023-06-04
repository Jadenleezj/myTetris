#pragma once
#include <graphics.h>
#include <vector>
struct Point {
	int row;
	int col;
};


class Block
{
public:
	Block();
	void drop();
	void moveLeftRight(int offset);
	void rotate(); //��ת
	void draw(int leftMargin, int topMargin); 
	static IMAGE** getImages();
	Block& operator=(const Block& other);
	bool blockInMap(const std::vector<std::vector<int>> &map);
	void solidify(std::vector<std::vector<int>>& map); //����̻�
	int getBlcokType();
private:
	int blockType; //��������
	Point smallBlocks[4]; //�洢�ĸ�С����ľ�������
	IMAGE *img;

	static IMAGE* imgs[7]; //�������鹲�е�����ͼƬ,ֻ��Ҫ��ʼ��һ��
	static int size; //ͼƬ��С
};

