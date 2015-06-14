#include "ChrsGrid.h"
#include "Chr.h"

ChrsGrid* ChrsGrid::create(ValueMap level_info, int row, int col)
{
	auto chrsgrid = new ChrsGrid();

	if (chrsgrid && chrsgrid->init(level_info, row, col))
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

bool ChrsGrid::init(ValueMap level_info, int row, int col)
{
	Node::init();

	//得到单词集合
	//auto sharedFile = FileUtils::getInstance();
	//m_Letters = sharedFile->getValueVectorFromFile(letterslist);
	m_Letters = level_info.at("letter").asValueVector();

	//初始化汉字集合
	initChrBox();
	
	//生成布局
	//根据行列初始化一个空的汉字盒子大小
	m_row = row;
	m_col = col;
	m_canCrush = false;
	m_SelectedChrs.clear();
	m_NewChrs.clear();
	m_ChrsBox.resize(m_row);
	for (auto &vec : m_ChrsBox) { vec.resize(m_col); }

	//生成汉字字典树
	createTrie(&chr_root, &m_Letters);

	//1.根据布局大小创建出汉字阵列
	//2.布局坐标以左下角为原点，x右y上为正方向
	for (int x = 0; x < m_col; x++)
	{
		for (int y = 0; y < m_row; y++)
		{ 
			m_ChrsBox[x][y] = createAChr(x, y); 
		}
	}

	//判断是否是死图
	while (isDeadMap())
	{
		//这里稍后做一个更新的算法
		for (int x = 0; x < m_col; x++)
		{
			for (int y = 0; y < m_row; y++)
			{ 
				m_ChrsBox[x][y]->removeFromParent();
				m_ChrsBox[x][y] = createAChr(x, y); 
			}
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

	//得到阵列坐标
	int x = pos.x / GRID_WIDTH;
	int y = pos.y / GRID_WIDTH;

	//得到汉字原点模型坐标
	auto chr_pos = Vec2(x * GRID_WIDTH, y * GRID_WIDTH);
	
	//是否有按在汉字上
	if (Rect(chr_pos.x, chr_pos.y, CHR_WITDH, CHR_WITDH).containsPoint(pos))
	{
		//得到当前选中的汉字元素，鬓设置选中颜色
		auto chr = m_ChrsBox[x][y];
		chr->getBg()->setTexture("char_bg_selected.png");

		//加入临时选定汉字集合
		m_SelectedChrs.pushBack(chr);

		//得到能否消除的状态
		m_canCrush = canCrush();

		//log("touch coordinate: x=%d,y=%d", x, y);

		return true;
	}
	else
	{
		return false;
	}
}

/**
  *函数说明：移动触摸，将触摸到的新汉字元素放进已选汉字盒子
  *如果是倒退，那么将最后一个汉字元素从已选汉字盒子中删除
  *每一次增加/删除汉字元素，都会判断当前已选的汉字是否能消除
  */
void ChrsGrid::onTouchMoved(Touch* pTouch, Event*)
{
	//移动时也可选择
	//将触摸点的坐标转化为模型坐标
	auto pos = this->convertToNodeSpace(pTouch->getLocation());

	//得到阵列坐标
	int x = pos.x / GRID_WIDTH;
	int y = pos.y / GRID_WIDTH;

	//得到汉字原点模型坐标
	auto chr_pos = Vec2(x * GRID_WIDTH, y * GRID_WIDTH);

	//将已选的合法汉字加入临时已选汉字盒子
	if (y < m_row && x < 6 && Rect(chr_pos.x, chr_pos.y, CHR_WITDH, CHR_WITDH).containsPoint(pos))
	{
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
				//判断哪个箭头显示
				int dx = chr->getX() - last_chr->getX();
				int dy = chr->getY() - last_chr->getY();
				if (d == 1)
				{
					if (dy ==  1) { (last_chr->getArrow())[0]->setVisible(true);; } //上
					if (dy == -1) { (last_chr->getArrow())[1]->setVisible(true);; } //下
					if (dx == -1) { (last_chr->getArrow())[2]->setVisible(true);; } //左
					if (dx ==  1) { (last_chr->getArrow())[3]->setVisible(true);; } //右
				}
				if (d == 2)
				{
					if (dx > 0 && dy > 0) { (last_chr->getArrow())[4]->setVisible(true); } //右上
					if (dx > 0 && dy < 0) { (last_chr->getArrow())[5]->setVisible(true); } //右下
					if (dx < 0 && dy > 0) { (last_chr->getArrow())[6]->setVisible(true); } //左上
					if (dx < 0 && dy < 0) { (last_chr->getArrow())[7]->setVisible(true); } //左下
				}
				
				m_SelectedChrs.pushBack(chr);
				chr->getBg()->setTexture("char_bg_selected.png");
				
				//得到能否消除的状态
				m_canCrush = canCrush();
			}
		}

		//如果当前触摸点是已选汉字盒子中倒数第二个汉字，说明是后退操作
		//将倒数第一个元素删除出已选汉字盒子
		if (m_SelectedChrs.size() >= 2)
		{
			//得到倒数第二个元素，判断是否和触摸点的元素一致
			auto secondlast_chr = m_SelectedChrs.at(m_SelectedChrs.size()-2);
			if (secondlast_chr == chr)
			{
				//将最后一个元素删除出去
				m_SelectedChrs.back()->getBg()->setTexture("char_bg_normal.png");
				m_SelectedChrs.popBack();

				//然后将现有最后一个的汉字的箭头隐藏
				auto chr = m_SelectedChrs.back();
				auto arrow = chr->getArrow();
				for (int i = 0; i < 8; i++) { if (arrow[i]->isVisible()) arrow[i]->setVisible(false); }

				//得到能否消除的状态
				m_canCrush = canCrush();
			}
		}
	}
}

bool ChrsGrid::canCrush()
{
	//将临时已选汉字组成一个字符串
	char selected_str[1000] = {0};
	for (auto &chr : m_SelectedChrs)
	{
		strcat(selected_str, chr->getString().getCString());
	}

	//遍历单词集合，如果和其中一条吻合，那么即可消除
	for (auto &val : m_Letters)
	{
		if (strcmp(selected_str, val.asString().c_str()) == 0)
		{
			//将其颜色设置成可消除
			for (auto &chr : m_SelectedChrs)
			{
				chr->getBg()->setTexture("char_bg_crush.png");
			}
			return true;
		}
	}

	//遍历完毕也没有，那么说明不能消除
	//将其颜色设置成选择
	for (auto &chr : m_SelectedChrs)
	{
		chr->getBg()->setTexture("char_bg_selected.png");
	}
	return false;
}

void ChrsGrid::onTouchEnded(Touch*, Event*)
{
	//如果能消除，那么清除已选文字
	if (m_canCrush)
	{
		//首先暂停触摸
		_eventDispatcher->pauseEventListenersForTarget(this);

		for (auto &chr : m_SelectedChrs)
		{
			//清除该汉字，然后添加一个新汉字到新汉字盒子
			clearSelectedChr(chr);
		}

		//使汉字掉落，同时开启掉落状态捕捉函数
		dropChrs();
		schedule(schedule_selector(ChrsGrid::onChrsDropping), 0.1);
	}
	else
	{
		//如果不能，改变回背景颜色，其箭头也隐藏
		for (auto &chr : m_SelectedChrs)
		{
			chr->getBg()->setTexture("char_bg_normal.png");

			auto arrow = chr->getArrow();
			for (int i = 0; i < 8; i++) { if (arrow[i]->isVisible()) arrow[i]->setVisible(false); }
		}
	}
	
	//清空临时已选汉字集合
	m_SelectedChrs.clear();
}

//汉字掉落状态捕捉函数，当新汉字掉落完，并清除出新汉字盒子时，掉落完毕
void ChrsGrid::onChrsDropping(float dt) 
{
	//如果新汉字都掉落完，停止该捕捉函数，并恢复触摸
	if (m_NewChrs.empty())
	{
		unschedule(schedule_selector(ChrsGrid::onChrsDropping));

		//判断是否是死图
		while (isDeadMap())
		{
			//这里稍后做一个更新的算法
			//1.根据布局大小创建出汉字阵列
			//2.布局坐标以左下角为原点，x右y上为正方向
			for (int x = 0; x < m_col; x++)
			{
				for (int y = 0; y < m_row; y++)
				{ 
					m_ChrsBox[x][y]->removeFromParent();
					m_ChrsBox[x][y] = createAChr(x, y); 
				}
			}
		}

		_eventDispatcher->resumeEventListenersForTarget(this);
	}
}
 
void ChrsGrid::clearSelectedChr(Chr* chr)
{
	//将汉字盒子中的对应元素置为空
	m_ChrsBox[chr->getX()][chr->getY()] = nullptr;

	//生成一个新的汉字元素，将其加入临时新汉字盒子
	//与欲消除的汉字同列，但位于阵列最上面一行的上一行
	auto new_chr = createAChr(chr->getX(), m_row);
	m_NewChrs.pushBack(new_chr);

	//将汉字元素从父节点移除
	chr->removeFromParent();
}

void ChrsGrid::dropChrs()
{
	for (int x = 0; x < m_col; x++)
	{
		auto pChrsBox = &m_ChrsBox;

		int space = 0;//记录当前列的空格数
		for (int y = 0; y < m_row; y++)
		{
			auto chr = m_ChrsBox[x][y];

			if (!chr)
			{
				space++;
				continue;
			}
			else if (space != 0)//如果该列出现了空格，那么应该掉落
			{
				chr->setY(chr->getY() - space);//重置其坐标y值

				auto move = MoveBy::create(0.2, Vec2(0, -space*GRID_WIDTH));
				auto call = CallFunc::create([pChrsBox, chr](){
					//更新汉字盒子内的数据
					(*pChrsBox)[chr->getX()][chr->getY()] = chr;
				});

				chr->runAction(Sequence::create(move, call, nullptr));//准备下落的动作
			}
		}

		//如果遍历完列，space不为0，那么该列存在新汉字，让新汉字掉落
		if (space == 0)
		{
			continue;
		}

		int n = space;
		int delta = 1;//设定速度阶梯的辅助变量
		for (auto &chr : m_NewChrs)
		{
			if (chr->getX() == x)
			{
				chr->setY(m_row - n);

				auto delay = DelayTime::create(0.2);
				//后下落的速度设置慢一些
				auto move = MoveBy::create(0.2*delta++, Vec2(0, -n--*GRID_WIDTH));
				auto call = CallFunc::create([chr, pChrsBox, this](){
					(*pChrsBox)[chr->getX()][chr->getY()] = chr;
					//从新汉字盒子中移除该汉字，也就是说，当新汉字盒子为空时，消除结束
					m_NewChrs.eraseObject(chr);
				});

				chr->runAction(Sequence::create(delay, move, call, nullptr));
			}
		}
	}
}

bool ChrsGrid::isDeadMap()
{
	//遍历每一个汉字元素
	for (int x = 0; x < m_col; x++)
	{
		//遍历列
		for (int y = 0; y < m_row; y++)
		{
			//从汉字元素盒子中选定一个汉字元素
			auto chr = m_ChrsBox[x][y];
			string letter;
			if (findRoot(chr, &letter))
			{
				return false;//找到路了，则不是死图，否则继续下一个chr
			}
		}
	}

	log ("this is a deadmap!");
	return true;
}

bool ChrsGrid::findRoot(Chr* chr, string* letter)
{
	bool ending = false;//标记该汉字元素是否为结束汉字

	string old_letter = *letter;//保留原有的letter用于恢复

	//将汉字组成一个新的letter
	*letter = *letter + string(chr->getString().getCString());
	
	//如果选定的letter能被字典包含，再判断该letter是否终点
	if (isLetterMatchingTrie(&chr_root, letter, &ending))
	{
		//如果为终点，则成功寻路，否则再选一个周边的汉字，递归遍历
		if (ending == true)
		{
			//log ("%s", letter->c_str());
			return true;
		}
		else
		{
			//递归遍历临近的汉字元素
			//遍历上
			if (chr->getY() < m_row - 1)
			{
				auto next_chr = m_ChrsBox[chr->getX()][chr->getY() + 1];
				if (findRoot(next_chr, letter))
				{
					return true;
				}
			}

			//遍历下
			if (chr->getY() > 0)
			{
				auto next_chr = m_ChrsBox[chr->getX()][chr->getY() - 1];
				if (findRoot(next_chr, letter))
				{
					return true;
				}
			}
			
			//遍历左
			if (chr->getX() > 0)
			{
				auto next_chr = m_ChrsBox[chr->getX() - 1][chr->getY()];
				if (findRoot(next_chr, letter))
				{
					return true;
				}
			}

			//遍历右
			if (chr->getX() < m_col - 1)
			{
				auto next_chr = m_ChrsBox[chr->getX() + 1][chr->getY()];
				if (findRoot(next_chr, letter))
				{
					return true;
				}
			}

			//遍历右上
			if ((chr->getX() < m_col - 1) && (chr->getY() < m_row - 1))
			{
				auto next_chr = m_ChrsBox[chr->getX() + 1][chr->getY() + 1];
				if (findRoot(next_chr, letter))
				{
					return true;
				}
			}

			//遍历右下
			if ((chr->getX() < m_col - 1) && chr->getY() > 0)
			{
				auto next_chr = m_ChrsBox[chr->getX() + 1][chr->getY() - 1];
				if (findRoot(next_chr, letter))
				{
					return true;
				}
			}

			//遍历左上
			if (chr->getX() > 0 && (chr->getY() < m_row - 1))
			{
				auto next_chr = m_ChrsBox[chr->getX() - 1][chr->getY() + 1];
				if (findRoot(next_chr, letter))
				{
					return true;
				}
			}

			//遍历左下
			if (chr->getX() > 0 && chr->getY() > 0)
			{
				auto next_chr = m_ChrsBox[chr->getX() - 1][chr->getY() - 1];
				if (findRoot(next_chr, letter))
				{
					return true;
				}
			}

			//上下左右全遍历了而未返回，说明没找到路
			return false;
		}
	}
	else
	{
		*letter = old_letter;//该路行不通，回复原有的letter
		return false;
	}
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

bool isChrExist(String* chr, struct ChrTrie *p, int *n)
{
	int i = 0;
    *n = i;

    while (p->next[i] != NULL && i < MAX)
    {
		if (strcmp(chr->getCString(), p->next[i]->chr) == 0)
        {
            return true;
        }

        i++;
        *n = i;
    }

    return false;
}

void createTrie(struct ChrTrie* chr_root, ValueVector* letters)
{
	//使字典树根中的next内容都为空
	for (int i = 0; i < MAX; i++)
	{
		chr_root->next[i] = nullptr;
		memset(chr_root->chr, 0, sizeof(chr_root->chr));
		chr_root->isEnding = false;
	}

	for (auto &val : *letters)
	{
		//每一次遍历一个letter，都从根节点开始
		struct ChrTrie *p = chr_root;

		auto letter = val.asString();//获得单个单词
		int i = 0;
		while (i < letter.size())
		{
			//从letter中选定一个字
			char buf[4] = {0};
			memcpy(buf, &letter.at(i), 3);
			auto chr = String::createWithFormat("%s", buf);

			 //判断该字是否存在于p的子节点中
            int n = 0;//n表示p的字节点第几个是NULL，用来进行填充
			if (isChrExist(chr, p, &n) == false)
            {
                //如果不存在，那么给该字开辟一个空间
                struct ChrTrie *pChr = (struct ChrTrie*)malloc(sizeof(struct ChrTrie));
				for (int i = 0; i < MAX; i++)
				{
					pChr->next[i] = nullptr;
					memset(pChr->chr, 0, sizeof(pChr->chr));
					pChr->isEnding = false;
				}
				strcpy(pChr->chr, chr->getCString());//将其内容设置成chr
                p->next[n] = pChr;
                p = p->next[n];//p下移
            }
			else //如果该字存在于p的字节点中
			{
				p = p->next[n];
			}

			//如果该汉字是单词的最后一个字，置结尾标志为true
			if (i == letter.size() - 3)
			{
				p->isEnding = true;
			}

			i += 3;
		}
		
	}
}

bool isLetterMatchingTrie(struct ChrTrie* chr_root, string* pLetter, bool *isEnding)
{
	auto p = chr_root;

	//遍历根节点之子节点
	int n = 0;//n代表字节点序列
	int i = 0;//i代表字符串序列
	while (p->next[n] != NULL && i < pLetter->size())
	{
		//如果p的子节点有相应字符，那么往下遍历
		//buf存放pLetter中的一个字
		char buf[4] = {0};
		memcpy(buf, &(pLetter->at(i)), 3);
		if (strcmp(buf, p->next[n]->chr) == 0)
		{
			p = p->next[n];
			i += 3;
			n = 0;//子节点重新从第一个位置开始遍历
			continue;
		}
		else
		{
			n++;//横向遍历子节点
		}
	}
	
	if (i == pLetter->size())
	{
		*isEnding = p->isEnding;
		return true;
	}
	else
	{
		return false;
	}
}