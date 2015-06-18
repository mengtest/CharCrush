#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

#define SCORE_PER_CHR 30 //临时定义，一个字30分

//游戏主场景类
class GameScene : public Layer
{
public:
	//工厂函数，参数为本关卡的配置信息
	static Scene* createScene(ValueMap level_info);
	static GameScene* create(ValueMap level_info);
	bool init(ValueMap level_info);

public:
	void subStep();				//减去一步，并显示
	void addScore(int score);   //加分并显示
	int getStep() { return m_step; } //返回当前的步数

	//更改letter Label的内容，包括加分选项，第二个参数表示是否能消除，即是否能加分
	void setLetterLabel(string letter, bool isCorrect);

private:
	//测试用，调用此函数将返回关卡的选择界面
	void onBackCallBack(Ref* pSender) { Director::getInstance()->popScene(); }

private:
	int m_step;			//游戏关卡的限制步数
	int m_score;		//游戏当前分数
	int m_score_start1;	//通关所需的分数，分为1星，2星，3星
	int m_score_start2;
	int m_score_start3;
};

#endif

