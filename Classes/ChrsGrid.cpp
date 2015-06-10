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

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	log("ChrsGrid init!");

	return true;
}

bool ChrsGrid::onTouchBegan(Touch* pTouch, Event*)
{
	//�������������ת��Ϊģ������
	auto pos = this->convertToNodeSpace(pTouch->getLocation());
	
	//�Ƿ��а��ں���������
	if (Rect(0, 0, m_col*GRID_WIDTH, m_row*GRID_WIDTH).containsPoint(pos))
	{
		//�õ���������
		int x = pos.x / GRID_WIDTH;
		int y = pos.y / GRID_WIDTH;

		//�õ���ǰѡ�еĺ���Ԫ��
		auto chr = m_ChrsBox[x][y];
		chr->getBg()->setTexture("char_bg_selected.png");
		//�������������С����ͼƬ���ʺ�ɲ�����
		chr->getBg()->setScale(40 / chr->getBg()->getContentSize().width);

		log("touch coordinate: x=%d,y=%d", x, y);

		return true;
	}
	else
	{
		return false;
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