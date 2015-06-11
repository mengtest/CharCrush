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
	addChild(m_bg);
	//�������������С����ͼƬ���ʺ�ɲ�����
	m_bg->setScale(25 / m_bg->getContentSize().width);

	//���Label�����ݾ����ֱ���
	auto label = Label::createWithSystemFont((*str).getCString(), "Arial", 20);
	label->setTextColor(Color4B::BLACK);
	addChild(label);

	//��ʼ������λ��
	this->m_x = x;
	this->m_y = y;

	return true;
}
