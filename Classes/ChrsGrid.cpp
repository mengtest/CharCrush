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

	//�õ����ʼ���
	auto sharedFile = FileUtils::getInstance();
	m_Letters = sharedFile->getValueVectorFromFile(letterslist);

	//��ʼ�����ּ���
	initChrBox();
	
	//���ɲ���
	//�������г�ʼ��һ���յĺ��ֺ��Ӵ�С
	m_row = row;
	m_col = col;
	m_canCrush = false;
	m_SelectedChrs.clear();
	m_NewChrs.clear();
	m_ChrsBox.resize(m_row);
	for (auto &vec : m_ChrsBox) { vec.resize(m_col); }

	//1.���ݲ��ִ�С��������������
	//2.�������������½�Ϊԭ�㣬x��y��Ϊ������
	for (int x = 0; x < m_col; x++)
	{
		for (int y = 0; y < m_row; y++)
		{
			m_ChrsBox[x][y] = createAChr(x, y); 
		}
	}

	//���봥������
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
	//�������������ת��Ϊģ������
	auto pos = this->convertToNodeSpace(pTouch->getLocation());

	//�õ���������
	int x = pos.x / GRID_WIDTH;
	int y = pos.y / GRID_WIDTH;

	//�õ�����ԭ��ģ������
	auto chr_pos = Vec2(x * GRID_WIDTH, y * GRID_WIDTH);

	//��������ԭ��ľ���
	auto d = abs(pos.distance(chr_pos));
	
	//�Ƿ��а��ں�����
	if (Rect(0, 0, m_col*GRID_WIDTH, m_row*GRID_WIDTH).containsPoint(pos) && d < CHR_EDGE)
	{
		//�õ���������
		int x = pos.x / GRID_WIDTH;
		int y = pos.y / GRID_WIDTH;

		//�õ���ǰѡ�еĺ���Ԫ�أ�������ѡ����ɫ
		auto chr = m_ChrsBox[x][y];
		chr->getBg()->setTexture("char_bg_selected.png");

		//������ʱѡ�����ּ���
		m_SelectedChrs.pushBack(chr);

		//�õ��ܷ�������״̬
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
  *����˵�����ƶ������������������º���Ԫ�طŽ���ѡ���ֺ���
  *����ǵ��ˣ���ô�����һ������Ԫ�ش���ѡ���ֺ�����ɾ��
  *ÿһ������/ɾ������Ԫ�أ������жϵ�ǰ��ѡ�ĺ����Ƿ�������
  */
void ChrsGrid::onTouchMoved(Touch* pTouch, Event*)
{
	//�ƶ�ʱҲ��ѡ��
	//�������������ת��Ϊģ������
	auto pos = this->convertToNodeSpace(pTouch->getLocation());

	//�õ���������
	int x = pos.x / GRID_WIDTH;
	int y = pos.y / GRID_WIDTH;

	//�õ�����ԭ��ģ������
	auto chr_pos = Vec2(x * GRID_WIDTH, y * GRID_WIDTH);

	//��������ԭ��ľ���
	auto d = abs(pos.distance(chr_pos));

	//����ѡ�ĺϷ����ּ�����ʱ��ѡ���ֺ���
	if (Rect(0, 0, m_col*GRID_WIDTH, m_row*GRID_WIDTH).containsPoint(pos) && d < CHR_EDGE)
	{
		//�õ���ǰ������ĺ���Ԫ�أ��Լ����һ��ѡ��ĺ���
		auto chr = m_ChrsBox[x][y];
		auto last_chr = m_SelectedChrs.back();
		
		//�жϵ�ǰ������ĺ����Ƿ������һ��ѡ�������
		int dx = abs(chr->getX() - last_chr->getX());
		int dy = abs(chr->getY() - last_chr->getY());
		int d = dx + dy;
		if (dx < 2 && dy < 2 && d <= 2 && d > 0)
		{
			//��������������ô���������ʱѡ���ֺ��ӣ�������ѡ����ɫ
			//ֻ�е���ʱѡ���ּ�����û�иú���Ԫ��ʱ�������
			if (!m_SelectedChrs.contains(chr))
			{
				m_SelectedChrs.pushBack(chr);
				chr->getBg()->setTexture("char_bg_selected.png");
				
				//�õ��ܷ�������״̬
				m_canCrush = canCrush();
			}
		}

		//�����ǰ����������ѡ���ֺ����е����ڶ������֣�˵���Ǻ��˲���
		//��������һ��Ԫ��ɾ������ѡ���ֺ���
		if (m_SelectedChrs.size() >= 2)
		{
			//�õ������ڶ���Ԫ�أ��ж��Ƿ�ʹ������Ԫ��һ��
			auto secondlast_chr = m_SelectedChrs.at(m_SelectedChrs.size()-2);
			if (secondlast_chr == chr)
			{
				//�����һ��Ԫ��ɾ����ȥ
				m_SelectedChrs.back()->getBg()->setTexture("char_bg_normal.png");
				m_SelectedChrs.popBack();

				//�õ��ܷ�������״̬
				m_canCrush = canCrush();
			}
		}
	}
}

bool ChrsGrid::canCrush()
{
	//����ʱ��ѡ�������һ���ַ���
	char selected_str[1000] = {0};
	for (auto &chr : m_SelectedChrs)
	{
		strcat(selected_str, chr->getString().getCString());
	}

	//�������ʼ��ϣ����������һ���Ǻϣ���ô��������
	for (auto &val : m_Letters)
	{
		if (strcmp(selected_str, val.asString().c_str()) == 0)
		{
			//������ɫ���óɿ�����
			for (auto &chr : m_SelectedChrs)
			{
				chr->getBg()->setTexture("char_bg_crush.png");
			}
			return true;
		}
	}

	//�������Ҳû�У���ô˵����������
	//������ɫ���ó�ѡ��
	for (auto &chr : m_SelectedChrs)
	{
		chr->getBg()->setTexture("char_bg_selected.png");
	}
	return false;
}

void ChrsGrid::onTouchEnded(Touch*, Event*)
{
	//�������������ô�����ѡ����
	if (m_canCrush)
	{
		//������ͣ����
		_eventDispatcher->pauseEventListenersForTarget(this);

		for (auto &chr : m_SelectedChrs)
		{
			//����ú��֣�Ȼ�����һ���º��ֵ��º��ֺ���
			clearSelectedChr(chr);
		}

		//ʹ���ֵ��䣬ͬʱ��������״̬��׽����
		dropChrs();
		schedule(schedule_selector(ChrsGrid::onChrsDropping), 0.1);
	}
	else
	{
		//������ܣ��ı�ر�����ɫ
		for (auto &chr : m_SelectedChrs)
		{
			chr->getBg()->setTexture("char_bg_normal.png");
		}
	}
	
	//�����ʱ��ѡ���ּ���
	m_SelectedChrs.clear();
}

//���ֵ���״̬��׽���������º��ֵ����꣬��������º��ֺ���ʱ���������
void ChrsGrid::onChrsDropping(float dt) 
{
	//����º��ֶ������ֹ꣬ͣ�ò�׽���������ָ�����
	if (m_NewChrs.empty())
	{
		unschedule(schedule_selector(ChrsGrid::onChrsDropping));
		_eventDispatcher->resumeEventListenersForTarget(this);
	}
}
 
void ChrsGrid::clearSelectedChr(Chr* chr)
{
	//�����ֺ����еĶ�ӦԪ����Ϊ��
	m_ChrsBox[chr->getX()][chr->getY()] = nullptr;

	//����һ���µĺ���Ԫ�أ����������ʱ�º��ֺ���
	//���������ĺ���ͬ�У���λ������������һ�е���һ��
	auto new_chr = createAChr(chr->getX(), m_row);
	m_NewChrs.pushBack(new_chr);

	//������Ԫ�شӸ��ڵ��Ƴ�
	chr->removeFromParent();
}

void ChrsGrid::dropChrs()
{
	for (int x = 0; x < m_col; x++)
	{
		auto pChrsBox = &m_ChrsBox;

		int space = 0;//��¼��ǰ�еĿո���
		for (int y = 0; y < m_row; y++)
		{
			auto chr = m_ChrsBox[x][y];

			if (!chr)
			{
				space++;
				continue;
			}
			else if (space != 0)//������г����˿ո���ôӦ�õ���
			{
				chr->setY(chr->getY() - space);//����������yֵ

				auto move = MoveBy::create(0.2, Vec2(0, -space*GRID_WIDTH));
				auto call = CallFunc::create([pChrsBox, chr](){
					//���º��ֺ����ڵ�����
					(*pChrsBox)[chr->getX()][chr->getY()] = chr;
				});

				chr->runAction(Sequence::create(move, call, nullptr));//׼������Ķ���
			}
		}

		//����������У�space��Ϊ0����ô���д����º��֣����º��ֵ���
		if (space == 0)
		{
			continue;
		}

		int n = space;
		int delta = 1;//�趨�ٶȽ��ݵĸ�������
		for (auto &chr : m_NewChrs)
		{
			if (chr->getX() == x)
			{
				chr->setY(m_row - n);

				auto delay = DelayTime::create(0.2);
				//��������ٶ�������һЩ
				auto move = MoveBy::create(0.2*delta++, Vec2(0, -n--*GRID_WIDTH));
				auto call = CallFunc::create([chr, pChrsBox, this](){
					(*pChrsBox)[chr->getX()][chr->getY()] = chr;
					//���º��ֺ������Ƴ��ú��֣�Ҳ����˵�����º��ֺ���Ϊ��ʱ����������
					m_NewChrs.eraseObject(chr);
				});

				chr->runAction(Sequence::create(delay, move, call, nullptr));
			}
		}
	}
}

Chr* ChrsGrid::createAChr(int x, int y)
{
	//1.���ݲ������괴��һ������Ԫ�أ��������ݸ��ݺ��ּ���������
	//2.���øú���Ԫ�ص���������
	//3.���ú��ּ�����Ⱦ�ڵ�
	Chr* chr = nullptr;

	int i = random(0, (int)m_Chrs.size()-1);
	auto s = m_Chrs.at(i);
	chr = Chr::createWithString(s, x, y);

	chr->setPosition(x * GRID_WIDTH, y * GRID_WIDTH);
	addChild(chr);

	//log("add a chr!---content:%s---x:%d---y:%d", chr->getString().getCString(), x, y);

	return chr;
}

//�ж����Ƿ����ּ�����
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

//�ӵ����б��ļ��У���ȡ�����֣��ŵ����ּ�����
void ChrsGrid::initChrBox()
{
	m_Chrs.clear();

	//�ӵ��ʼ����У���ѡ�����ظ����֣������ּ�����
	for (auto &val : m_Letters)
	{
		//letter����ÿһ������
		auto letter = val.asString();

		//����ÿһ�����ʵĺ��֣����ַ����ּ���
		int i = 0;
		while (i < letter.size())
		{
			//ÿһ������ռ��3���ֽڣ��������Ŀ��ַ������ĸ��ֽ����һ��String
			char buf[4] = {0};
			memcpy(buf, &letter.at(i), 3);
			auto letter = String::createWithFormat("%s", buf);

			//�жϸõ����Ƿ����ּ��������ظ������ظ��ż����ּ���
			if (!isChrExist(&m_Chrs, letter))
			{
				m_Chrs.pushBack(letter);
			}

			i += 3;
		}
	}
}