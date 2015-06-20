#ifndef LOADINGSCENE_H
#define LOADINGSCENE_H

#include "cocos2d.h"

USING_NS_CC;

#define TOTAL_PIC 23

class LoadingScene : public Layer
{
public:
	static Scene* createScene();
	CREATE_FUNC(LoadingScene);
	bool init();

private:
	void onTextureLoading(float dt); //检测纹理加载是否完毕

	int m_texture_num; //当前加载的纹理数
	ValueVector m_levels_info; //所有关卡配置信息
};

#endif

