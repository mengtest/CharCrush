#ifndef __LEVELSCENE_H__
#define __LEVELSCENE_H__

#include "cocos2d.h"

USING_NS_CC;

//��Ϊ�ؿ�ѡ�񳡾���
class LevelScene : public Layer
{
public:
	CREATE_FUNC(LevelScene);
	static Scene* createScene();
	bool init();

private:
	void onLevelChooseCallBack(Ref* pSender);//ѡ��ؿ���Ļص�����

private:
	ValueVector m_levels_info; //������еĹؿ�������Ϣ
};

#endif

