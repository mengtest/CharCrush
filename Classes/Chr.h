#ifndef __CHR_H__
#define __CHR_H__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

#define CHR_WITDH 50 //���ֿ��
#define CHR_EDGE 70 //���־���֮б�ߣ�Ϊ�˲��ó��Ը���2���ļ���

//������������֮���
#define SPECIAL_TYPE_HOR 1//��
#define SPECIAL_TYPE_VAR 2//��
#define SPECIAL_TYPE_RBS 3//����б
#define SPECIAL_TYPE_LBS 4//����б
#define SPECIAL_TYPE_HVR 5//����
#define SPECIAL_TYPE_RLB 6//������б
#define SPECIAL_TYPE_ALL 7//ȫ��λ
#define SPECIAL_TYPE_BON 8//�߷�ֵ����

//�����࣬������ɴ���
class Chr : public Node
{
public:
	//ͨ��һ��String�������֣�x��y������
	static Chr* createWithString(String*, int x, int y);
	bool init(String*, int x, int y);

public:
	String getString() { return m_chr; }
	Sprite* getBg() { return m_bg; }
	int getX() { return m_x; }
	int getY() { return m_y; }
	string getNormalBG(); //������ͨ״̬�µĺ��ֱ���

	void setX(int x) { m_x = x; }
	void setY(int y) { m_y = y; }

	Sprite** getArrow() { return m_arrow; }
	int getSpecial() { return m_special_type; }

	void showArrow(Chr* next_chr); //������һ������Ԫ�أ���ʾ��ͷ
	void hideArrow(); //�������еļ�ͷ

	void bomb(); //����Ԫ������
	void setSpecial(int special_type); //����type�趨����������

private:
	//x,y��������
	int m_x;
	int m_y;
	int m_special_type; //�������ͣ�0��������

	String m_chr;//��������
	Sprite* m_bg;//���ֱ���ͼ
	Sprite* m_arrow[8];//��ͷ��־��0�ϣ�1�£�2��3�ң�4���ϣ�5���£�6���ϣ�7����
	Sprite* m_special[8];//�����־��0�ϣ�1�£�2��3�ң�4���ϣ�5���£�6���ϣ�7����
};

#endif
