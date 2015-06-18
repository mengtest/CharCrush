#ifndef __LEVELSCENE_H__
#define __LEVELSCENE_H__

#include "cocos2d.h"

USING_NS_CC;

//此为关卡选择场景类
class LevelScene : public Layer
{
public:
	CREATE_FUNC(LevelScene);
	static Scene* createScene();
	bool init();

private:
	void onLevelChooseCallBack(Ref* pSender);//选择关卡后的回调函数

private:
	ValueVector m_levels_info; //存放所有的关卡配置信息
};

#endif

