#ifndef __CHR_H__
#define __CHR_H__

#include "cocos2d.h"

USING_NS_CC;

//�����࣬������ɴ���
class Chr : public Node
{
public:
	//ͨ��һ��String�������֣�x��y������
	static Chr* createWithString(String*, int x, int y);
	bool init(String*, int x, int y);

public:
	String getString() { return m_chr; }
	Sprite* getBg() { return m_bg; }
	int getX() { return m_x; }
	int getY() { return m_y; }

private:
	//x,y��������
	int m_x;
	int m_y;

	String m_chr;//��������
	Sprite* m_bg;//���ֱ���ͼ
};

#endif
