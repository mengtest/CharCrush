#include "Chr.h"
#include "ChrsGrid.h"

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

	//初始化为正常类型
	m_special_type = 0;

	m_chr = *str;
	m_bg = Sprite::create("char_bg_normal.png");
	//调整背景纹理大小，待图片合适后可不调整
	m_bg->setScale(CHR_WITDH / m_bg->getContentSize().width);
	m_bg->setAnchorPoint(Vec2(0, 0));
	addChild(m_bg);

	//添加箭头背景，并使其隐藏
	for (int i = 0; i < 8; i++)
	{
		auto arrow = Sprite::create("arrow.png");
		arrow->setScale(CHR_WITDH / (3 * arrow->getContentSize().width));
		arrow->setVisible(false);

		int delta = 10;
		switch(i)
		{
		case 0:
			arrow->setPosition(CHR_WITDH / 2, CHR_WITDH + delta);
			arrow->setRotation(-90);
			break;
		case 1:
			arrow->setPosition(CHR_WITDH / 2, -delta);
			arrow->setRotation(90);
			break;
		case 2:
			arrow->setPosition(-delta, CHR_WITDH / 2);
			arrow->setRotation(180);
			break;
		case 3:
			arrow->setPosition(CHR_WITDH + delta, CHR_WITDH / 2);
			break;
		case 4:
			arrow->setPosition(CHR_WITDH + delta, CHR_WITDH + delta);
			arrow->setRotation(-45);
			break;
		case 5:
			arrow->setPosition(CHR_WITDH + delta, -delta);
			arrow->setRotation(45);
			break;
		case 6:
			arrow->setPosition(-delta, CHR_WITDH + delta);
			arrow->setRotation(-135);
			break;
		case 7:
			arrow->setPosition(-delta, -delta);
			arrow->setRotation(135);
			break;
		}
		m_arrow[i] = arrow;
		addChild(m_arrow[i]);
	}

	//添加特殊标志并使其隐藏
	for (int i = 0; i < 8; i++)
	{
		auto special_rl = Sprite::create("right_left.png");
		auto special_bs = Sprite::create("bias.png");
		special_rl->setScale(CHR_WITDH / (5 * special_rl->getContentSize().width));
		special_bs->setScale(CHR_WITDH / (4 * special_bs->getContentSize().width));
		special_rl->setVisible(false);
		special_bs->setVisible(false);

		int delta1 = 3;
		int delta2 = 5;
		switch(i)
		{
		case 0:
			special_rl->setPosition(CHR_WITDH / 2, CHR_WITDH - delta1);
			special_rl->setRotation(90);
			break;
		case 1:
			special_rl->setPosition(CHR_WITDH / 2, delta1);
			special_rl->setRotation(-90);
			break;
		case 2:
			special_rl->setPosition(delta1, CHR_WITDH / 2);
			break;
		case 3:
			special_rl->setPosition(CHR_WITDH - delta1, CHR_WITDH / 2);
			special_rl->setRotation(180);
			break;
		case 4:
			special_bs->setPosition(CHR_WITDH - delta2, CHR_WITDH - delta2);
			special_bs->setRotation(90);
			break;
		case 5:
			special_bs->setPosition(CHR_WITDH - delta2, delta2);
			special_bs->setRotation(180);
			break;
		case 6:
			special_bs->setPosition(delta2, CHR_WITDH - delta2);
			break;
		case 7:
			special_bs->setPosition(delta2, delta2);
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

		addChild(m_special[i]);
	}

	//添加Label，内容就是字本身
	auto label = Label::createWithSystemFont((*str).getCString(), "Arial", CHR_WITDH - 8);
	label->setTextColor(Color4B::BLACK);
	label->setAnchorPoint(Vec2(0, 0));
	label->setPosition(4, 2);
	addChild(label);

	//初始化坐标位置
	this->m_x = x;
	this->m_y = y;

	return true;
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
	//判断哪个箭头显示
	int dx = next_chr->getX() - this->getX();
	int dy = next_chr->getY() - this->getY();
	int d = abs(dx) + abs(dy);
	if (d == 1)
	{
		if (dy ==  1) { (this->getArrow())[0]->setVisible(true); } //上
		if (dy == -1) { (this->getArrow())[1]->setVisible(true); } //下
		if (dx == -1) { (this->getArrow())[2]->setVisible(true); } //左
		if (dx ==  1) { (this->getArrow())[3]->setVisible(true); } //右
	}
	if (d == 2)
	{
		if (dx > 0 && dy > 0) { (this->getArrow())[4]->setVisible(true); } //右上
		if (dx > 0 && dy < 0) { (this->getArrow())[5]->setVisible(true); } //右下
		if (dx < 0 && dy > 0) { (this->getArrow())[6]->setVisible(true); } //左上
		if (dx < 0 && dy < 0) { (this->getArrow())[7]->setVisible(true); } //左下
	}
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
void Chr::bombHOR(ChrsGrid* chrsgrid)
{
	auto chrsbox = chrsgrid->getChrsBox();

	for (int i = 0; i < chrsgrid->getCol(); i++)
	{
		//横方向上有可能出现已经消除了的，则跳过
		if ((*chrsbox)[i][m_y] == nullptr)
		{
			continue;
		}

		(*chrsbox)[i][m_y]->removeFromParent();
		(*chrsbox)[i][m_y] = nullptr;
	}
}

//竖消
void Chr::bombVAR(ChrsGrid* chrsgrid)
{
	auto chrsbox = chrsgrid->getChrsBox();

	for (int i = 0; i < chrsgrid->getRow(); i++)
	{
		if ((*chrsbox)[m_x][i] == nullptr)
		{
			continue;
		}

		(*chrsbox)[m_x][i]->removeFromParent();
		(*chrsbox)[m_x][i] = nullptr;
	}
}

//右斜消
void Chr::bombRBS(ChrsGrid* chrsgrid)
{
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

		(*chrsbox)[x][y]->removeFromParent();
		(*chrsbox)[x][y] = nullptr;

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

		(*chrsbox)[x][y]->removeFromParent();
		(*chrsbox)[x][y] = nullptr;

		i++;
	}
}

//左斜消
void Chr::bombLBS(ChrsGrid* chrsgrid)
{
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

		(*chrsbox)[x][y]->removeFromParent();
		(*chrsbox)[x][y] = nullptr;

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

		(*chrsbox)[x][y]->removeFromParent();
		(*chrsbox)[x][y] = nullptr;

		i++;
	}
}

//根据汉字元素之类型，对其或周边元素进行消除
void Chr::bomb()
{
	auto chrsgrid = (ChrsGrid*)(this->getParent());

	//如果父节点为空，说明该元素已经被消除了，则无需再消除
	if (chrsgrid == nullptr)
	{
		return;
	}

	auto chrsbox = chrsgrid->getChrsBox();

	//简单的单消
	if (m_special_type == 0)
	{
		(*chrsbox)[m_x][m_y] = nullptr;
		this->removeFromParent();
	}

	//横消
	if (m_special_type == SPECIAL_TYPE_HOR)
	{
		bombHOR(chrsgrid);
	}

	//竖消
	if (m_special_type == SPECIAL_TYPE_VAR)
	{
		bombVAR(chrsgrid);
	}

	//右斜消
	if (m_special_type == SPECIAL_TYPE_RBS)
	{
		bombRBS(chrsgrid);
	}

	//左斜消
	if (m_special_type == SPECIAL_TYPE_LBS)
	{
		bombLBS(chrsgrid);
	}

	//横竖十字消
	if (m_special_type == SPECIAL_TYPE_HVR)
	{
		bombHOR(chrsgrid);
		bombVAR(chrsgrid);
	}

	//左右斜消
	if (m_special_type == SPECIAL_TYPE_RLB)
	{
		bombRBS(chrsgrid);
		bombLBS(chrsgrid);
	}

	//全方位消除
	if (m_special_type == SPECIAL_TYPE_ALL)
	{
		bombHOR(chrsgrid);
		bombVAR(chrsgrid);
		bombRBS(chrsgrid);
		bombLBS(chrsgrid);
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
