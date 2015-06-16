#ifndef __CHR_H__
#define __CHR_H__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

#define CHR_WITDH 50 //文字宽度
#define CHR_EDGE 70 //文字矩形之斜边，为了不用乘以根号2消耗计算

//特殊消除汉字之标记
#define SPECIAL_TYPE_HOR 1//横
#define SPECIAL_TYPE_VAR 2//竖
#define SPECIAL_TYPE_RBS 3//右倾斜
#define SPECIAL_TYPE_LBS 4//左倾斜
#define SPECIAL_TYPE_HVR 5//横竖
#define SPECIAL_TYPE_RLB 6//左右倾斜
#define SPECIAL_TYPE_ALL 7//全方位
#define SPECIAL_TYPE_BON 8//高分值奖励

//汉字类，由它组成词组
class Chr : public Node
{
public:
	//通过一个String创建汉字，x、y是坐标
	static Chr* createWithString(String*, int x, int y);
	bool init(String*, int x, int y);

public:
	String getString() { return m_chr; }
	Sprite* getBg() { return m_bg; }
	int getX() { return m_x; }
	int getY() { return m_y; }
	string getNormalBG(); //返回普通状态下的汉字背景

	void setX(int x) { m_x = x; }
	void setY(int y) { m_y = y; }

	Sprite** getArrow() { return m_arrow; }
	int getSpecial() { return m_special_type; }

	void showArrow(Chr* next_chr); //根据下一个汉字元素，显示箭头
	void hideArrow(); //隐藏所有的箭头

	void bomb(); //汉字元素消除
	void setSpecial(int special_type); //根据type设定其特殊类型

private:
	//x,y代表坐标
	int m_x;
	int m_y;
	int m_special_type; //特殊类型，0代表正常

	String m_chr;//汉字内容
	Sprite* m_bg;//汉字背景图
	Sprite* m_arrow[8];//箭头标志，0上，1下，2左，3右，4右上，5右下，6左上，7左下
	Sprite* m_special[8];//特殊标志，0上，1下，2左，3右，4右上，5右下，6左上，7左下
};

#endif
