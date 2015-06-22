#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

#define SCORE_PER_CHR 25 //��ʱ���壬һ���ֵķ�ֵ

class ChrsGrid;

//��Ϸ��������
class GameScene : public Layer
{
public:
	//��������������Ϊ���ؿ���������Ϣ
	static Scene* createScene(ValueMap level_info);
	static GameScene* create(ValueMap level_info);
	bool init(ValueMap level_info);

private:
	//��ʼ����������غ���

public:
	int subStep();				//��ȥһ��������ʾ
	int addScore(int score);   //�ӷֲ���ʾ

	int getStepNeed() { return m_step_need; }
	int getScoreNeed() { return m_score_need; }
	void gameover(bool isWin);

	//����letter Label�����ݣ������ӷ�ѡ��ڶ���������ʾ�Ƿ������������Ƿ��ܼӷ�
	void setLetterLabel(string letter, bool isCorrect);

private:
	//�����ã����ô˺��������عؿ���ѡ�����
	void onBackCallBack(Ref* pSender);

	void goingBackToLevelScene(float dt);

private:
	int m_level_id;		//�ؿ�ID�����ؿ������
	int m_step;			//��Ϸ�ؿ������Ʋ���
	int m_step_need;
	int m_score;		//��Ϸ��ǰ����
	int m_score_need;   //ͨ���������

	int m_aim; //�ؿ�Ŀ��ID

	int m_countdown; //���عؿ�ѡ����浹��ʱ

	Sprite *m_end_bar;

	ChrsGrid *m_chrsgrid; //��������
};

#endif

