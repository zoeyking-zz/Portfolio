#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <windows.h>
#include <math.h>
#include <iostream>
#include <sstream>
#include "Player.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 960

#define STARTBTN_W 243
#define STARTBTN_H 150
#define STARTRULE_W 243
#define STARTRULE_H 138
#define BACKBTN_W 240
#define BACKBTN_H 200

#define  BUTTON1_CORNER_X	1050
#define  BUTTON1_CORNER_Y	670
#define  BUTTON1_DETA_X		115
#define  BUTTON1_DETA_Y		80

#define  BUTTON2_CORNER_X	20
#define  BUTTON2_CORNER_Y	20
#define  BUTTON2_DETA_X		130

#define  BUTTON3_CORNER_X	540
#define  BUTTON3_CORNER_Y	380

//Buttons1:红框功能键
#define BUTTON_W1 100
#define BUTTON_H1 60
//Buttons2:方形背景、音乐、设置按钮（可以优化多边形按钮
#define BUTTON_W2 100
#define BUTTON_H2 100
//Button3:切换颜色
#define BUTTON_W3 200
#define BUTTON_H3 200
//AI按键
#define BTN_AI_W 110
#define BTN_AI_H 50
//头像尺寸
#define PLAYERBOX_SCALE 0.5
#define PLAYERBOX_W 200
#define PLAYERBOX_H 200
//玩家头像的坐标
#define  P1_PHOTO_CORNER_X      30		//down
#define  P1_PHOTO_CORNER_Y      680
#define  P3_PHOTO_CORNER_X      980		//up
#define  P3_PHOTO_CORNER_Y      30
#define  P2_PHOTO_CORNER_X      30		//left
#define  P2_PHOTO_CORNER_Y      180
#define  P4_PHOTO_CORNER_X      1160	//right
#define  P4_PHOTO_CORNER_Y      480

#define  CARDOUT_CORNER_X   590
#define  CARDOUT_CORNER_Y   220
#define CHANGECLOLR_OFFSET_X -100//改色牌坐标偏移修正
#define CHANGECLOLR_OFFSET_Y -45//改色牌坐标偏移修正

#define  GAMEOVER_CORNER_X  440
#define  GAMEOVER_CORNER_Y  430

//改变颜色牌
#define CHANGECOLOR_X 490
#define CHANGECOLOR_Y 220

using namespace sf;
using namespace std;


enum GameState		//游戏场景
{
	cStart,
	cRule,	//设置
	cPlaying,
	cGameOver,
};
enum MouseState		//鼠标状态，枚举
{
	cNoPressed,
	cLPressed,
	cRPressed,
};


class UNOGAME
{
public:
	RenderWindow window;
	UNOGAME();
	~UNOGAME();

	int BG, musicNo, CardOut_i;

	//开始场景文字按钮区域
	Texture tStartBtn, tStartRule;
	Sprite spStartBtn, spStartRule;
	IntRect intStartBtn, intStartRule;
	//菜单场景文字按钮区域
	Texture tBack, tRuleBG;
	Sprite spBack, spRuleBG;
	IntRect intBack;
	//游戏场景文字按钮区域
	Texture tBackground, tCardCount, tCounter, tGameOver, tGameStart, tRuleCH, tRuleEN,
		tPlayer, tCardBack, tCard, tButton, tButtonAI, tRuleHook, tChangeColor, tPlus4Color;
	Sprite spBackground, spCardCount, spCounter, spGameOver, spGameStart, spRuleCH, spRuleEN,
		spPlayer, spCardBack, spCard, spButton,spButtonAI, spRuleHook, spChangeColor, spPlus4Color;
	IntRect BtnNoCards, BtnHandDown, BtnPlay, BtnUno, BtnQuit, BtnRestart, 
		BtnChooseBG, BtnChooseMusic, BtnChooseColor, BtnSettings, BtnAIStart,BtnAIPause;
	bool BtnNoCardsState, BtnHandDownState, BtnPlayState, BtnUnoState, BtnChooseColorState;
	bool isNoCardsPressed, isHandDownPressed, isPlayPressed, isUnoPressed, isChooseColorPressed, isSettingsPressed;
	//声音对象
	SoundBuffer sbWin, sbLose, sbDraw, sbPlay, sbFollow, sbReverse, sbSkip, sbUno, sbHandDown;
	Sound soundWin, soundLose, soundDraw, soundPlay, soundFollow, soundReverse, soundSkip, soundUno, soundHandDown;
	Music bgMusic;


	bool GameOver, GameQuit, GameStart;
	//bool GameStart;
	//玩家AI是否开始
	bool AI_start;
	Clock gameClock;							//AI出牌的延时
	Player Player1, Player2, Player3, Player4;	//玩家
	//建立树类
	Tree *AI2Tree;
	
	//180张卡牌信息（卡牌颜色和卡牌类型）
	CardInfo Cards[108];	
	//牌堆里的牌是否被取走
	bool isCardTaken[108];	
	
	//记录当前牌的颜色
	CardInfo nowCard, lastCard;
	int nowcardNum, lastcardNum;
	bool isClockWise;

	//记录当前场景、鼠标状态
	GameState nowStage, LastStage;
	MouseState nowMouse;
	Vector2i p_mouse;		//记录现在鼠标坐标(并不实时

	int PlusCard;	//加牌叠加计数
	bool PlusCardStart;	//是否开始计数


	void Initial();
	void InitialData();
	void InitialPlayerData();	//初始化玩家信息
	void InitialCardsData();	//初始化卡牌信息
	void InitialMeida();
	void Init_now_Card(Player *player);//传址,每次出牌前改变player.now_Card
	

	void Input();

	void Logic();
	void logic();
	//随机发牌函数，生成不重复的随机数，调用链表的前插函数（cardCount为发牌数量）
	void DealCards(Player Player, int cardCount);	
	//人类出牌，读取isCardChosen数组，获取要出什么牌，调用链表的删除函数
	bool HumanPlayCard();	
	//顺序AI算法:遍历，找到第一张能出的牌出掉
	bool AIPlayCard(Player *player);
	//检测玩家/AI是否能出牌，返回牌在手牌链表中的序号
	int  CheckCardDemand(Player player);		
	//判断是否点击了卡牌区域
	void isCardPressed(Vector2i mPoint);
	//判断是否点击了按钮区域
	void isButtonPressed(Vector2i mPoint);	

	//检测玩家是否能出牌，能出的牌放在AnbleCards[30]
	void  CheckCardDemand1_PD(Player *player);	
	//依据自己手牌情况，调整AbleCard.weight
	void myCards_value(Player *player);
	//依据各玩家手牌情况，调整AbleCard.weight
	void othersChange_value(CardInfo& AbleCard);
	//通过树寻找多步计算后最佳的出牌;round为几回合（一回合四个玩家出牌四层树深）
	void othersChange_value2(CardInfo &AbleCard, int round);
	//遍历所有能出的牌，出特殊值最小的牌;AIPlan为第几种AI决策算法
	int PD_DecdieCard(Player *player, int AIPlan);
	//玩家1把颜色换成自己多别人少的颜色
	int PD_DecdieColor();
	//AI算法1：别人少自己多的牌优先+同色普通>同色功能>同号>万能牌+换自己多别人少的颜色
	//AI算法2：多步（多个回合）计算后最优出牌+算法1
	bool AIPlayCard_PD(Player *player, int AIPlan);
	int Tree_DecdieCard(Player *player);
	

	
	

	void isGameOver();
	void Score();

	void Draw();
	void DrawStart();
	void DrawButton();
	void DrawPlayerInfo();
	void DrawCard();
	void DrawRule();
	void DrawGameOver();
	void DrawCardOut();
	void Draw_playerCard(Player player);//每个玩家各自的出牌绘制
	void DrawCardCount();

	void Run();
};

