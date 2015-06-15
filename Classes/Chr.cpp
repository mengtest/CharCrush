#include "Chr.h"

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

	m_chr = *str;
	m_bg = Sprite::create("char_bg_normal.png");
	//�������������С����ͼƬ���ʺ�ɲ�����
	m_bg->setScale(CHR_WITDH / m_bg->getContentSize().width);
	m_bg->setAnchorPoint(Vec2(0, 0));
	addChild(m_bg);

	//��Ӽ�ͷ��������ʹ������
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

	//���Label�����ݾ����ֱ���
	auto label = Label::createWithSystemFont((*str).getCString(), "Arial", CHR_WITDH - 8);
	label->setTextColor(Color4B::BLACK);
	label->setAnchorPoint(Vec2(0, 0));
	label->setPosition(4, 2);
	addChild(label);

	//��ʼ������λ��
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
	//�ж��ĸ���ͷ��ʾ
	int dx = next_chr->getX() - this->getX();
	int dy = next_chr->getY() - this->getY();
	int d = abs(dx) + abs(dy);
	if (d == 1)
	{
		if (dy ==  1) { (this->getArrow())[0]->setVisible(true); } //��
		if (dy == -1) { (this->getArrow())[1]->setVisible(true); } //��
		if (dx == -1) { (this->getArrow())[2]->setVisible(true); } //��
		if (dx ==  1) { (this->getArrow())[3]->setVisible(true); } //��
	}
	if (d == 2)
	{
		if (dx > 0 && dy > 0) { (this->getArrow())[4]->setVisible(true); } //����
		if (dx > 0 && dy < 0) { (this->getArrow())[5]->setVisible(true); } //����
		if (dx < 0 && dy > 0) { (this->getArrow())[6]->setVisible(true); } //����
		if (dx < 0 && dy < 0) { (this->getArrow())[7]->setVisible(true); } //����
	}
}