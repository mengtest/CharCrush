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
	void onTextureLoading(float dt); //�����������Ƿ����

	int m_texture_num; //��ǰ���ص�������
	ValueVector m_levels_info; //���йؿ�������Ϣ
};

#endif

