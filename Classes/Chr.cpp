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
	//调整背景纹理大小，待图片合适后可不调整
	m_bg->setScale(25 / m_bg->getContentSize().width);

	//添加Label，内容就是字本身
	auto label = Label::createWithSystemFont((*str).getCString(), "Arial", 20);
	label->setTextColor(Color4B::BLACK);
	addChild(label);

	//初始化坐标位置
	this->m_x = x;
	this->m_y = y;

	return true;
}
