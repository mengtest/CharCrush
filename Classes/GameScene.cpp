﻿#include "GameScene.h"
#include "ChrsGrid.h"

Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);

	return scene;
}

bool GameScene::init()
{
	Layer::init();

	auto winSize = Director::getInstance()->getWinSize();

	//加载texture
	TextureCache::getInstance()->addImage("char_bg_selected.png");
	TextureCache::getInstance()->addImage("char_bg_normal.png");
	TextureCache::getInstance()->addImage("char_bg_crush.png");

	//根据行列以及单词列表文件，创建一个汉字阵列
	auto chrsgrid = ChrsGrid::create("letters_1.plist", 4, 4);
	addChild(chrsgrid);
	chrsgrid->setPosition(0 ,280);
	
	return true;
}
