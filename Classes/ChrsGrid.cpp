#include "ChrsGrid.h"
#include "Chr.h"

ChrsGrid* ChrsGrid::create(const char* letterlist, int row, int col)
{
	auto chrsgrid = new ChrsGrid();

	if (chrsgrid && chrsgrid->init(letterlist, row, col))
	{
		chrsgrid->autorelease();
		return chrsgrid;
	}
	else
	{
		CC_SAFE_DELETE(chrsgrid);
		return nullptr;
	}	
}

bool ChrsGrid::init(const char* letterslist, int row, int col)
{
	Node::init();

	//得到单词集合
	auto sharedFile = FileUtils::getInstance();
	m_Letters = sharedFile->getValueVectorFromFile(letterslist);

	//初始化汉字集合
	initChrBox();
	
	//生成布局
	//根据行列初始化一个空的汉字盒子大小
	m_row = row;
	m_col = col;
	m_SelectedChrs.clear();
	m_ChrsBox.resize(m_row);
	for (auto &vec : m_ChrsBox) { vec.resize(m_col); }

	//1.根据布局大小创建出汉字阵列
	//2.布局坐标以左下角为原点，x右y上为正方向
	for (int x = 0; x < m_col; x++)
	{
		for (int y = 0; y < m_row; y++)
		{
			m_ChrsBox[x][y] = createAChr(x, y); 
		}
	}

	//加入触摸监听
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(ChrsGrid::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(ChrsGrid::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(ChrsGrid::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	log("ChrsGrid init!");

	return true;
}

bool ChrsGrid::onTouchBegan(Touch* pTouch, Event*)
{
	//将触摸点的坐标转化为模型坐标
	auto pos = this->convertToNodeSpace(pTouch->getLocation());
	
	//是否有按在汉字阵列上
	if (Rect(0, 0, m_col*GRID_WIDTH, m_row*GRID_WIDTH).containsPoint(pos))
	{
		//得到阵列坐标
		int x = pos.x / GRID_WIDTH;
		int y = pos.y / GRID_WIDTH;

		//得到当前选中的汉字元素，鬓设置选中颜色
		auto chr = m_ChrsBox[x][y];
		chr->getBg()->setTexture("char_bg_selected.png");

		//加入临时选定汉字集合
		m_SelectedChrs.pushBack(chr);

		//log("touch coordinate: x=%d,y=%d", x, y);

		return true;
	}
	else
	{
		return false;
	}
}

void ChrsGrid::onTouchMoved(Touch* pTouch, Event*)
{
	//移动时也可选择
	//将触摸点的坐标转化为模型坐标
	auto pos = this->convertToNodeSpace(pTouch->getLocation());

	//将已选的合法汉字加入临时已选汉字盒子
	if (Rect(0, 0, m_col*GRID_WIDTH, m_row*GRID_WIDTH).containsPoint(pos))
	{
		//得到触摸点阵列坐标
		int x = pos.x / GRID_WIDTH;
		int y = pos.y / GRID_WIDTH;

		//得到当前触摸点的汉字元素，以及最后一次选择的汉字
		auto chr = m_ChrsBox[x][y];
		auto last_chr = m_SelectedChrs.back();
		
		//判断当前触摸点的汉字是否与最后一次选择的相邻
		int dx = abs(chr->getX() - last_chr->getX());
		int dy = abs(chr->getY() - last_chr->getY());
		int d = dx + dy;
		
		if (dx < 2 && dy < 2 && d <= 2 && d > 0)
		{
			//如果符合情况，那么将其加入临时选择汉字盒子，并设置选中颜色
			//只有当临时选择汉字集合中没有该汉字元素时，才添加
			if (!m_SelectedChrs.contains(chr))
			{
				m_SelectedChrs.pushBack(chr);
				chr->getBg()->setTexture("char_bg_selected.png");
			}
		}
	}
}

void ChrsGrid::onTouchEnded(Touch*, Event*)
{
	//如果能消除，那么消除...

	//如果不能，改变回背景颜色
	for (auto &chr : m_SelectedChrs)
	{
		chr->getBg()->setTexture("char_bg_normal.png");
	}

	//清空临时已选汉字集合
	m_SelectedChrs.clear();
}

Chr* ChrsGrid::createAChr(int x, int y)
{
	//1.根据布局坐标创建一个汉字元素，汉字内容根据汉字集合随机获得
	//2.设置该汉字元素的世界坐标
	//3.将该汉字加入渲染节点
	Chr* chr = nullptr;

	int i = random(0, (int)m_Chrs.size()-1);
	auto s = m_Chrs.at(i);
	chr = Chr::createWithString(s, x, y);

	chr->setPosition(x * GRID_WIDTH, y * GRID_WIDTH);
	addChild(chr);

	//log("add a chr!---content:%s---x:%d---y:%d", chr->getString().getCString(), x, y);

	return chr;
}

//判断字是否在字集合中
bool ChrsGrid::isChrExist(Vector<String*>* pVec, String* letter)
{
	auto vec = *pVec;

	for (auto &letter_cur : m_Chrs)
	{
		if (strcmp(letter_cur->getCString(), letter->getCString()) == 0)
		{
			return true;
		}
	}

	return false;
}

//从单词列表文件中，提取出汉字，放到汉字集合中
void ChrsGrid::initChrBox()
{
	m_Chrs.clear();

	//从单词集合中，挑选出不重复的字，放入字集合中
	for (auto &val : m_Letters)
	{
		//letter代表每一条单词
		auto letter = val.asString();

		//遍历每一条单词的汉字，将字放入字集合
		int i = 0;
		while (i < letter.size())
		{
			//每一个汉字占据3个字节，加上最后的空字符，共四个字节组成一个String
			char buf[4] = {0};
			memcpy(buf, &letter.at(i), 3);
			auto letter = String::createWithFormat("%s", buf);

			//判断该单词是否在字集合中有重复，不重复才加入字集合
			if (!isChrExist(&m_Chrs, letter))
			{
				m_Chrs.pushBack(letter);
			}

			i += 3;
		}
	}
}