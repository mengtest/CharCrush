#ifndef __LETTERSGRID_H__
#define __LETTERSGRID_H__

#include "cocos2d.h"
#include <vector>

USING_NS_CC;
using namespace std;

#define GRID_WIDTH 70

class Chr;
class GameScene;

//��Ϊ�ֵ�����һ���ڵ����ͣ����ڹ����ֵ���
#define MAX 20 //һ���ڵ������20���ӽڵ㣬ʵ���ϴ�������������
struct ChrTrie
{
	ChrTrie* next[MAX];	//��Ϊ�ýڵ�֮�ӽڵ�����
	char chr[4];		//�ýڵ�ĺ�������
	bool isEnding;		//���ʽ�β��ǣ�trueΪ��β
};

void createTrie(struct ChrTrie* chr_root, ValueVector* letters);//���ݵ��ʼ�������һ���ֵ���
void deleteTrie(struct ChrTrie* chr_root); //�ͷ��ֵ���
bool isChrExist(String* chr, struct ChrTrie *p, int *n);//�жϸ����Ƿ������p���ӽڵ���
bool isLetterMatchingTrie(struct ChrTrie* chr_root, string* pLetter, bool *isEnding);//�ж�letter�Ƿ����ֵ�����ƥ��

class ChrsGrid : public Node
{
public:
	//����letter.plist�ļ������ʼ��ϣ��������ּ��ϣ��Լ�����
	static ChrsGrid* create(ValueMap level_info, int col, int row);
	bool init(ValueMap level_info, int col, int row);

	void onExit();

private:
	bool onTouchBegan(Touch*, Event*);
	void onTouchMoved(Touch*, Event*);
	void onTouchEnded(Touch*, Event*);
	void onTouchCancelled(Touch*, Event*);

	void onChrsDropping(float dt);//���ֵ���״̬��׽����
	void onChrsCrushing(float dt);//����״̬��׽����
	void onCountdownCallBack(float dt);//����ʱ��׽������ÿһ���ӵ���һ�Σ�Ϊ0��ʼ��ʾ

private:
	Chr* createAChr(int x, int y); //���ݲ������괴��һ������Ԫ��
	bool isChrExist(Vector<String*>*, String*);//�ж����Ƿ����ּ�����
	void initChrBox();//��ʼ�����ּ���

	bool canCrush();	//�жϵ�ǰ��ѡ�����ܷ�����
	void goCrush();		//����ģ�飬����ֹͣ����ʱ����
	void crushLastChr();//����ѡ���ֺ��������һ��Ԫ�������⴦��
	void addNewChrs();	//�����º���Ԫ��ģ�飬��������ģ����ɺ�����
	void dropChrs();	//���ֵ���
	bool isDeadMap();	//�жϵ�ǰ�����Ƿ��޷�����
	bool findRoot(Chr*);//��chrΪ��㣬������ӵ�m_AnswerChrs���ж��Ƿ�Ϊ�ֵ�����֮�ս�
	string getStringFromChrs(Vector<Chr*>*);//�Ӻ��ֺ����л�ȡ�ַ���

	void resetCountdown();	//���õ���ʱʱ��
	void resetAnswerChrs();	//����ϵͳ��ʾ���ֺ���
	void showAnswer();		//ϵͳ��ʾ��������
	GameScene* getGameScene() { return (GameScene*)(this->getParent()->getParent()); }//�õ����������е���Ϸ����
	int getSpecial(int i);	//����i��m_selected�л��special����

public:
	vector<vector<Chr*>>* getChrsBox() { return &m_ChrsBox; } //�õ���������
	int getCol() { return m_col; }
	int getRow() { return m_row; }

private:
	int m_row;//��
	int m_col;//��

	bool m_canCrush;//��ǵ�ǰ��ѡ�����ܷ�����
	int m_countdown;//��ʾ����ʱ������ʱ������0������ϵͳ������ʾ���ӽ���һ����ʾ

	Vector<String*> m_Chrs;	//���ּ���
	struct ChrTrie chr_root;//�����ֵ���
	ValueVector m_Letters;	//���ʼ���
	ValueMap m_letter_info; //�ؿ�������Ϣ

	vector<vector<Chr*>> m_ChrsBox;	//���ֺ��ӣ���ŵ�ǰ�����еĺ���Ԫ��
	Vector<Chr*> m_SelectedChrs;	//�����ѡ��ĺ��ּ���
	Vector<Chr*> m_NewChrs;			//��������뺺�����еĺ���Ԫ��
	Vector<Chr*> m_AnswerChrs;		//���ϵͳ��ʾ�ĵ�ǰ�������ĺ�������
};

#endif

