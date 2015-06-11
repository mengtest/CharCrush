#ifndef __LETTERSGRID_H__
#define __LETTERSGRID_H__

#include "cocos2d.h"
#include <vector>

USING_NS_CC;
using namespace std;

#define GRID_WIDTH 60

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

	bool canCrush(); //判断当前已选汉字能否消除
	void clearSelectedChr(Chr*); //清空已选汉字
	void dropChrs(); //汉字掉落

private:
	bool onTouchBegan(Touch*, Event*);
	void onTouchMoved(Touch*, Event*);
	void onTouchEnded(Touch*, Event*);

	void onChrsDropping(float dt);//汉字掉落状态捕捉函数

private:
	int m_row;//行
	int m_col;//列

	bool m_canCrush;//标记当前已选汉字能否消除

private:
	Vector<String*> m_Chrs;//汉字集合
	ValueVector m_Letters;//单词集合

	vector<vector<Chr*>> m_ChrsBox;//汉字盒子：存放当前布局中的汉字元素
	Vector<Chr*> m_SelectedChrs;//存放已选择的汉字集合
	Vector<Chr*> m_NewChrs; //存放欲加入汉字阵列的汉字元素
};

#endif

