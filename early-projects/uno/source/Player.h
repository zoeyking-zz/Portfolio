#pragma once
#include "Card.h"
#include "Tree.h"
#include <SFML/Graphics.hpp>
#include <windows.h>
#include <algorithm>

using namespace std;
using namespace sf;

//玩家标记
enum rolePlayer
{
	roleNONE,				//空  
	rolePLAYER1,			//玩家 1  
	rolePLAYER2,			//玩家 2
	rolePLAYER3,			//玩家 3
	rolePLAYER4				//玩家 4
};

//玩家状态
enum isOverState
{
	cNoOver,
	cWin,
	cLose
};

//玩家和AI的手牌链表
typedef struct CardNode
{
	int cardNum;		//牌在数组中的序号
	struct CardNode* next;
}CardNode, * CardList;


class Player
{
public:
	Player();
	~Player();

	rolePlayer role;				//玩家标记
	bool isRound;					//当前回合

	CardList PlayerCards;			//玩家手牌链表
	isOverState isOver;				//结束状态

	int Score;

	bool isCardChosen[MAXCARDS];	//手牌是否被选择
	int CardCount;					//卡牌数量，用于判断游戏是否结束
	sf::IntRect P_Card[MAXCARDS];	//记录玩家手牌绘制的区域

	int able[30];
	bool drawing;
	Vector2i CardOut_pos;	//每个玩家出牌绘制位置
	CardInfo now_Card;		//每个玩家保存各自当前出牌信息
	
	//玩家能出的手牌（玩家1最多30张手牌）
	CardInfo AbleCards[30];

	int R, G, B, Y;					//各色手牌数
	int MaxColor, MinColor;			//颜色最多/最少的手牌

	//初始化player.now_Card
	void Init();	
	//初始化AbleCard[30]数组
	void InitAblbCards();
	//重置AbleCards数组
	void ReSetAbleCards();
	//初始化手卡链表，手卡数量置为 0
	int InitHandCardList();
	//按照序号顺序，将序号为 cardNum 的卡牌插入手牌链表(用于抽牌和发牌)
	int ListInsert(int cardNum);
	//定位到链表中的顺序（第cardOrder张牌），返回它的序号，失败则返回-1
	int ListLocate(int cardOrder);
	//按值查找手牌链表里是否有序号为 cardNum1 到 cardNum2 的卡牌，成功返回该牌在链表中的顺序，失败返回0
	int ListSearch(int cardNum1, int cardNum2);
	int ListSearch2(int cardNum1, int cardNum2);

	//删除手牌链表中的第 cardOrder 张卡牌
	int ListDelete(int cardOrder);
	//遍历链表长度
	int ListCount();

	//统计玩家的手牌中各颜色数量、MaxColor，MinColor
	void PD_DecdieColor_CountCard();

	//计算手牌的分数
	int ScorePoint(CardInfo Cards[]);
};
