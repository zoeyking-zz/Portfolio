#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <windows.h>
#include <iostream>
#include <sstream>


#define GRIDSIZE 25
#define LV1_W 10
#define LV1_H 10
#define LV1_NUM 10
#define LV2_W 15
#define LV2_H 15
#define LV2_NUM 30
#define LV3_W 20
#define LV3_H 20
#define LV3_NUM 50
#define BG_NUM 2
#define SKIN_NUM 6

typedef enum GRIDSTATE
{
	ncNULL,		//空地
	ncUNDOWN,	//背景方块
	ncMINE,		//地雷
	ncONE,
	ncTWO,
	ncTHERE,
	ncFOUR,
	ncFIVE,
	ncSIX,
	ncSEVEN,
	ncEIGHT,
	ncFLAG,
	ncQ,
	ncX,
	ncBOOMBING,
	ncUNFOUND
};
typedef enum GAMEOVERSTATE //游戏场景
{
	ncNO,
	ncWIN,
	ncLOSE
};

class BOOM
{
public:
	int mState;			//格子状态
	int mStateBackUp;	//格子状态备份
	bool isPress;		//格子是否被点击
};

using namespace sf;
using namespace std;

class Game
{
public:
	Game();
	~Game();

	RenderWindow window;

	int Window_width, Window_height;
	int Stage_width, Stage_height;
	int mMineNum, mFlagCalc;
	int GameLV, mTime;
	BOOM mGameData[LV3_W][LV3_H];
	Vector2i mCornPoint;//游戏雷区左上顶点坐标
	int GridSize;
	bool GameOver, GameQuit;
	bool GameStart;
	int GameOverState;
	Vector2i RL_Point;
	bool RL_ClkJudge_flag;
	int Mouse_RL_ClkReady;
	int imgBGNo, imgSkinNo;

	Texture tBackground, tTiles, tButtons, tNum, tTimer, tCounter, tGameOver;
	Sprite spBackground, spTiles, spButtons, spNum, spTimer, spCounter, spGameOver;
	IntRect ButtonRectEasy, ButtonRectNormal, ButtonRectHard, ButtonRectBG,
		ButtonRectSkin, ButtonRectRestart, ButtonRectQuit;

	SoundBuffer sbWin, sbBoom;
	Sound SoundWin, SoundBoom;
	Music bkMusic;

	Clock MouseClickTimer, GameClock;

	void Run();

	void Initial();
	void InitGameData();
	void LoadMediaData();
	void MineSet(int Py, int Px);

	void Input();
	void RButtonDown(Vector2i mPoint);//鼠标左击
	void LButtonDown(Vector2i mPoint);
	void LButtonDoubleClick(Vector2i mPiont);
	void RL_ButtonDown(Vector2i mPoint);
	void RL_ClkJudge();

	void Logic();
	void isWin();
	void unCover();
	void NullClick(int j, int i);

	void Draw();
	void DrawGrid();
	void DrawButton();
	void DrawScore();
	void DrawTimer();
	void DrawGameEnd();

};
