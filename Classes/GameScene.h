#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

#define SCORE_PER_CHR 30 //��ʱ���壬һ����30��

//��Ϸ��������
class GameScene : public Layer
{
public:
	//��������������Ϊ���ؿ���������Ϣ
	static Scene* createScene(ValueMap level_info);
	static GameScene* create(ValueMap level_info);
	bool init(ValueMap level_info);

public:
	void subStep();				//��ȥһ��������ʾ
	void addScore(int score);   //�ӷֲ���ʾ
	int getStep() { return m_step; } //���ص�ǰ�Ĳ���

	//����letter Label�����ݣ������ӷ�ѡ��ڶ���������ʾ�Ƿ������������Ƿ��ܼӷ�
	void setLetterLabel(string letter, bool isCorrect);

private:
	//�����ã����ô˺��������عؿ���ѡ�����
	void onBackCallBack(Ref* pSender) { Director::getInstance()->popScene(); }

private:
	int m_step;			//��Ϸ�ؿ������Ʋ���
	int m_score;		//��Ϸ��ǰ����
	int m_score_start1;	//ͨ������ķ�������Ϊ1�ǣ�2�ǣ�3��
	int m_score_start2;
	int m_score_start3;
};

#endif

