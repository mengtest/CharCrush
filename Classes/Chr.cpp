#include "Chr.h"
#include "ChrsGrid.h"
#include "GameScene.h"

Chr* Chr::createWithString(String* str, int x, int y)
{
	auto chr = new Chr();

	if (chr && chr->init(str, x, y))
	{
		chr->autorelease();
		return chr;
	}
	else
	{
		CC_SAFE_DELETE(chr);
		return nullptr;
	}
}

bool Chr::init(String* str, int x, int y)
{
	Node::init();

	//初始化坐标位置
	this->m_x = x;
	this->m_y = y;

	m_special_type = 0;	//初始化为正常类型

	m_isCrushing = false;
	m_isAction = false;

	m_chr = *str;		//初始化汉字内容
	m_bg = Sprite::create("char_bg_normal.png"); //初始化汉字背景
	addChild(m_bg);

	//调整背景大小，待图片合适后可不调整
	m_bg->setScale(CHR_WITDH / m_bg->getContentSize().width);
	m_bg->setAnchorPoint(Vec2(0.5, 0.5));
	
	initArrow();	//初始化箭头

	//添加汉字内容
	auto label = Label::createWithSystemFont((*str).getCString(), "Arial", CHR_WITDH - 20);
	label->setTextColor(Color4B::BLACK);
	label->setAnchorPoint(Vec2(0.5, 0.5));
	label->setPosition(0, 0);
	addChild(label);

	initSpecial();	//初始化特殊消除标志

	return true;
}

//初始化特殊消除标志
void Chr::initSpecial()
{
	for (int i = 0; i < 8; i++)
	{
		auto special_rl = Sprite::create("right_left.png");
		auto special_bs = Sprite::create("bias.png");
		special_rl->setScale(CHR_WITDH / (5 * special_rl->getContentSize().width));
		special_bs->setScale(CHR_WITDH / (4 * special_bs->getContentSize().width));
		special_rl->setVisible(false);
		special_bs->setVisible(false);

		auto scale_rl = special_rl->getScale();
		//标签动态效果
		auto delay = DelayTime::create(2);
		auto larger = ScaleTo::create(0.5, scale_rl + 0.1);
		auto smaller = ScaleTo::create(0.5, scale_rl);
		auto action = RepeatForever::create(Sequence::create(delay, larger, smaller, nullptr));

		int x = this->getPosition().x;
		int y = this->getPosition().y;

		int delta1 = 3;
		int delta2 = 8;
		switch(i)
		{
		case 0:
			special_rl->setPosition(x, y + CHR_WITDH / 2 - delta1);
			special_rl->setRotation(90);
			break;
		case 1:
			special_rl->setPosition(x, y - CHR_WITDH / 2 + delta1);
			special_rl->setRotation(-90);
			break;
		case 2:
			special_rl->setPosition(x - CHR_WITDH / 2 + delta1, y);
			break;
		case 3:
			special_rl->setPosition(x + CHR_WITDH / 2 - delta1, y);
			special_rl->setRotation(180);
			break;
		case 4:
			special_bs->setPosition(x + CHR_WITDH / 2 - delta2, y + CHR_WITDH / 2 - delta2);
			special_bs->setRotation(90);
			break;
		case 5:
			special_bs->setPosition(x + CHR_WITDH / 2 - delta2, y - CHR_WITDH / 2 + delta2);
			special_bs->setRotation(180);
			break;
		case 6:
			special_bs->setPosition(x - CHR_WITDH / 2 + delta2, y + CHR_WITDH / 2 - delta2);
			break;
		case 7:
			special_bs->setPosition(x - CHR_WITDH / 2 + delta2, y - CHR_WITDH / 2 + delta2);
			special_bs->setRotation(-90);
			break;
		}

		if (i < 4)
		{
			m_special[i] = special_rl;
		}
		else
		{
			m_special[i] = special_bs;
		}

		//m_special[i]->runAction(action);
		addChild(m_special[i]);
	}
}

//初始化箭头背景，并使其隐藏
void Chr::initArrow()
{
	for (int i = 0; i < 8; i++)
	{
		auto arrow = Sprite::create("arrow.png");
		arrow->setScale(0.35);
		arrow->setGlobalZOrder(100);
		arrow->setVisible(false);

		int x = m_bg->getPosition().x;
		int y = m_bg->getPosition().y;

		int delta = 4;
		switch(i)
		{
		case 0:
			arrow->setPosition(x, y + CHR_WITDH / 2 + delta);
			arrow->setRotation(-90);
			break;
		case 1:
			arrow->setPosition(x, y - CHR_WITDH / 2 - delta);
			arrow->setRotation(90);
			break;
		case 2:
			arrow->setPosition(x - CHR_WITDH / 2 - delta, y);
			arrow->setRotation(180);
			break;
		case 3:
			arrow->setPosition(x + CHR_WITDH / 2 + delta, y);
			break;
		case 4:
			arrow->setPosition(x + CHR_WITDH / 2 + delta, y + CHR_WITDH / 2 + delta);
			arrow->setRotation(-45);
			break;
		case 5:
			arrow->setPosition(x + CHR_WITDH / 2 + delta, y - CHR_WITDH / 2 - delta);
			arrow->setRotation(45);
			break;
		case 6:
			arrow->setPosition(x - CHR_WITDH / 2 - delta, y + CHR_WITDH / 2 + delta);
			arrow->setRotation(-135);
			break;
		case 7:
			arrow->setPosition(x - CHR_WITDH / 2 - delta, y - CHR_WITDH / 2 - delta);
			arrow->setRotation(135);
			break;
		}
		m_arrow[i] = arrow;
		addChild(m_arrow[i]);
	}
}

void Chr::chrAciton()
{
	//如果正在执行动作，那么不执行
	if (m_isAction)
	{
		return;
	}

	m_isAction = true;
	auto chrsgrid = (ChrsGrid*)(this->getParent());

	auto scale = this->getScale();
	auto scalebigger = ScaleTo::create(0.1, scale + 0.1);
	auto scalenormal = ScaleTo::create(0.1, scale);
	auto scalesmaller = ScaleTo::create(0.1, scale - 0.2);
	auto call = CallFunc::create([this]() {
		this->m_isAction = false;
	});
	auto action = Sequence::create(scalebigger, scalesmaller, scalenormal, call, nullptr);
	this->runAction(action);
}

void Chr::hideArrow()
{
	for (int i = 0; i < 8; i++) 
	{ 
		if (m_arrow[i]->isVisible()) m_arrow[i]->setVisible(false);
	}
}

void Chr::showArrow(Chr* next_chr)
{
	Sprite* arrow = nullptr;

	//判断哪个箭头显示
	int dx = next_chr->getX() - this->getX();
	int dy = next_chr->getY() - this->getY();
	int d = abs(dx) + abs(dy);

	if (d == 1)
	{
		if (dy ==  1) { arrow = (this->getArrow())[0]; } //上
		if (dy == -1) { arrow = (this->getArrow())[1]; } //下
		if (dx == -1) { arrow = (this->getArrow())[2]; } //左
		if (dx ==  1) { arrow = (this->getArrow())[3]; } //右
	}
	if (d == 2)
	{
		if (dx > 0 && dy > 0) { arrow = (this->getArrow())[4]; } //右上
		if (dx > 0 && dy < 0) { arrow = (this->getArrow())[5]; } //右下
		if (dx < 0 && dy > 0) { arrow = (this->getArrow())[6]; } //左上
		if (dx < 0 && dy < 0) { arrow = (this->getArrow())[7]; } //左下
	}

	//箭头的显示动画，这里存在bug，并未解决。
	auto scalebigger = ScaleTo::create(0.2, 0.5);
	auto scalenormal = ScaleTo::create(0.2, 0.35);
	auto call = CallFunc::create([arrow]() {
		arrow->setScale(0.01);
		arrow->setVisible(true);
	});
	//检查此时已选汉字盒子是否为空，如果是，让此箭头隐藏。这是因为
	//如果释放操作太快，可能箭头没有隐藏，必须自行隐藏
	auto call2 = CallFunc::create([this, arrow]() {
		auto seletedBox = ((ChrsGrid*)this->getParent())->getSeletedBox();
		if (seletedBox->empty())
		{
			arrow->setVisible(false);
		}
	});

	auto action = Sequence::create(call, scalebigger, scalenormal, call2, nullptr);
	
	arrow->runAction(action);
}

string Chr::getNormalBG()
{
	if (m_special_type != 0)
	{
		return string("char_bg_special.png");
	}
	else
	{
		return string("char_bg_normal.png");
	}
}

//横消
void Chr::bombHOR()
{
	this->m_special_type = 0;

	auto chrsgrid = (ChrsGrid*)(this->getParent());
	auto chrsbox = chrsgrid->getChrsBox();

	for (int i = 0; i < chrsgrid->getCol(); i++)
	{
		//横方向上有可能出现正在消除了的，则跳过
		if ((*chrsbox)[i][m_y] ->isCrushing())
		{
			continue;
		}

		(*chrsbox)[i][m_y]->bomb();
	}
}

//竖消
void Chr::bombVAR()
{
	this->m_special_type = 0;

	auto chrsgrid = (ChrsGrid*)(this->getParent());
	auto chrsbox = chrsgrid->getChrsBox();

	for (int i = 0; i < chrsgrid->getRow(); i++)
	{
		if ((*chrsbox)[m_x][i] == nullptr)
		{
			continue;
		}

		(*chrsbox)[m_x][i]->bomb();
	}
}

//右斜消
void Chr::bombRBS()
{
	this->m_special_type = 0;

	auto chrsgrid = (ChrsGrid*)(this->getParent());
	auto chrsbox = chrsgrid->getChrsBox();

	//往下消除，因i为0，包含消除自身
	int i = 0;
	while (1)
	{
		int x = m_x - i;
		int y = m_y - i;
		if (x < 0 || y < 0)
		{
			break;
		}

		if ((*chrsbox)[x][y] == nullptr)
		{
			i++;
			continue;
		}

		(*chrsbox)[x][y]->bomb();

		i++;
	}
	//往上消除
	i = 1;
	while (1)
	{
		int x = m_x + i;
		int y = m_y + i;
		if (x == chrsgrid->getCol() || y == chrsgrid->getRow())
		{
			break;
		}

		if ((*chrsbox)[x][y] == nullptr)
		{
			i++;
			continue;
		}

		(*chrsbox)[x][y]->bomb();

		i++;
	}
}

//左斜消
void Chr::bombLBS()
{
	this->m_special_type = 0;

	auto chrsgrid = (ChrsGrid*)(this->getParent());
	auto chrsbox = chrsgrid->getChrsBox();

	//往下消除，因i为0，包含消除自身
	int i = 0;
	while (1)
	{
		int x = m_x + i;
		int y = m_y - i;
		if (x == chrsgrid->getCol() || y < 0)
		{
			break;
		}

		if ((*chrsbox)[x][y] == nullptr)
		{
			i++;
			continue;
		}

		(*chrsbox)[x][y]->bomb();

		i++;
	}
	//往上消除
	i = 1;
	while (1)
	{
		int x = m_x - i;
		int y = m_y + i;
		if (x < 0 || y == chrsgrid->getRow())
		{
			break;
		}

		if ((*chrsbox)[x][y] == nullptr)
		{
			i++;
			continue;
		}

		(*chrsbox)[x][y]->bomb();

		i++;
	}
}

//根据汉字元素之类型，对其或周边元素进行消除
void Chr::bomb()
{
	if (this->m_isCrushing == true)
	{
		return;
	}

	//简单的单消
	if (m_special_type == 0)
	{
		//加分
		((ChrsGrid*)this->getParent())->addBonus(SCORE_PER_CHR);

		this->m_isCrushing = true;
		this->setAnchorPoint(Vec2(0.5, 0.5));//为了能够从中间缩放

		//消除动作
		auto scalebigger = ScaleTo::create(0.1, 1.1);
		auto scalesmaller = ScaleTo::create(0.2, 0.01);

		//此为消除后执行的
		auto call = CallFunc::create([this]() {
			auto chrsgrid = (ChrsGrid*)(this->getParent());
			auto chrsbox = chrsgrid->getChrsBox();
			(*chrsbox)[m_x][m_y] = nullptr;
			this->removeFromParent();
			this->m_isCrushing = false;
		});
		auto action = Sequence::create(scalebigger, scalesmaller, call, nullptr);

		//执行完整消除动作
		this->runAction(action);

		return;
	}

	//横消
	if (m_special_type == SPECIAL_TYPE_HOR)
	{
		bombHOR();
	}

	//竖消
	if (m_special_type == SPECIAL_TYPE_VAR)
	{
		bombVAR();
	}

	//右斜消
	if (m_special_type == SPECIAL_TYPE_RBS)
	{
		bombRBS();
	}

	//左斜消
	if (m_special_type == SPECIAL_TYPE_LBS)
	{
		bombLBS();
	}

	//横竖十字消
	if (m_special_type == SPECIAL_TYPE_HVR)
	{
		bombHOR();
		bombVAR();
	}

	//左右斜消
	if (m_special_type == SPECIAL_TYPE_RLB)
	{
		bombRBS();
		bombLBS();
	}

	//全方位消除
	if (m_special_type == SPECIAL_TYPE_ALL)
	{
		bombHOR();
		bombVAR();
		bombRBS();
		bombLBS();
	}
}

void Chr::setSpecial(int special_type)
{
	if (special_type == 0) return; //正常类型无需设定

	m_special_type = special_type;
	m_bg->setTexture("char_bg_special.png");

	//添加特效标记
	switch (m_special_type)
	{
	case SPECIAL_TYPE_HOR :
		m_special[2]->setVisible(true);
		m_special[3]->setVisible(true);
		break;
	case SPECIAL_TYPE_VAR :
		m_special[0]->setVisible(true);
		m_special[1]->setVisible(true);
		break;
	case SPECIAL_TYPE_RBS :
		m_special[4]->setVisible(true);
		m_special[7]->setVisible(true);
		break;
	case SPECIAL_TYPE_LBS :
		m_special[5]->setVisible(true);
		m_special[6]->setVisible(true);
		break;
	case SPECIAL_TYPE_HVR :
		m_special[0]->setVisible(true);
		m_special[1]->setVisible(true);
		m_special[2]->setVisible(true);
		m_special[3]->setVisible(true);
		break;
	case SPECIAL_TYPE_RLB :
		m_special[4]->setVisible(true);
		m_special[7]->setVisible(true);
		m_special[5]->setVisible(true);
		m_special[6]->setVisible(true);
		break;
	case SPECIAL_TYPE_ALL :
		for (int i = 0; i < 8; i++)
		{
			m_special[i]->setVisible(true);
		}
		break;
	}
}
