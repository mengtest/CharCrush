#include "GameScene.h"
#include "ChrsGrid.h"
#include "LevelScene.h"

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

	//得到关卡序号
	m_level_id = level_info.at("ID").asInt();

	//初始化本关卡目标 
	auto aim_info = level_info.at("aim").asValueMap();
	m_aim = aim_info.at("ID").asInt();
	m_score_need = aim_info.at("score").asInt();

	m_step_need = level_info.at("step").asInt();

	//添加游戏背景
	auto bg = Sprite::create("bg.png");
	bg->setPosition(winSize.width / 2, winSize.height / 2);
	addChild(bg);

	//添加顶部关卡bar
	auto bar = Sprite::create("level_bar.png");
	bar->setScale(0.5);
	bar->setAnchorPoint(Vec2(0.5, 1));
	bar->setPosition(winSize.width / 2, winSize.height);
	addChild(bar);

	//根据行列以及本关卡配置信息，创建一个汉字阵列
	int col = 7;
	int row = 8;
	m_chrsgrid = ChrsGrid::create(level_info, col, row);

	//创建一个裁剪节点，裁剪大小是汉字阵列的大小，并将汉字阵列加入此裁剪结点
	//这是为了让方块掉落时，只在位于汉字阵列内时才显示
	auto clipper = ClippingNode::create();
	clipper->setStencil(m_chrsgrid->getChildByTag(1000));
	//clipper->setStencil(Sprite::create("bg.png"));
	clipper->setPosition(winSize.width / 2, winSize.height / 2 + 50);
	clipper->addChild(m_chrsgrid);
	addChild(clipper);

	//根据关卡配置获得游戏关卡的限定步数
	m_step = level_info.at("step").asInt();

	//添加步数Label
	char buf[10] = {0};
	sprintf(buf, "%d", m_step);
	auto step_label = LabelAtlas::create(buf, "labelatlas.png", 17, 22, '0');
	step_label->setPosition(Vec2(winSize.width / 4 - 30, winSize.height - 35));
	step_label->setTag(100);
	step_label->setAnchorPoint(Vec2(0.5, 0.5));
	addChild(step_label);

	//添加letter label
	auto letter_label = Label::createWithSystemFont("", "Arial", 25);
	letter_label->setPosition(winSize.width / 2 - 20, winSize.height - letter_label->getContentSize().height / 2 - 25);
	letter_label->setTag(101);
	letter_label->setColor(Color3B::BLACK);
	addChild(letter_label);

	//添加scoreadd label
	auto scoreadd_label = LabelAtlas::create("0", "labelatlas.png", 17, 22, '0');
	scoreadd_label->setPosition(Vec2(winSize.width / 4 * 3 + 35, winSize.height - 25));
	scoreadd_label->setTag(102);
	scoreadd_label->setAnchorPoint(Vec2(0.5, 0.5));
	addChild(scoreadd_label);

	//添加当前分数Label
	m_score = 0;
	auto score_label = LabelAtlas::create("0", "labelatlas.png", 17, 22, '0');
	score_label->setPosition(Vec2(winSize.width / 2, winSize.height - 100));
	score_label->setTag(103);
	score_label->setScale(1.5);
	score_label->setAnchorPoint(Vec2(0.5, 0.5));
	//addChild(score_label);

	//关卡目标1代表获得指定分数
	if (m_aim == 1)
	{
		char buf1[100] = {0};
		sprintf(buf1, "(本关通关所需分数:%d)", m_score_need);
		auto label_score_need = Label::createWithSystemFont(buf1, "Arial", 20);
		label_score_need->setPosition(score_label->getPosition());
		label_score_need->setPositionX(label_score_need->getPositionX() + 150);
		label_score_need->setColor(Color3B::BLACK);
		addChild(label_score_need);
	}

	//显示三颗星星，代表通关星级
	auto star1 = Sprite::create("star1.png");star1->setTag(200);star1->setScale(0.5);star1->setVisible(false);
	auto star2 = Sprite::create("star2.png");star2->setTag(201);star2->setScale(0.5);star2->setVisible(false);
	auto star3 = Sprite::create("star3.png");star3->setTag(202);star3->setScale(0.5);star3->setVisible(false);
	star1->setPosition(winSize.width / 4 - 65, winSize.height - 65);
	star2->setPosition(winSize.width / 4 - 30, winSize.height - 78);
	star3->setPosition(winSize.width / 4 +  5, winSize.height - 65);
	addChild(star1);
	addChild(star2);
	addChild(star3);

	//测试按钮，返回关卡选择界面
	auto back = MenuItemFont::create("返回关卡选择界面", this, menu_selector(GameScene::onBackCallBack));
	back->setColor(Color3B::BLACK);
	auto menu = Menu::create(back, nullptr);
	menu->setPosition(winSize.width / 2, 165);
	addChild(menu);

	//结束提示bar
	auto label = Label::createWithSystemFont("", "Arial", 36);
	label->setColor(Color3B::BLACK);
	auto label_countdown = Label::createWithSystemFont("", "Arial", 36);
	label_countdown->setColor(Color3B::BLACK);
	label_countdown->setVisible(false);
	m_end_bar = Sprite::create("GameScene/aim_bar.jpg");
	m_end_bar->setPosition(winSize.width / 2, winSize.height / 2 + 100);
	addChild(m_end_bar);
	m_end_bar->addChild(label, 0, 100);
	m_end_bar->addChild(label_countdown, 0, 101);
	label->setPosition(m_end_bar->getContentSize().width / 2, m_end_bar->getContentSize().height / 2 + 30);
	label_countdown->setPosition(m_end_bar->getContentSize().width / 2, m_end_bar->getContentSize().height / 2 - 30);
	m_end_bar->setOpacity(0);

	return true;
}

//倒计时返回关卡选择界面
void GameScene::goingBackToLevelScene(float dt)
{
	if (m_countdown != 0)
	{
		char buf[10] = {0};
		sprintf(buf, "%d", m_countdown);
		((Label*)m_end_bar->getChildByTag(101))->setString(buf);
	}

	if (m_countdown == 0)
	{
		unschedule(schedule_selector(GameScene::goingBackToLevelScene));

		auto sharedFile = FileUtils::getInstance();
		auto levels_info = sharedFile->getValueVectorFromFile("level_config.plist");
		auto scene = LevelScene::createScene(levels_info);
		auto tscene = TransitionFade::create(0.5, scene);
		Director::getInstance()->replaceScene(tscene);
	}

	m_countdown--;
}

void GameScene::onBackCallBack(Ref* pSender)
{
	auto sharedFile = FileUtils::getInstance();
	auto levels_info = sharedFile->getValueVectorFromFile("level_config.plist");
	auto scene = LevelScene::createScene(levels_info);
	Director::getInstance()->replaceScene(scene);
}

//加分并显示，返回当前积分
int GameScene::addScore(int score)
{
	//当前关卡分数增加
	m_score += score;
	char buf[10] = {0};
	sprintf(buf, "%d", m_score);

	//同步显示当前关卡分数
	auto scoreadd_label = (LabelAtlas*)(this->getChildByTag(102));
	//score_label->setString(buf);
	scoreadd_label->setString(buf);

	return m_score;

	//根据游戏得分进度使星级显现
	/*
	if (m_aim == 1)
	{
	int star1 = m_score_need / 3;
	int star2 = 2 * m_score_need / 3;
	int star3 = m_score_need;
	//1星级
	if (m_score >= star1 && m_score < star2)
	{
	((Sprite*)this->getChildByTag(200))->setVisible(true);
	}

	//2星级
	if (m_score >= star2 && m_score < star3)
	{
	((Sprite*)this->getChildByTag(201))->setVisible(true);
	}

	//3星级
	if (m_score >= star3)
	{
	((Sprite*)this->getChildByTag(202))->setVisible(true);
	}
	}
	*/
}

void GameScene::gameover(bool isWin)
{
	//游戏将终结
	log ("game over...");

	//通关label提示
	auto fadein = FadeIn::create(1);
	auto call = CallFunc::create([this]() {
		((Label*)m_end_bar->getChildByTag(101))->setVisible(true);
		m_countdown = 3;
		goingBackToLevelScene(0);
		schedule(schedule_selector(GameScene::goingBackToLevelScene), 1);
	});
	auto end_action = Sequence::create(fadein->clone(), call, nullptr);

	if (isWin)
	{
		log ("已通关");

		if (m_aim == 1)
		{
			//关卡解锁
			//得到当前最高关卡
			auto heighest_level = UserDefault::getInstance()->getIntegerForKey("HeighestLevel");

			//该关卡是否为最高关卡，是则最高关卡+1
			if (m_level_id == heighest_level)
			{
				heighest_level++;
				UserDefault::getInstance()->setIntegerForKey("HeighestLevel", heighest_level);
			}

			((Label*)m_end_bar->getChildByTag(100))->setString("you win this level");
			((Label*)m_end_bar->getChildByTag(100))->setOpacity(0);
			((Label*)m_end_bar->getChildByTag(100))->runAction(fadein);
			m_end_bar->runAction(end_action);
		}
	}
	else
	{
		log ("未通关");
		((Label*)m_end_bar->getChildByTag(100))->setString("you lose this level");
		((Label*)m_end_bar->getChildByTag(100))->setOpacity(0);
		((Label*)m_end_bar->getChildByTag(100))->runAction(fadein);
		m_end_bar->runAction(end_action);
	}
}

//减去一步，返回当前步数
int GameScene::subStep()
{
	//当前步数减一
	m_step--;
	char buf[10] = {0};
	sprintf(buf, "%d", m_step);

	//同步显示当前步数限制
	auto step_label = (LabelAtlas*)(this->getChildByTag(100));
	step_label->setString(buf);

	return m_step;
}

//更改letter Label的内容，包括加分选项，第二个参数表示是否能消除，即是否能加分
void GameScene::setLetterLabel(string letter, bool isCorrect)
{
	//不显示超过7个的字符。也就是说，词的长度不要超过7
	int letter_len = letter.size() / 3;
	if (letter_len > 6) return;

	//如果词条正确，显示加多少分
	auto scoreadd_label = (LabelAtlas*)(this->getChildByTag(102));
	if (isCorrect)
	{
		char buf[10] = {0};
		sprintf(buf, "%d", SCORE_PER_CHR * letter_len);
		scoreadd_label->setString(buf);
	}
	else
	{
		char buf[10] = {0};
		sprintf(buf, "%d", m_score);
		scoreadd_label->setString(buf);
	}

	//同步显示当前已选单词
	auto letter_label = (Label*)(this->getChildByTag(101));
	letter_label->setString(letter.c_str());
}