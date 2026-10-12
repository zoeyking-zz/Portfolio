#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

//卡牌尺寸
#define CARD_BACK_SCALE 1
#define CARD_BACK_ROTATION 90
#define CARD_BACK_W 100
#define CARD_BACK_H 200

//玩家卡牌的坐标
#define  P1_CARD_CORNER_X      170
#define  P1_CARD_CORNER_Y      650
#define  P2_CARD_CORNER_X      20
#define  P2_CARD_CORNER_Y      350
#define  P3_CARD_CORNER_X      540
#define  P3_CARD_CORNER_Y      30
#define  P4_CARD_CORNER_X      1260
#define  P4_CARD_CORNER_Y      400

//各玩家出牌位置
#define  CARDOUT_CORNER_X1   550
#define  CARDOUT_CORNER_Y1   400
#define  CARDOUT_CORNER_X2   200
#define  CARDOUT_CORNER_Y2   300
#define  CARDOUT_CORNER_X3   600
#define  CARDOUT_CORNER_Y3   150
#define  CARDOUT_CORNER_X4   950
#define  CARDOUT_CORNER_Y4   300

//每个玩家最多持有13张牌
#define MAXCARDS 30

#define MAX_CARD_LINE	3	//Player1手牌最多展示几排
#define MAX_CARD_CINE	10	//Player1手牌最多展示几列


//牌的颜色，枚举
enum CARDCOLOR		
{
	cBLACK,					//黑色
	cBLUE,					//蓝色
	cRED,					//红色
	cGREEN,					//绿色
	cYELLOW,				//黄色
	cBLANKCOLOR
};
//牌的内容，枚举
enum CARDTYPE
{
	cZERO,					//数字牌：0
	cONE,					//数字牌：1
	cTWO,					//数字牌：2
	cTHREE,					//数字牌：3
	cFOUR,					//数字牌：4
	cFIVE,					//数字牌：5
	cSIX,					//数字牌：6
	cSEVEN,					//数字牌：7
	cEIGHT,					//数字牌：8
	cNINE,					//数字牌：9
	cPlus2,					//功能牌：罚牌两张		20分
	cSkip,					//功能牌：跳过			20分
	cReverse,				//功能牌：转向			20分
	cPlus4 = 0,					//功能牌：换色+罚牌四张，在素材中是第一张，所以令其值为0	40分
	cChangeColor = 1,			//功能牌：换色，在素材中是第二张，所以令其值为1				40分
	cBLANKNUM = 20,
};
//牌的分值，枚举
enum CARDSCORE
{
	cZERO_S = 0,			//数字牌：0		0分
	cONE_S,					//数字牌：1		1分
	cTWO_S,					//数字牌：2		2分
	cTHREE_S,				//数字牌：3		3分
	cFOUR_S,				//数字牌：4		4分
	cFIVE_S,				//数字牌：5		5分
	cSIX_S,					//数字牌：6		6分
	cSEVEN_S,				//数字牌：7		7分
	cEIGHT_S,				//数字牌：8		8分
	cNINE_S,				//数字牌：9		9分
	cTWENTY_S = 20,			//跳过 & 转向 & 罚牌两张	20分
	cFOURTY_S = 40,			//换色 & 罚牌4张	40分
	cBLANKSCORE,
};

//结构体，记录每张牌的颜色和内容
struct CardInfo				
{
	CARDCOLOR cColor;
	CARDTYPE cType;
	CARDSCORE cScore;
	int cNum;
	int weight;
};

class Card
{
public:
	
};