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

	//加载资源模块，日后可将其分离至Loading界面
	TextureCache::getInstance()->addImage("char_bg_selected.png");
	TextureCache::getInstance()->addImage("char_bg_normal.png");
	TextureCache::getInstance()->addImage("char_bg_crush.png");
	TextureCache::getInstance()->addImage("char_bg_special.png");
	TextureCache::getInstance()->addImage("arrow.png");
	TextureCache::getInstance()->addImage("grid_bg.png");
	TextureCache::getInstance()->addImage("bg.png");
	TextureCache::getInstance()->addImage("level_bar.png");
	TextureCache::getInstance()->addImage("labelatlas.png");
	TextureCache::getInstance()->addImage("star1.png");
	TextureCache::getInstance()->addImage("star2.png");
	TextureCache::getInstance()->addImage("star3.png");
	TextureCache::getInstance()->addImage("bias.png");
	TextureCache::getInstance()->addImage("right_left.png");

	//从关卡配置文件中，读取关卡配置信息，此信息是列表型的，每一个单元存放一个关卡信息
	auto sharedFile = FileUtils::getInstance();
	m_levels_info = sharedFile->getValueVectorFromFile("level_config.plist");

	//加入关卡选择菜单，此为临时设置，日后关卡界面需设计漂亮些
	auto level1_choose = MenuItemFont::create("level 1", this, menu_selector(LevelScene::onLevelChooseCallBack));
	auto level2_choose = MenuItemFont::create("level 2", this, menu_selector(LevelScene::onLevelChooseCallBack));
	auto level3_choose = MenuItemFont::create("level 3", this, menu_selector(LevelScene::onLevelChooseCallBack));

	//tag用于标记关卡，100代表第一关
	level1_choose->setTag(100);
	level2_choose->setTag(101);
	level3_choose->setTag(102);

	auto menu = Menu::create(level1_choose, level2_choose, level3_choose, nullptr);
	addChild(menu);
	menu->alignItemsVerticallyWithPadding(20);

	return true;
}

//关卡选择后，回调函数，将根据选择的关卡进入到游戏界面
void LevelScene::onLevelChooseCallBack(Ref* level_item)
{
	//根据level_item的标签定位对应关卡的信息
	int level = ((MenuItemFont*)level_item)->getTag() - 100; //得到第几关
	auto level_info = m_levels_info.at(level).asValueMap();  //得到该关的关卡信息，此信息是字典型的

	//进入游戏界面
	auto scene = GameScene::createScene(level_info);
	Director::getInstance()->pushScene(scene);
}