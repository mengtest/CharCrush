#include "MenuScene.h"
#include "LevelScene.h"

Scene* MenuScene::createScene(ValueVector levels_info)
{
	auto scene = Scene::create();
	auto layer = MenuScene::create(levels_info);
	scene->addChild(layer);

	return scene;
}

MenuScene* MenuScene::create(ValueVector levels_info)
{
	auto menuscene = new MenuScene();

	if (menuscene && menuscene->init(levels_info))
	{
		menuscene->autorelease();
		return menuscene;
	}
	else
	{
		CC_SAFE_DELETE(menuscene);
		return nullptr;
	}	
}

bool MenuScene::init(ValueVector levels_info)
{
	Layer::init();

	m_levels_info = levels_info;

	auto winSize = Director::getInstance()->getWinSize();

	//背景图片
	auto bg = Sprite::create("MenuScene/menu_bg.jpg");
	bg->setPosition(winSize.width / 2, winSize.height / 2);
	addChild(bg);

	//冒险模式按钮
	auto adventure_item = MenuItemImage::create("MenuScene/adventure_mode.png", "MenuScene/adventure_mode.png");
	adventure_item->setPositionX(-100);
	adventure_item->setPositionY(150);

	//闯关模式按钮
	auto level_item = MenuItemImage::create("MenuScene/level_mode.png", "MenuScene/level_mode_selected.png", 
		this, menu_selector(MenuScene::onLevelItemCallBack));
	level_item->setPositionX(100);
	level_item->setPositionY(150);

	auto menu = Menu::create(adventure_item, level_item, nullptr);
	addChild(menu);

	return true;
}

void MenuScene::onLevelItemCallBack(Ref* pSender)
{
	auto scene = LevelScene::createScene(m_levels_info);
	Director::getInstance()->replaceScene(scene);
}