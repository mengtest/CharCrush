#ifndef __LEVELSCENE_H__
#define __LEVELSCENE_H__

#include "cocos2d.h"

USING_NS_CC;

//��Ϊ�ؿ�ѡ�񳡾���
class LevelScene : public Layer
{
public:
	static Scene* createScene(ValueVector levels_info);
	static LevelScene* create(ValueVector levels_info);
	bool init(ValueVector levels_info);

private:
	void onLevelChooseCallBack(Ref* pSender);	//ѡ��ؿ���Ļص�����

	void onBackCallBack(Ref* pSender);   //������Ϸ������

	void setLevelOpened(Menu *level_menu);		//���ùؿ��Ƿ񿪷�

private:
	ValueVector m_levels_info; //������еĹؿ�������Ϣ
};

#endif

