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
	//����letter.plist�ļ������ʼ��ϣ��������ּ��ϣ��Լ��С���
	static ChrsGrid* create(const char* letterlist, int row, int col);
	bool init(const char* letterslist, int row, int col);

private:
	bool isChrExist(Vector<String*>*, String*);//�ж����Ƿ����ּ�����
	void initChrBox();//��ʼ�����ּ���

	Chr* createAChr(int x, int y); //���ݲ������괴��һ������Ԫ��

	bool canCrush(); //�жϵ�ǰ��ѡ�����ܷ�����
	void clearSelectedChr(Chr*); //�����ѡ����
	void dropChrs(); //���ֵ���

private:
	bool onTouchBegan(Touch*, Event*);
	void onTouchMoved(Touch*, Event*);
	void onTouchEnded(Touch*, Event*);

	void onChrsDropping(float dt);//���ֵ���״̬��׽����

private:
	int m_row;//��
	int m_col;//��

	bool m_canCrush;//��ǵ�ǰ��ѡ�����ܷ�����

private:
	Vector<String*> m_Chrs;//���ּ���
	ValueVector m_Letters;//���ʼ���

	vector<vector<Chr*>> m_ChrsBox;//���ֺ��ӣ���ŵ�ǰ�����еĺ���Ԫ��
	Vector<Chr*> m_SelectedChrs;//�����ѡ��ĺ��ּ���
	Vector<Chr*> m_NewChrs; //��������뺺�����еĺ���Ԫ��
};

#endif

