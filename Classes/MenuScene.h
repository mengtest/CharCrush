#ifndef __MENUSCENE_H__
#define __MENUSCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class MenuScene : public Layer
{
public:
	static Scene* createScene(ValueVector levels_info);
	static MenuScene* create(ValueVector levels_info);
	bool init(ValueVector levels_info);

private:
	void onLevelItemCallBack(Ref* pSender);

private:
	ValueVector m_levels_info; //所有关卡信息
};

#endif

