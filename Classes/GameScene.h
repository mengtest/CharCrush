#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

#define SCORE_PER_CHR 30 //临时定义，一个字30分

class GameScene : public Layer
{
public:
	static Scene* createScene(ValueMap level_info);
	static GameScene* create(ValueMap level_info);//传入本关卡配置信息
	bool init(ValueMap level_info);

private:
	void onBackCallBack(Ref* pSender);

public:
	void subStep();
	void addScore(int score); //加分并显示
	int getStep() { return m_step; }

	//更改letter Label的内容，包括加分选项，第二个参数表示是否能消除，即是否能加分
	void setLetterLabel(string letter, bool isCorrect);

private:
	int m_step; //游戏关卡的限制步数
	int m_score; //存储当前分数
	int m_score_start1;//通关所需的分数，分为1星，2星，3星
	int m_score_start2;
	int m_score_start3;
};

#endif

