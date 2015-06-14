#include "GameScene.h"
#include "ChrsGrid.h"
#include "ui/CocosGUI.h"

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
	auto bg = Sprite::create("bg.jpg");
	bg->setAnchorPoint(Vec2(0 ,0));
	bg->setPosition(0, 0);
	addChild(bg);

	//棋盘底图
	auto gridbg = ui::Scale9Sprite::create("grid_bg.png");
	gridbg->setAnchorPoint(Vec2(0, 0));
	gridbg->setContentSize(Size(6 * GRID_WIDTH, 6* GRID_WIDTH));
	gridbg->setPosition(0, 270);
	addChild(gridbg);

	//根据行列以及单词列表文件，创建一个汉字阵列
	auto chrsgrid = ChrsGrid::create(level_info, 6, 6);
	addChild(chrsgrid);
	chrsgrid->setPosition(10, 280);

	//测试按钮，返回关卡选择界面
	auto back = MenuItemFont::create("返回关卡选择界面", this, menu_selector(GameScene::onBackCallBack));
	back->setColor(Color3B::BLACK);
	auto menu = Menu::create(back, nullptr);
	menu->setPosition(180, 200);
	addChild(menu);
	
	return true;
}

void GameScene::onBackCallBack(Ref* pSender)
{
	Director::getInstance()->popScene();
}