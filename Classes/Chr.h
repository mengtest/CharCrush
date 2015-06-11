#ifndef __CHR_H__
#define __CHR_H__

#include "cocos2d.h"

USING_NS_CC;

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

private:
	//x,y代表坐标
	int m_x;
	int m_y;

	String m_chr;//汉字内容
	Sprite* m_bg;//汉字背景图
};

#endif
