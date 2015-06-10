#ifndef __LETTERSGRID_H__
#define __LETTERSGRID_H__

#include "cocos2d.h"
#include <vector>

USING_NS_CC;
using namespace std;

#define GRID_WIDTH 40


class Chr;

class ChrsGrid : public Node
{
public:
	//根据letter.plist文件（单词集合），创建字集合，以及行、列
	static ChrsGrid* create(const char* letterlist, int row, int col);
	bool init(const char* letterslist, int row, int col);

private:
	bool isChrExist(Vector<String*>*, String*);//判断字是否在字集合中
	void initChrBox();//初始化汉字集合

	Chr* createAChr(int x, int y); //根据布局坐标创建一个汉字元素

private:
	bool onTouchBegan(Touch*, Event*);

private:
	int m_row;//行
	int m_col;//列

private:
	Vector<String*> m_Chrs;//汉字集合
	ValueVector m_Letters;//单词集合

	vector<vector<Chr*>> m_ChrsBox;//存放当前布局中的汉字元素
};

#endif

