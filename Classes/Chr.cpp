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
	//调整背景纹理大小，待图片合适后可不调整
	m_bg->setScale(CHR_WITDH / m_bg->getContentSize().width);
	m_bg->setAnchorPoint(Vec2(0, 0));
	addChild(m_bg);

	//添加Label，内容就是字本身
	auto label = Label::createWithSystemFont((*str).getCString(), "Arial", CHR_WITDH - 3);
	label->setTextColor(Color4B::BLACK);
	label->setAnchorPoint(Vec2(0, 0));
	addChild(label);

	//初始化坐标位置
	this->m_x = x;
	this->m_y = y;

	return true;
}
