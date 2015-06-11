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

	//���Label�����ݾ����ֱ���
	auto label = Label::createWithSystemFont((*str).getCString(), "Arial", CHR_WITDH - 3);
	label->setTextColor(Color4B::BLACK);
	label->setAnchorPoint(Vec2(0, 0));
	addChild(label);

	//��ʼ������λ��
	this->m_x = x;
	this->m_y = y;

	return true;
}
