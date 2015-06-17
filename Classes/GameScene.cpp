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
	//设定一个裁剪节点
	auto clipper = ClippingNode::create();
	//以阵列的布局背景做裁剪模板，此模板不会被绘制，只是用来遮罩的
	clipper->setStencil(chrsgrid->getChildByTag(1000));
	//clipper->setStencil(Sprite::create("bg.png"));
	//clipper->setInverted(true);
	clipper->setContentSize(winSize);
	clipper->setPosition(winSize.width / 2 + 10, winSize.height / 2 + 50);
	//将chrsgrid加入裁剪节点，不在裁剪模板范围内的将不显示
	clipper->addChild(chrsgrid);
	addChild(clipper);

	//根据关卡配置获得游戏关卡的限定步数
	m_step = level_info.at("step").asInt();
	//添加步数Label，m_step为0无需添加
	char buf[10] = {0};
	sprintf(buf, "%d", m_step);
	auto step_label = LabelAtlas::create(buf, "labelatlas.png", 17, 22, '0');
	step_label->setPosition(Vec2(winSize.width / 4 - 30, 
								 winSize.height - 35));
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
	scoreadd_label->setPosition(Vec2(winSize.width / 4 * 3 + 35, 
								 winSize.height - 25));
	scoreadd_label->setTag(102);
	scoreadd_label->setAnchorPoint(Vec2(0.5, 0.5));
	addChild(scoreadd_label);

	//添加当前分数Label
	m_score = 0;
	auto score_label = LabelAtlas::create("0", "labelatlas.png", 17, 22, '0');
	score_label->setPosition(Vec2(winSize.width / 2, 
								 winSize.height - 100));
	score_label->setTag(103);
	score_label->setScale(1.5);
	score_label->setAnchorPoint(Vec2(0.5, 0.5));
	addChild(score_label);

	//临时添加，通关所需分数
	m_score_start1 = level_info.at("score1").asInt();
	m_score_start2 = level_info.at("score2").asInt();
	m_score_start3 = level_info.at("score3").asInt();
	char buf1[100] = {0};
	sprintf(buf1, "(通关所需分数:%d)", m_score_start1);
	auto label_score_need = Label::createWithSystemFont(buf1, "Arial", 20);
	label_score_need->setPosition(score_label->getPosition());
	label_score_need->setPositionX(label_score_need->getPositionX() + 150);
	label_score_need->setColor(Color3B::BLACK);
	addChild(label_score_need);

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

	return true;
}

void GameScene::onBackCallBack(Ref* pSender)
{
	Director::getInstance()->popScene();
}

void GameScene::addScore(int score)
{
	m_score += score;
	char buf[10] = {0};
	sprintf(buf, "%d", m_score);

	auto score_label = (LabelAtlas*)(this->getChildByTag(103));
	score_label->setString(buf);

	//根据获得的分数使星星显现
	if (m_score >= m_score_start1) ((Sprite*)this->getChildByTag(200))->setVisible(true);
	if (m_score >= m_score_start2) ((Sprite*)this->getChildByTag(201))->setVisible(true);
	if (m_score >= m_score_start3) ((Sprite*)this->getChildByTag(202))->setVisible(true);
}

//步数减一
void GameScene::subStep()
{
	m_step--;
	char buf[10] = {0};
	sprintf(buf, "%d", m_step);

	auto step_label = (LabelAtlas*)(this->getChildByTag(100));
	step_label->setString(buf);
}

void GameScene::setLetterLabel(string letter, bool isCorrect)
{
	//不显示超过7个的字符。也就是说，词的长度不要超过7
	int letter_len = letter.size() / 3;
	if (letter_len > 7) return;
	
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
		scoreadd_label->setString("0");
	}

	auto letter_label = (Label*)(this->getChildByTag(101));
	letter_label->setString(letter.c_str());
}