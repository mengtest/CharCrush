#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

#define SCORE_PER_CHR 25 //临时定义，一个字的分值

class ChrsGrid;

//游戏主场景类
class GameScene : public Layer
{
public:
	//工厂函数，参数为本关卡的配置信息
	static Scene* createScene(ValueMap level_info);
	static GameScene* create(ValueMap level_info);
	bool init(ValueMap level_info);

private:
	//初始化本场景相关函数

public:
	int subStep();				//减去一步，并显示
	int addScore(int score);   //加分并显示

	int getStepNeed() { return m_step_need; }
	int getScoreNeed() { return m_score_need; }
	void gameover(bool isWin);

	//更改letter Label的内容，包括加分选项，第二个参数表示是否能消除，即是否能加分
	void setLetterLabel(string letter, bool isCorrect);

private:
	//测试用，调用此函数将返回关卡的选择界面
	void onBackCallBack(Ref* pSender);

	void goingBackToLevelScene(float dt);

private:
	int m_level_id;		//关卡ID，即关卡的序号
	int m_step;			//游戏关卡的限制步数
	int m_step_need;
	int m_score;		//游戏当前分数
	int m_score_need;   //通关所需分数

	int m_aim; //关卡目标ID

	int m_countdown; //返回关卡选择界面倒计时

	Sprite *m_end_bar;

	ChrsGrid *m_chrsgrid; //布局阵列
};

#endif

