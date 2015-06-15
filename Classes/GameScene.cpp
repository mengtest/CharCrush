#include "GameScene.h"
#include "ChrsGrid.h"

Scene* GameScene::createScene(ValueMap level_info)
{
	auto scene = Scene::create();
	auto layer = GameScene::create(level_info);
	scene->addChild(layer);

	return scene;
}

GameScene* GameScene::create(ValueMap level_info)
{
	auto gamescene = new GameScene();

	if (gamescene && gamescene->init(level_info))
	{
		gamescene->autorelease();
		return gamescene;
	}
	else
	{
		CC_SAFE_DELETE(gamescene);
		return nullptr;
	}	
}

bool GameScene::init(ValueMap level_info)
{
	Layer::init();

	auto winSize = Director::getInstance()->getWinSize();

	//游戏背景
	auto bg = Sprite::create("bg.png");
	bg->setAnchorPoint(Vec2(0 ,0));
	bg->setPosition(0, 0);
	addChild(bg);

	//顶部关卡bar
	auto bar = Sprite::create("level_bar.png");
	bar->setScale(0.5);
	bar->setAnchorPoint(Vec2(0.5, 1));
	bar->setPosition(winSize.width / 2, winSize.height);
	addChild(bar);
	
	//根据行列以及单词列表文件，创建一个汉字阵列
	int col = 7;
	int row = 8;
	auto chrsgrid = ChrsGrid::create(level_info, col, row);
	addChild(chrsgrid);
	chrsgrid->setPosition(winSize.width / 2 + 10, winSize.height / 2 + 50);

	//测试按钮，返回关卡选择界面
	auto back = MenuItemFont::create("返回关卡选择界面", this, menu_selector(GameScene::onBackCallBack));
	back->setColor(Color3B::BLACK);
	auto menu = Menu::create(back, nullptr);
	menu->setPosition(winSize.width / 2, 100);
	addChild(menu);
	
	return true;
}

void GameScene::onBackCallBack(Ref* pSender)
{
	Director::getInstance()->popScene();
}