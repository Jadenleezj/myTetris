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
	void rotate(); //旋转
	void draw(int leftMargin, int topMargin); 
	static IMAGE** getImages();
	Block& operator=(const Block& other);
	bool blockInMap(const std::vector<std::vector<int>> &map);
	void solidify(std::vector<std::vector<int>>& map); //方块固化
	int getBlcokType();
private:
	int blockType; //方块类型
	Point smallBlocks[4]; //存储四个小方块的具体坐标
	IMAGE *img;

	static IMAGE* imgs[7]; //整个方块共有的七种图片,只需要初始化一次
	static int size; //图片大小
};

