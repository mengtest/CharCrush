#ifndef __CHR_H__
#define __CHR_H__

#include "cocos2d.h"

USING_NS_CC;

#define CHR_WITDH 40 //���ֿ��
#define CHR_EDGE 56 //���־���֮б�ߣ�Ϊ�˲��ó��Ը���2���ļ���

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

	void setX(int x) { m_x = x; }
	void setY(int y) { m_y = y; }

private:
	//x,y��������
	int m_x;
	int m_y;

	String m_chr;//��������
	Sprite* m_bg;//���ֱ���ͼ
};

#endif
