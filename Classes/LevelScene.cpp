#include "LevelScene.h"
#include "GameScene.h"
#include "MenuScene.h"

Scene* LevelScene::createScene(ValueVector levels_info)
{
	auto scene = Scene::create();
	auto layer = LevelScene::create(levels_info);
	scene->addChild(layer);

	return scene;
}

LevelScene* LevelScene::create(ValueVector levels_info)
{
	auto levelscene = new LevelScene();

	if (levelscene && levelscene->init(levels_info))
	{
		levelscene->autorelease();
		return levelscene;
	}
	else
	{
		CC_SAFE_DELETE(levelscene);
		return nullptr;
	}	
}

bool LevelScene::init(ValueVector levels_info)
{
	Layer::init();

	auto winSize = Director::getInstance()->getWinSize();

	m_levels_info = levels_info;

	//添加关卡背景
	auto bg = Sprite::create("LevelScene/level_bg.jpg");
	bg->setPosition(winSize.width / 2, winSize.height / 2);
	addChild(bg);

	//加入关卡选择菜单，此为临时设置，日后关卡界面需设计漂亮些
	auto level1_choose = MenuItemImage::create("LevelScene/leel_locked.png", "LevelScene/leel_locked.png", 
		this, menu_selector(LevelScene::onLevelChooseCallBack));
	auto level2_choose = MenuItemImage::create("LevelScene/leel_locked.png", "LevelScene/leel_locked.png", 
		this, menu_selector(LevelScene::onLevelChooseCallBack));
	auto level3_choose = MenuItemImage::create("LevelScene/leel_locked.png", "LevelScene/leel_locked.png", 
		this, menu_selector(LevelScene::onLevelChooseCallBack));
	auto level4_choose = MenuItemImage::create("LevelScene/leel_locked.png", "LevelScene/leel_locked.png", 
		this, menu_selector(LevelScene::onLevelChooseCallBack));
	auto level5_choose = MenuItemImage::create("LevelScene/leel_locked.png", "LevelScene/leel_locked.png", 
		this, menu_selector(LevelScene::onLevelChooseCallBack));
	auto level6_choose = MenuItemImage::create("LevelScene/leel_locked.png", "LevelScene/leel_locked.png", 
		this, menu_selector(LevelScene::onLevelChooseCallBack));
	auto level7_choose = MenuItemImage::create("LevelScene/leel_locked.png", "LevelScene/leel_locked.png", 
		this, menu_selector(LevelScene::onLevelChooseCallBack));

	//tag用于标记关卡，100代表第一关
	level1_choose->setTag(100);level1_choose->setPosition(120, 120);
	level2_choose->setTag(101);level2_choose->setPosition(320, 120);
	level3_choose->setTag(102);level3_choose->setPosition(440, 300);
	level4_choose->setTag(103);level4_choose->setPosition(320, 500);
	level5_choose->setTag(104);level5_choose->setPosition(120, 650);
	level6_choose->setTag(105);level6_choose->setPosition(320, 780);
	level7_choose->setTag(106);level7_choose->setPosition(480, 880);

	auto menu = Menu::create(level1_choose, level2_choose, level3_choose, level4_choose, level5_choose, 
		level6_choose, level7_choose, nullptr);
	addChild(menu);
	//menu->alignItemsVerticallyWithPadding(20);
	menu->setPosition(0, 0);

	setLevelOpened(menu);

	//测试按钮，返回关卡选择界面
	auto back = MenuItemFont::create("back to game menu", this, menu_selector(LevelScene::onBackCallBack));
	back->setColor(Color3B::BLACK);
	auto menu1 = Menu::create(back, nullptr);
	menu1->setPosition(winSize.width / 2, 50);
	addChild(menu1);

	return true;
}

void LevelScene::onBackCallBack(Ref* pSender)
{
	auto sharedFile = FileUtils::getInstance();
	auto levels_info = sharedFile->getValueVectorFromFile("level_config.plist");
	auto scene = MenuScene::createScene(levels_info);
	Director::getInstance()->replaceScene(scene);
}

void LevelScene::setLevelOpened(Menu *level_menu)
{
	//获取单例对象的同时，将判断文件是否存在，不存在则自动建立一个空XML文件
	auto userdefault = UserDefault::getInstance();
	
	//log(userdefault->getXMLFilePath().c_str());
	
	//从UserDefault中获取最高开放的关卡，0代表第一关
	int heighest_level = userdefault->getIntegerForKey("HeighestLevel");

	auto levels_button = level_menu->getChildren();

	for (auto &level_button : levels_button)
	{
		//小于等于最高关卡的才会开放，否则不予开放
		if (level_button->getTag()-100 <= heighest_level)
		{
			((MenuItemImage*)level_button)->setEnabled(true);
			((MenuItemImage*)level_button)->setNormalImage(Sprite::create("LevelScene/level_opened.png"));
			((MenuItemImage*)level_button)->setSelectedImage(Sprite::create("LevelScene/level_opened_selected.png"));
		}
		else
		{
			((MenuItemImage*)level_button)->setEnabled(false);
		}
	}
}

//关卡选择后，回调函数，将根据选择的关卡进入到游戏界面
void LevelScene::onLevelChooseCallBack(Ref* level_item)
{
	//根据level_item的标签定位对应关卡的信息 
	int level = ((MenuItemFont*)level_item)->getTag() - 100; //得到第几关
	auto level_info = m_levels_info.at(level).asValueMap();  //得到该关的关卡信息，此信息是字典型的

	//进入游戏界面
	auto scene = GameScene::createScene(level_info);
	Director::getInstance()->replaceScene(scene);
}