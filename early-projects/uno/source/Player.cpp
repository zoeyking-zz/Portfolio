#include "Player.h"

Player::Player()
{

	Init();
	InitAblbCards();
}
Player::~Player()
{
	
}

//初始化now_Card
void Player::Init()
{
	now_Card.cColor = cBLANKCOLOR;
	now_Card.cType = cBLANKNUM;
	now_Card.cNum = -1;
}

//初始化AbleCard[30]数组
void Player::InitAblbCards()
{	
	for (int i = 0; i < 30; i++)
	{
		AbleCards[i].cColor = cBLANKCOLOR;
		AbleCards[i].cType = cBLANKNUM;
		AbleCards[i].cScore = cBLANKSCORE;
		AbleCards[i].weight = 0;	// 因为选取特殊值最小的牌打出，所以outweight的初始状态为一个较大的值
	}
}

//重置AbleCard[30]数组
void Player::ReSetAbleCards()
{
	for (int i = 0; i < 30; i++)
	{
		AbleCards[i].cColor = cBLANKCOLOR;
		AbleCards[i].cType = cBLANKNUM;
		AbleCards[i].cScore = cBLANKSCORE;
		AbleCards[i].weight = 0;	// 因为选取特殊值最小的牌打出，所以outweight的初始状态为一个较大的值
	}
}

//初始化玩家手牌链表，手牌数量置为 0
int Player::InitHandCardList()
{
	PlayerCards = new CardNode;
	PlayerCards->next = NULL;

	for (int i = 0; i < 30; i++)
		able[i] = -1;

	return 1;
}

//按照序号顺序，将序号为 cardNum 的卡牌插入手牌链表(用于抽牌和发牌)
int Player::ListInsert(int cardNum)
{
	CardList p, card;	//p为临时结点， card为要插入的卡牌
	card = new CardNode;
	card->cardNum = cardNum;
	p = PlayerCards;	//头结点

	//如果PlayerCards还没有一张牌，则插入头结点后
	if (p->next == NULL)
	{
		card->next = NULL;
		PlayerCards->next = card;
	}
	else//否则将card按照cardNum插入手牌链表中
	{
		while (p && p->next)
		{
			if (card->cardNum > p->cardNum && card->cardNum < p->next->cardNum)
			{
				card->next = p->next;
				p->next = card;
				break;
			}
			p = p->next;
		}
		//如果循环结束是因为没有p->next == NULL，card->cardNum是最大值，将card插入尾部
		if (p->next == NULL)
		{
			card->next = NULL;
			p->next = card;
		}
	}
	return 1;
}

//定位（第cardOrder张牌）在链表中的顺序，返回它的序号，失败则返回-1
int Player::ListLocate(int cardOrder)
{
	int i = 0;
	CardList p;
	p = PlayerCards;

	//查找第cardOrder个节点，p指向该结点
	while ((p->next) && (i < cardOrder))
	{
		p = p->next;
		i++;
	}
	//当i超过链表长度或 cardOrder <1时，寻找位置不合理
	if (p == NULL || (i > cardOrder))
	{
		return -1;
	}
	else return p->cardNum;
}

//按值查找手牌链表里是否有序号为 cardNum1 到 cardNum2 的卡牌，成功返回该牌在链表中的顺序，失败返回0
int Player::ListSearch(int cardNum1, int cardNum2)
{
	int i = 0;
	CardList p;
	p = PlayerCards;

	//当前的p->cardNum不满足在 cardNum1 到 cardNum2之间，则循环继续
	while (p && (p->cardNum < cardNum1 || p->cardNum > cardNum2))
	{
		p = p->next;
		i++;
	}

	//判断是否成功
	if (p) return i;
	else return 0;
}

//按值查找手牌链表里是否有序号为 cardnum1 到 cardnum2 的卡牌，找到将该牌在Cards[]中的序号存入able数组第i个位置，i为在手牌中的顺序
int Player::ListSearch2(int cardNum1, int cardNum2)
{

	int i = 0;
	CardList p;
	p = PlayerCards;
	bool notEmpty = false;

	//当前的p->cardNum不满足在 cardnum1 到 cardnum2之间，则循环继续
	while (p)
	{
		if (p->cardNum >= cardNum1 && p->cardNum <= cardNum2)
		{
			able[i] = p->cardNum;
			notEmpty = true;
		}
		p = p->next;
		i++;
	}
	if (notEmpty) return 1;
	else return 0;
}

//删除手牌链表中的第 cardOrder 张卡牌
int Player::ListDelete(int cardOrder)
{
	int i = 0;
	CardList p, q;	//两个临时结点
	p = PlayerCards;
	//查找第 cardOrder - 1个节点，p指向该结点
	while ((p->next) && (i < cardOrder - 1))
	{
		p = p->next;
		i++;
	}
	//当i超过链表长度或cardOrder < 1时，删除位置不合理
	if (!(p->next) || i > cardOrder - 1)
	{
		return 0;
	}
	//删除q结点，即第cardOrder张手牌
	q = p->next;
	p->next = q->next;
	delete q;
	return 1;
}

//遍历链表长度
int Player::ListCount()
{
	int i = 0;
	CardList p;
	p = PlayerCards;
	//p的后一结点不是Null
	while (p->next != NULL)
	{
		p = p->next;
		i++;
	}
	return i;
}

//统计玩家的手牌中各颜色数量、MaxColor，MinColor
void Player::PD_DecdieColor_CountCard()
{
	int i = 0;
	CardList p;
	p = PlayerCards;

	while (p)
	{
		if (p->cardNum >= 8 && p->cardNum <= 32)
			B++;
		else if (p->cardNum >= 33 && p->cardNum <= 57)
			R++;
		else if (p->cardNum >= 58 && p->cardNum <= 82)
			G++;
		else if (p->cardNum >= 83 && p->cardNum <= 107)
			Y++;
		p = p->next;
		i++;
	}

	MaxColor = cRED;
	if (max(max(R, B), max(G, Y)) == B)
		MaxColor = cBLUE;
	else if (max(max(R, B), max(G, Y)) == G)
		MaxColor = cGREEN;
	else if (max(max(R, B), max(G, Y)) == Y)
		MaxColor = cYELLOW;

	MinColor = cRED;
	if (min(min(R, B), min(G, Y)) == B)
		MinColor = cBLUE;
	else if (min(min(R, B), min(G, Y)) == G)
		MinColor = cGREEN;
	else if (min(min(R, B), min(G, Y)) == Y)
		MinColor = cYELLOW;
}


int Player::ScorePoint(CardInfo Cards[])
{
	int i = 0;
	CardList p;
	p = PlayerCards;
	while (p->next != NULL)
	{
		p = p->next;
		i = i + Cards[p->cardNum].cScore;
	}

	return i;
}

