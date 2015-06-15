#ifndef __CHR_H__
#define __CHR_H__

#include "cocos2d.h"

USING_NS_CC;

#define CHR_WITDH 50 //文字宽度
#define CHR_EDGE 70 //文字矩形之斜边，为了不用乘以根号2消耗计算

//汉字类，由它组成词组
class Chr : public Node
{
public:
	//通过一个String创建汉字，x、y是坐标
	static Chr* createWithString(String*, int x, int y);
	bool init(String*, int x, int y);

public:
	String getString() { return m_chr; }
	Sprite* getBg() { return m_bg; }
	int getX() { return m_x; }
	int getY() { return m_y; }

	void setX(int x) { m_x = x; }
	void setY(int y) { m_y = y; }

	Sprite** getArrow() { return m_arrow; }
	void showArrow(Chr* next_chr); //根据下一个汉字元素，显示箭头
	void hideArrow(); //隐藏所有的箭头

private:
	//x,y代表坐标
	int m_x;
	int m_y;

	String m_chr;//汉字内容
	Sprite* m_bg;//汉字背景图
	Sprite* m_arrow[8];//箭头标志，0上，1下，2左，3右，4右上，5右下，6左上，7左下
};

#endif
