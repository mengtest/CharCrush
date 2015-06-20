#ifndef __LEVELSCENE_H__
#define __LEVELSCENE_H__

#include "cocos2d.h"

USING_NS_CC;

//此为关卡选择场景类
class LevelScene : public Layer
{
public:
	static Scene* createScene(ValueVector levels_info);
	static LevelScene* create(ValueVector levels_info);
	bool init(ValueVector levels_info);

private:
	void onLevelChooseCallBack(Ref* pSender);	//选择关卡后的回调函数

	void onBackCallBack(Ref* pSender);   //返回游戏主界面

	void setLevelOpened(Menu *level_menu);		//设置关卡是否开放

private:
	ValueVector m_levels_info; //存放所有的关卡配置信息
};

#endif

