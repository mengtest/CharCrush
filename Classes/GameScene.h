#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class GameScene : public Layer
{
public:
	static Scene* createScene(ValueMap level_info);
	static GameScene* create(ValueMap level_info);//���뱾�ؿ�������Ϣ
	bool init(ValueMap level_info);

private:
	void onBackCallBack(Ref* pSender);
};

#endif

