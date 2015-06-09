#include "GameScene.h"

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

	auto sp = Sprite::create("HelloWorld.png");
	sp->setPosition(winSize.width / 2, winSize.height / 2);
	addChild(sp);

	return true;
}
