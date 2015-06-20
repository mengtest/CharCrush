#ifndef __LETTERSGRID_H__
#define __LETTERSGRID_H__

#include "cocos2d.h"
#include <vector>

USING_NS_CC;
using namespace std;

#define GRID_WIDTH 70

class Chr;
class GameScene;

//此为字典树的一个节点类型，用于构成字典树
#define MAX 20 //一个节点最多有20个子节点，实质上代表单词首字数量
struct ChrTrie
{
	ChrTrie* next[MAX];	//此为该节点之子节点数组
	char chr[4];		//该节点的汉字内容
	bool isEnding;		//单词结尾标记，true为结尾
};

void createTrie(struct ChrTrie* chr_root, ValueVector* letters);//根据单词集合生成一颗字典树
void deleteTrie(struct ChrTrie* chr_root); //释放字典树
bool isChrExist(String* chr, struct ChrTrie *p, int *n);//判断该字是否存在于p的子节点中
bool isLetterMatchingTrie(struct ChrTrie* chr_root, string* pLetter, bool *isEnding);//判断letter是否在字典树中匹配

class ChrsGrid : public Node
{
public:
	//根据letter.plist文件（单词集合），创建字集合，以及行列
	static ChrsGrid* create(ValueMap level_info, int col, int row);
	bool init(ValueMap level_info, int col, int row);

	void onExit();

private:
	bool onTouchBegan(Touch*, Event*);
	void onTouchMoved(Touch*, Event*);
	void onTouchEnded(Touch*, Event*);
	void onTouchCancelled(Touch*, Event*);

	void onChrsDropping(float dt);//汉字掉落状态捕捉函数
	void onChrsCrushing(float dt);//消除状态捕捉函数
	void onCountdownCallBack(float dt);//倒计时捕捉函数，每一秒钟调用一次，为0开始提示

private:
	Chr* createAChr(int x, int y); //根据布局坐标创建一个汉字元素
	bool isChrExist(Vector<String*>*, String*);//判断字是否在字集合中
	void initChrBox();//初始化汉字集合

	bool canCrush();	//判断当前已选汉字能否消除
	void goCrush();		//消除模块，将在停止触摸时启用
	void crushLastChr();//对已选汉字盒子内最后一个元素做特殊处理
	void addNewChrs();	//增加新汉字元素模块，将在消除模块完成后启用
	void dropChrs();	//汉字掉落
	bool isDeadMap();	//判断当前阵列是否无法消除
	bool findRoot(Chr*);//以chr为结点，将其添加到m_AnswerChrs，判断是否为字典树中之终结
	string getStringFromChrs(Vector<Chr*>*);//从汉字盒子中获取字符串

	void resetCountdown();	//重置倒计时时间
	void resetAnswerChrs();	//重置系统提示汉字盒子
	void showAnswer();		//系统演示消除方法
	GameScene* getGameScene() { return (GameScene*)(this->getParent()->getParent()); }//得到包含该阵列的游戏场景
	int getSpecial(int i);	//根据i从m_selected中获得special类型

public:
	vector<vector<Chr*>>* getChrsBox() { return &m_ChrsBox; } //得到汉字阵列
	int getCol() { return m_col; }
	int getRow() { return m_row; }

private:
	int m_row;//行
	int m_col;//列

	bool m_canCrush;//标记当前已选汉字能否消除
	int m_countdown;//提示倒计时，倒计时结束（0），则系统根据提示盒子进行一次提示

	Vector<String*> m_Chrs;	//汉字集合
	struct ChrTrie chr_root;//汉字字典树
	ValueVector m_Letters;	//单词集合
	ValueMap m_letter_info; //关卡配置信息

	vector<vector<Chr*>> m_ChrsBox;	//汉字盒子：存放当前布局中的汉字元素
	Vector<Chr*> m_SelectedChrs;	//存放已选择的汉字集合
	Vector<Chr*> m_NewChrs;			//存放欲加入汉字阵列的汉字元素
	Vector<Chr*> m_AnswerChrs;		//存放系统提示的当前可消除的汉字序列
};

#endif

