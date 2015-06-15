#include "LevelScene.h"
#include "GameScene.h"

Scene* LevelScene::createScene()
{
	auto scene = Scene::create();
	auto layer = LevelScene::create();
	scene->addChild(layer);
	return scene;
}

bool LevelScene::init()
{
	Layer::init();

	//加载texture
	TextureCache::getInstance()->addImage("char_bg_selected.png");
	TextureCache::getInstance()->addImage("char_bg_normal.png");
	TextureCache::getInstance()->addImage("char_bg_crush.png");
	TextureCache::getInstance()->addImage("arrow.png");
	TextureCache::getInstance()->addImage("grid_bg.png");
	TextureCache::getInstance()->addImage("bg.png");
	TextureCache::getInstance()->addImage("level_bar.png");

	//打开关卡配置文件,levels存放了所有的关卡信息
	auto sharedFile = FileUtils::getInstance();
	m_levels_info = sharedFile->getValueVectorFromFile("level_config.plist");

	//加入关卡选择按钮，此为临时设置
	auto level1_choose = MenuItemFont::create("level 1", this, menu_selector(LevelScene::onLevelChooseCallBack));
	auto level2_choose = MenuItemFont::create("level 2", this, menu_selector(LevelScene::onLevelChooseCallBack));
	auto level3_choose = MenuItemFont::create("level 3", this, menu_selector(LevelScene::onLevelChooseCallBack));

	//tag用于标记关卡，比如100代表第一关
	level1_choose->setTag(100);
	level2_choose->setTag(101);
	level3_choose->setTag(102);

	auto menu = Menu::create(level1_choose, level2_choose, level3_choose, nullptr);
	addChild(menu);
	menu->alignItemsVerticallyWithPadding(20);

	return true;
}

void LevelScene::onLevelChooseCallBack(Ref* level_item)
{
	//根据level_item的标签定位对应关卡的信息
	int level = ((MenuItemFont*)level_item)->getTag() - 100;//得到第几关
	auto level_info = m_levels_info.at(level).asValueMap();//得到关卡信息

	auto scene = GameScene::createScene(level_info);
	Director::getInstance()->pushScene(scene);
}