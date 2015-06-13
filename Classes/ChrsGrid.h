#ifndef __LETTERSGRID_H__
#define __LETTERSGRID_H__

#include "cocos2d.h"
#include <vector>

USING_NS_CC;
using namespace std;

#define GRID_WIDTH 60

class Chr;

//此为字典树的一个节点类型，用于构成字典树
#define MAX 20 //一个节点最多有20个子节点，实质上代表单词首字数量
struct ChrTrie
{
    ChrTrie* next[MAX];//此为该节点之子节点数组
    char chr[4];//该节点的汉字内容
	bool isEnding;//单词结尾标记，true为结尾
};

void createTrie(struct ChrTrie* chr_root, ValueVector* letters);//根据单词集合生成一颗字典树
bool isChrExist(String* chr, struct ChrTrie *p, int *n);//判断该字是否存在于p的子节点中
bool isLetterMatchingTrie(struct ChrTrie* chr_root, string* pLetter, bool *isEnding);//判断letter是否在字典树中匹配

class ChrsGrid : public Node
{
public:
	//根据letter.plist文件（单词集合），创建字集合，以及行、列
	static ChrsGrid* create(const char* letterlist, int row, int col);
	bool init(const char* letterslist, int row, int col);

private:
	bool isChrExist(Vector<String*>*, String*);//判断字是否在字集合中
	void initChrBox();//初始化汉字集合

	Chr* createAChr(int x, int y); //根据布局坐标创建一个汉字元素

	bool canCrush(); //判断当前已选汉字能否消除
	void clearSelectedChr(Chr*); //清空已选汉字
	void dropChrs(); //汉字掉落
	bool isDeadMap();  //判断当前阵列是否无法消除
	bool findRoot(Chr*, string* letter);//以chr为结点，判断letter是否为字典树中之终结

private:
	bool onTouchBegan(Touch*, Event*);
	void onTouchMoved(Touch*, Event*);
	void onTouchEnded(Touch*, Event*);

	void onChrsDropping(float dt);//汉字掉落状态捕捉函数

private:
	int m_row;//行
	int m_col;//列

	bool m_canCrush;//标记当前已选汉字能否消除

private:
	Vector<String*> m_Chrs;//汉字集合
	struct ChrTrie chr_root;//汉字字典树
	ValueVector m_Letters;//单词集合

	vector<vector<Chr*>> m_ChrsBox;//汉字盒子：存放当前布局中的汉字元素
	Vector<Chr*> m_SelectedChrs;//存放已选择的汉字集合
	Vector<Chr*> m_NewChrs; //存放欲加入汉字阵列的汉字元素
};

#endif

