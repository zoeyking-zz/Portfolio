#include "Game.h"

//using namespace sf;

Game::Game()
{
	Window_width = 1000;
	Window_height = 800;

	window.create(VideoMode(Window_width, Window_height), "MineSweeper");

	GameLV = 2;
	imgBGNo = 0;
	imgSkinNo = 0;

	Mouse_RL_ClkReady = 0;
	RL_ClkJudge_flag = 0;
}
Game::~Game()
{

}

void Game::Initial()
{
	window.setFramerateLimit(60);
	GameOver = 0;
	GameQuit = 0;
	GameOverState = ncNO;
	mFlagCalc = 0;
	GameStart = 0;
	GridSize = GRIDSIZE;
	mTime = 0;

	switch (GameLV)
	{
	case 1:
		Stage_width = LV1_W;
		Stage_height = LV1_H;
		mMineNum = LV1_NUM;
		break;
	case 2:
		Stage_width = LV2_W;
		Stage_height = LV2_H;
		mMineNum = LV2_NUM;
		break;
	case 3:
		Stage_width = LV3_W;
		Stage_height = LV3_H;
		mMineNum = LV3_NUM;
		break;
	default:
		break;
	}

	mCornPoint.x = (Window_width - Stage_width * GRIDSIZE) / 2;
	mCornPoint.y = (Window_height - Stage_height * GRIDSIZE) / 2;
	InitGameData();
	LoadMediaData();

}
void Game::InitGameData()
{
	int i, j;
	for(j=0;j<Stage_height;j++)
		for (i = 0; i < Stage_width; i++)
		{
			mGameData[j][i].mState = ncUNDOWN;
			mGameData[j][i].isPress = 0;
			mGameData[j][i].mStateBackUp = ncNULL;
		}
}
void Game::LoadMediaData()
{
	stringstream ss;
	ss.fill('0');
	ss << "data/images/BackGround";
	ss.width(2);
	ss<<imgBGNo << ".jpg";
	if (!tBackground.loadFromFile(ss.str()))
		cout << "BackGround" << imgBGNo << "没有找到" << endl;

	ss.str("");//清空字符串
	ss << "data/images/Game" << imgSkinNo << ".png";
	if (!tTiles.loadFromFile(ss.str()))
		cout << "Game" << imgSkinNo << "没有找到" << endl;

	if(!tNum.loadFromFile("data/images/num.jpg"))
		cout << "num.jpg 没有找到" << endl;
	if(!tTimer.loadFromFile("data/images/jishiqi.jpg"))
	cout << "jishiqi.jpg 没有找到" << endl;
	if (!tCounter.loadFromFile("data/images/jishuqi.jpg"))
		cout << "jishuqi.jpg 没有找到" << endl;
	if (!tButtons.loadFromFile("data/images/button.jpg"))
		cout << "button.jpg 没有找到" << endl;
	if (!tGameOver.loadFromFile("data/images/gameover.jpg"))
		cout << "gameover.jpg 没有找到" << endl;

	spBackground.setTexture(tBackground);
	spTiles.setTexture(tTiles);
	spButtons.setTexture(tButtons);
	spNum.setTexture(tNum);
	spTimer.setTexture(tTimer);
	spCounter.setTexture(tCounter);
	spGameOver.setTexture(tGameOver);
}
void Game::MineSet(int Py, int Px)
{
	
	int mCount, i, j, k, l;
	mCount = 0;
	srand(time(NULL));

	do
	{
		k = rand() % Stage_height;
		l = rand() % Stage_width;
		if ((k - Py) * (k - Py) + (l - Px) * (l - Px) <= 2)
			continue;
		if ( mGameData[k][l].mState == ncUNDOWN)//第一键插了旗的格子会被布雷吗？
		{
			mGameData[k][l].mState = ncMINE;
			mGameData[k][l].mStateBackUp = ncMINE;
			mCount++;
		}
	} while (mCount != mMineNum);

	for(i=0;i<Stage_height;i++)
		for (j = 0; j < Stage_width; j++)
		{
			if (mGameData[i][j].mState != ncMINE)
			{
				mCount = 0;
				for (k = i - 1; k < i + 2; k++)
					for (l = j - 1; l < j + 2; l++)
						if (k >= 0 && k < Stage_height && l >= 0 && l < Stage_width)
							if (mGameData[k][l].mState == ncMINE)
								mCount++;
				switch (mCount)
				{
				case 0:
					mGameData[i][j].mState = ncNULL;
					break;
				case 1:
					mGameData[i][j].mState = ncONE;
					break;
				case 2:
					mGameData[i][j].mState = ncTWO;
					break;
				case 3:
					mGameData[i][j].mState = ncTHERE;
					break;
				case 4:
					mGameData[i][j].mState = ncFOUR;
					break;
				case 5:
					mGameData[i][j].mState = ncFIVE;
					break;
				case 6:
					mGameData[i][j].mState = ncSIX;
					break;
				case 7:
					mGameData[i][j].mState = ncSEVEN;
					break;
				case 8:
					mGameData[i][j].mState = ncEIGHT;
					break;
				default:
					break;
				}
			}
		}
}

void Game::Input()
{
	Event event;
	//struct MouseButtonEvent P;
	Vector2i  P1, P2;
	while (window.pollEvent(event))
	{
		//玩家关闭窗口
		if (event.type == Event::Closed ||
			(event.type == Event::EventType::KeyReleased 
				&& event.key.code == Keyboard::Escape))
		{
			window.close();
			GameQuit = 1;
		}

		//左击2（后击）
		if (event.type == sf::Event::MouseButtonPressed
			&& event.mouseButton.button == sf::Mouse::Left)
		{
			P2 = Mouse::getPosition(window);
			if (GameOverState == ncNO)
			{
				if (MouseClickTimer.getElapsedTime().asMilliseconds() < 500 
					&& P2.x - P1.x < GridSize / 4 
					&& P2.y - P1.y < GridSize / 4 && Mouse_RL_ClkReady)
				{
					LButtonDoubleClick(P2);//当两次点击的间隔小于300毫秒，则判定为鼠标双击
					RL_ClkJudge_flag = false;
				} 
				else
				{ 
					RL_ClkJudge_flag = true;
					Mouse_RL_ClkReady ++; //左键按下则RL双击准备
					if (RL_ClkJudge_flag && Mouse_RL_ClkReady == 2)
						RL_ButtonDown(P2);
					else
						LButtonDown(P2);
				}
			}
		}
		

		//左击1（先击）
		if (event.type == Event::MouseButtonReleased 
			&& event.mouseButton.button == Mouse::Left) 
		{
			P1 = Mouse::getPosition(window);
			if (GameOverState == ncNO) 
			{
				MouseClickTimer.restart();
				//按钮判断
				if (!GameStart) 
				{ 
					if (ButtonRectEasy.contains(event.mouseButton.x, event.mouseButton.y)) 
						GameLV = 1; 
					if (ButtonRectNormal.contains(event.mouseButton.x, event.mouseButton.y))
						GameLV = 2; 
					if (ButtonRectHard.contains(event.mouseButton.x, event.mouseButton.y))
						GameLV = 3; 
					Initial();//及时刷新舞台 
					
				} 
				
			} 
			if (ButtonRectBG.contains(event.mouseButton.x, event.mouseButton.y)) 
			{
				imgBGNo = ++imgBGNo % BG_NUM;
				LoadMediaData(); 
			} 
			if (ButtonRectSkin.contains(event.mouseButton.x, event.mouseButton.y))
			{ 
				imgSkinNo = ++imgSkinNo % SKIN_NUM;
				LoadMediaData(); 
			} 
			if (ButtonRectRestart.contains(event.mouseButton.x, event.mouseButton.y))
				Initial(); 
			if (ButtonRectQuit.contains(event.mouseButton.x, event.mouseButton.y))
			{
				window.close();
				GameQuit = true;
			}
		} 
		if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Right) 
		{
			if (GameOverState == ncNO)
			{
				Mouse_RL_ClkReady++;
				if (Mouse_RL_ClkReady == 2)
					RL_ButtonDown(Mouse::getPosition(window));//-----》鼠标左右击
				else
					RButtonDown(Mouse::getPosition(window));//-----》鼠标右击
			} 
		}
		if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Right) 
		{
			Mouse_RL_ClkReady = 0;//状态清除 
		} 
	}
}
void Game::RButtonDown(Vector2i mPoint)
{
	int i, j; 
	i = (mPoint.x - mCornPoint.x) / GridSize;
	j = (mPoint.y - mCornPoint.y) / GridSize;
	if (i >= 0 && i < Stage_width && j >= 0 && j < Stage_height)//如果在舞台内 
	{ 
		if (mGameData[j][i].isPress == false)
		{
			mGameData[j][i].isPress = true;
			mGameData[j][i].mStateBackUp = mGameData[j][i].mState;
			mGameData[j][i].mState = ncFLAG; 
			mFlagCalc++; 
		}
		else 
		{ 
			if (mGameData[j][i].mState == ncFLAG) 
			{
				mGameData[j][i].isPress = true;
				mGameData[j][i].mState = ncQ; 
				mFlagCalc--; 
			} 
			else if (mGameData[j][i].mState == ncQ)
			{ 
				mGameData[j][i].isPress = false;
				mGameData[j][i].mState = mGameData[j][i].mStateBackUp;
			} 
		} 
	} 
}
void Game::LButtonDown(Vector2i mPoint)
{
	int i, j;
	i = (mPoint.x - mCornPoint.x) / GridSize; //获取鼠标当前点击的块的位置 
	j = (mPoint.y - mCornPoint.y) / GridSize; 
	if (i >= 0 && i <Stage_width && j >= 0 && j < Stage_height) //如果在舞台内
	{ 
		if (!GameStart)//如果游戏未开始
		{ 
			GameStart = true;//游戏开始 
			GameClock.restart();
			MineSet(j, i); //点击之后再随机布雷
		} 
		//if(mGameData[j][i].mState != ncFLAG)//如果状态不是旗子 
			if (mGameData[j][i].isPress == false) 
			{ 
				mGameData[j][i].isPress = true; //当前块被点击
				cout << "mGameData[" << j << "][" << i << "].mState:" << endl;
				for (int k = j - 1; k <= j + 1; k++)
				{
					for (int l = i - 1; l <= i + 1; l++)
						cout << mGameData[k][l].mState << " ";
					cout << endl;
				}
				cout << "mGameData[" << j << "][" << i << "].mStateBackUp:" << endl;
				for (int k = j - 1; k <= j + 1; k++)
				{
					for (int l = i - 1; l <= i + 1; l++)
						cout << mGameData[k][l].mStateBackUp << " ";
					cout << endl;
				}
				cout << endl;
				if (mGameData[j][i].mState == ncMINE) //如果当前为雷 
				{ 
					GameStart = false; 
					GameOverState = ncLOSE;
					mGameData[j][i].mState = ncBOOMBING;
					unCover(); //揭开剩下未被找到的雷
				} 
				if (mGameData[j][i].mState == ncNULL) //如果当前块为空 
					NullClick(j, i); //查找未被点击的空块 
				
			} 
	} 
}
void Game::LButtonDoubleClick(Vector2i mPiont)
{
	int i, j, k, l, lvl;
	i = (mPiont.x - mCornPoint.x) / GridSize;
	j = (mPiont.y - mCornPoint.y) / GridSize;
	if (i >= 0 && i < Stage_width && j >= 0 && j < Stage_height
		&& mGameData[j][i].isPress && mGameData[j][i].mState != ncFLAG)
	//if (i >= 0 && i < Stage_width && j >= 0 && j < Stage_height)//若点在范围内 
		 //if (mGameData[j][i].isPress == true)//如果已被点击
			//if(mGameData[j][i].mState != ncFLAG)//如果当前块不是旗子
			//3个if为什么不写在一起？？
		for(k = j -1; k < j+2; k++)
			for(l = i-1; l < i+2; l++)//遍历周围8个格子 
				if (k >= 0 && k < Stage_height &&  l >= 0 && l < Stage_width
					&& mGameData[k][l].mState == ncFLAG)
					//if (mGameData[k][l].mState == ncFLAG)//若是旗子
						if (mGameData[k][l].mStateBackUp != ncMINE)
						{ 
							GameOverState = ncLOSE; 
							GameStart = false;
							unCover();
						} 
						else //如果状态不是旗子 
							if (mGameData[k][l].isPress == false)
							{
								mGameData[k][l].isPress = true; 
								if (mGameData[k][l].mState == ncMINE) 
								{ 
									GameOverState = ncLOSE;
									GameStart = false; 
									mGameData[k][l].mState = ncBOOMBING; 
									unCover();
								}
								if (mGameData[k][l].mState == ncNULL) 
									NullClick(k, l); 
							} 

}
void Game::RL_ButtonDown(Vector2i mPoint) //鼠标左右击 
{
	int i, j, k, l; 
	i = (mPoint.x - mCornPoint.x) / GridSize;
	j = (mPoint.y - mCornPoint.y) / GridSize; 
	if (i >= 0 && i < Stage_width && j >= 0 && j < Stage_height)
	{ 
		if (mGameData[j][i].isPress == true)//如果已被点击 
		{ 
			if (mGameData[j][i].mState != ncFLAG && mGameData[j][i].mState != ncQ) 
				for (k = j - 1; k < j + 2; k++)
					for (l = i - 1; l < i + 2; l++)//遍历周围8个格子
						if (k >= 0 && k < Stage_height && l >= 0 && l < Stage_width)
						{ 
							if (mGameData[k][l].isPress == false)
							{ 
								mGameData[k][l].isPress = true; 
								//mGameData[k][l].mStateBackUp = mGameData[k][l].mState;
								mGameData[k][l].mState = ncQ;
							}
						}
		}
		else
		{ 
			for (k = j - 1; k < j + 2; k++) 
				for (l = i - 1; l < i + 2; l++)//遍历周围8个格子 
					if (k >= 0 && k < Stage_height && l >= 0 && l < Stage_width) 
					{ 
						if (mGameData[k][l].isPress == false)
						{ 
							mGameData[k][l].isPress = true; 
							//mGameData[k][l].mStateBackUp = mGameData[k][l].mState;
							mGameData[k][l].mState = ncX;
						}
					} 
			mGameData[j][i].isPress = false;//微软规则，点击格，状态不变
		} 
	}
	RL_Point = mPoint; 
	RL_ClkJudge_flag = true; 
} 
//左右键松开
void Game::RL_ClkJudge()
{

}

void Game::unCover() 
{ 
	int i, j;
	for (j = 0; j < Stage_height; j++)
		for (i = 0; i < Stage_width; i++)
		{ 
			if (mGameData[j][i].isPress == false)
				if (mGameData[j][i].mState == ncMINE) 
				{ 
					mGameData[j][i].isPress = true; 
					mGameData[j][i].mState = ncUNFOUND; 
				} 
		}
}
void Game::NullClick(int j, int i) //查找空块 
{ 
	int k, l; 
	for (k = j - 1; k < j + 2; k++)//以(j,i)为中心的九宫格的行
		for (l = i - 1; l < i + 2; l++) //以(j,i)为中心的九宫格的列
			if (k >= 0 && k < Stage_height && l >= 0 && l < Stage_width)
			//（j,i）九宫格在舞台内才处理
			{
				if (mGameData[k][l].isPress == false) //格子未被翻开
				{ 
					mGameData[k][l].isPress = true; //翻开格子
					if (mGameData[k][l].mState == ncNULL) //如果状态为空 
						NullClick(k, l); //递归调用,继续查找 
				} 
			} 
}
void Game::Logic()
{
	if (Mouse_RL_ClkReady == 0 && RL_ClkJudge_flag)
		RL_ClkJudge();
	//每次鼠标点击行为都会触发一次胜负判断？
	//if (mFlagCalc == mMineNum||玩家认为剩下没插棋的块都是雷，点击“扫雷完成键”)
	isWin();
	
}
void Game::isWin()
{
	int i, j, c = 0;
	/*
	if (mFlagCalc == mMineNum)//判断插的所有旗是不是都是雷 
	{ 
		for(i=0;i<Stage_width;i++) 
			for (j = 0; j < Stage_height; j++)
			{
				if (mGameData[j][i].mState == ncFLAG) 
					if (mGameData[j][i].mStateBackUp == ncMINE)
						c++; 
			} 
	}
	else//判断剩下没插棋的块是不是都是雷 
	{ 
		if (GameOverState != ncLOSE)
		{ 
			for (i = 0; i < Stage_width; i++) 
				for (j = 0; j < Stage_height; j++) 
				{ 
					if (mGameData[j][i].isPress==false||mGameData[j][i].mState==ncFLAG)
						c++;
				}
		} 
	} 
	*/
	if (GameOverState != ncLOSE)
	{
		for (i = 0; i < Stage_width; i++)
			for (j = 0; j < Stage_height; j++) 
			{ //如果有旗子误插了，那该条件有没可能成立？
				if (mGameData[j][i].isPress == false || mGameData[j][i].mState == ncFLAG) 
					c++;
			}
	}
	
	if (c == mMineNum)//如果所有旗子对应都是雷，游戏胜利并结束
	{ 
		GameStart = false; 
		GameOverState = ncWIN; 
	} 
}



void Game::Draw()
{
	window.clear();
	spBackground.setPosition(0, 0);
	window.draw(spBackground);

	DrawGrid();
	DrawButton();
	DrawTimer();
	DrawScore();
	if (GameOver)
		DrawGameEnd();
	window.display();
}
void Game::DrawGrid()
{
	for (int j = 0; j < Stage_height; j++)
		for (int i = 0; i < Stage_width; i++)
		{
			if (mGameData[j][i].isPress)
			{
				spTiles.setTextureRect(IntRect(mGameData[j][i].mState*GRIDSIZE, 0, GRIDSIZE, GRIDSIZE));
				spTiles.setPosition(mCornPoint.x + i * GRIDSIZE, mCornPoint.y + j * GRIDSIZE);
			}
			else
			{
				//spTiles.setTextureRect(IntRect(mGameData[j][i].mState*GRIDSIZE, 0, GRIDSIZE, GRIDSIZE));
				spTiles.setTextureRect(IntRect(ncUNDOWN*GRIDSIZE, 0, GRIDSIZE, GRIDSIZE));
				spTiles.setPosition(mCornPoint.x + i * GRIDSIZE, mCornPoint.y + j * GRIDSIZE);
			}
			window.draw(spTiles);
		}
}
void Game::DrawButton()
{
	Vector2i LeftCorner;
	int ButtonWidth = 60;
	int ButtonHeight = 36;
	int detaX;
	detaX = (Window_width - ButtonWidth * 7) / 8;//7个按钮在界面上等分宽度
	LeftCorner.y = mCornPoint.y + (Stage_height + 2)*GRIDSIZE;//指定高度
	
	//ButtonRectEasy 
	LeftCorner.x = detaX;
	spButtons.setTextureRect(IntRect(0 * ButtonWidth, 0, ButtonWidth, ButtonHeight));//读取按钮的纹理区域
	spButtons.setPosition(LeftCorner.x, LeftCorner.y);//设置按钮的位置坐标
	ButtonRectEasy.left = LeftCorner.x;
	ButtonRectEasy.top = LeftCorner.y;
	ButtonRectEasy.width = ButtonWidth;
	ButtonRectEasy.height = ButtonHeight;
	window.draw(spButtons);

	//ButtonRectNormal 
	LeftCorner.x = detaX * 2 + ButtonWidth;
	spButtons.setTextureRect(IntRect(1 * ButtonWidth, 0, ButtonWidth, ButtonHeight));//读取按钮的纹理区域
	spButtons.setPosition(LeftCorner.x, LeftCorner.y);//设置按钮的位置坐标
	ButtonRectNormal.left = LeftCorner.x;
	ButtonRectNormal.top = LeftCorner.y;
	ButtonRectNormal.width = ButtonWidth;
	ButtonRectNormal.height = ButtonHeight;
	window.draw(spButtons);
	
	 //ButtonRectHard 
	LeftCorner.x = detaX * 3 + ButtonWidth * 2;
	spButtons.setTextureRect(IntRect(2 * ButtonWidth, 0, ButtonWidth, ButtonHeight));//读取按钮的纹理区域
	spButtons.setPosition(LeftCorner.x, LeftCorner.y);//设置按钮的位置坐标
	ButtonRectHard.left = LeftCorner.x;
	ButtonRectHard.top = LeftCorner.y;
	ButtonRectHard.width = ButtonWidth;
	ButtonRectHard.height = ButtonHeight;
	window.draw(spButtons);

	//ButtonRectSkin
	LeftCorner.x = detaX * 4 + ButtonWidth * 3;
	spButtons.setTextureRect(IntRect(3 * ButtonWidth, 0, ButtonWidth, ButtonHeight));//读取按钮的纹理区域
	spButtons.setPosition(LeftCorner.x, LeftCorner.y);//设置按钮的位置坐标
	ButtonRectSkin.left = LeftCorner.x;
	ButtonRectSkin.top = LeftCorner.y;
	ButtonRectSkin.width = ButtonWidth;
	ButtonRectSkin.height = ButtonHeight;
	window.draw(spButtons);

	//ButtonRectBG
	LeftCorner.x = detaX * 5 + ButtonWidth * 4;
	spButtons.setTextureRect(IntRect(4 * ButtonWidth, 0, ButtonWidth, ButtonHeight));//读取按钮的纹理区域
	spButtons.setPosition(LeftCorner.x, LeftCorner.y);//设置按钮的位置坐标
	ButtonRectBG.left = LeftCorner.x;
	ButtonRectBG.top = LeftCorner.y;
	ButtonRectBG.width = ButtonWidth;
	ButtonRectBG.height = ButtonHeight;
	window.draw(spButtons);


	//ButtonRectRestart
	LeftCorner.x = detaX * 6 + ButtonWidth * 5;
	spButtons.setTextureRect(IntRect(5 * ButtonWidth, 0, ButtonWidth, ButtonHeight));//读取按钮的纹理区域
	spButtons.setPosition(LeftCorner.x, LeftCorner.y);//设置按钮的位置坐标
	ButtonRectRestart.left = LeftCorner.x;
	ButtonRectRestart.top = LeftCorner.y;
	ButtonRectRestart.width = ButtonWidth;
	ButtonRectRestart.height = ButtonHeight;
	window.draw(spButtons);

	//ButtonRectRestart
	LeftCorner.x = detaX * 6 + ButtonWidth * 5;
	spButtons.setTextureRect(IntRect(5 * ButtonWidth, 0, ButtonWidth, ButtonHeight));//读取按钮的纹理区域
	spButtons.setPosition(LeftCorner.x, LeftCorner.y);//设置按钮的位置坐标
	ButtonRectRestart.left = LeftCorner.x;
	ButtonRectRestart.top = LeftCorner.y;
	ButtonRectRestart.width = ButtonWidth;
	ButtonRectRestart.height = ButtonHeight;
	window.draw(spButtons);

	//ButtonRectQuit
	LeftCorner.x = detaX * 7 + ButtonWidth * 6;
	spButtons.setTextureRect(IntRect(6 * ButtonWidth, 0, ButtonWidth, ButtonHeight));//读取按钮的纹理区域
	spButtons.setPosition(LeftCorner.x, LeftCorner.y);//设置按钮的位置坐标
	ButtonRectQuit.left = LeftCorner.x;
	ButtonRectQuit.top = LeftCorner.y;
	ButtonRectQuit.width = ButtonWidth;
	ButtonRectQuit.height = ButtonHeight;
	window.draw(spButtons);
}
void Game::DrawScore()
{
	
	Vector2i LeftCorner;
	LeftCorner.x = Window_width - spCounter.getLocalBounds().width*1.25;
	LeftCorner.y = spCounter.getLocalBounds().height*0.5;
	spCounter.setPosition(LeftCorner.x,LeftCorner.y);//计数器纹理的贴图位置
	window.draw(spCounter);
	int NumSize = spNum.getLocalBounds().height;
	LeftCorner.x = LeftCorner.x + spCounter.getLocalBounds().width - NumSize;
	LeftCorner.y = LeftCorner.y + spCounter.getLocalBounds().height*0.5 - NumSize * 0.5;
	int mScore = mMineNum - mFlagCalc;
	
	//绘制个位数的数字
	int a = mScore % 10;
	spNum.setTextureRect(IntRect(a*NumSize, 0, NumSize, NumSize));//在贴图上取对应数字字符的纹理贴图
	spNum.setPosition(LeftCorner.x, LeftCorner.y);
	window.draw(spNum);

	//绘制十位数的数字
	mScore = mScore / 10;
	a = mScore % 10;
	LeftCorner.x = LeftCorner.x - NumSize;
	spNum.setTextureRect(IntRect(a*NumSize, 0, NumSize, NumSize));//在贴图上取对应数字字符的纹理贴图
	spNum.setPosition(LeftCorner.x, LeftCorner.y);
	window.draw(spNum);

	//绘制百位数的数字
	mScore = mScore / 10;
	a = mScore % 10;
	LeftCorner.x = LeftCorner.x - NumSize;
	spNum.setTextureRect(IntRect(a*NumSize, 0, NumSize, NumSize));//在贴图上取对应数字字符的纹理贴图
	spNum.setPosition(LeftCorner.x, LeftCorner.y);
	window.draw(spNum);
}
void Game::DrawTimer()
{
	Vector2i LeftCorner;
	LeftCorner.x = spTimer.getLocalBounds().width*0.25;
	LeftCorner.y = spTimer.getLocalBounds().height*0.5;
	spTimer.setPosition(LeftCorner.x, LeftCorner.y);
	window.draw(spTimer);
	if (GameStart)
		mTime = GameClock.getElapsedTime().asSeconds();
	int mScore = mTime;
	if (mScore > 999)
		mScore = 999;
	int NumSize = spNum.getGlobalBounds().height;
	LeftCorner.x = LeftCorner.x + spTimer.getLocalBounds().width - NumSize * 1.5;
	LeftCorner.y = LeftCorner.y + spTimer.getLocalBounds().height*0.5 - NumSize * 0.5;

	//绘制个位数
	int a = mScore % 10;
	spNum.setTextureRect(IntRect(a*NumSize, 0, NumSize, NumSize));//在贴图上取对应数字字符的纹理贴图
	spNum.setPosition(LeftCorner.x, LeftCorner.y);
	window.draw(spNum);

	//绘制十位数的数字
	mScore = mScore / 10;
	a = mScore % 10;
	LeftCorner.x = LeftCorner.x - NumSize;
	spNum.setTextureRect(IntRect(a*NumSize, 0, NumSize, NumSize));//在贴图上取对应数字字符的纹理贴图
	spNum.setPosition(LeftCorner.x, LeftCorner.y);
	window.draw(spNum);

	//绘制百位数的数字
	mScore = mScore / 10;
	a = mScore % 10;
	LeftCorner.x = LeftCorner.x - NumSize;
	spNum.setTextureRect(IntRect(a*NumSize, 0, NumSize, NumSize));//在贴图上取对应数字字符的纹理贴图
	spNum.setPosition(LeftCorner.x, LeftCorner.y);
	window.draw(spNum);
}
void Game::DrawGameEnd()
{
	Vector2i LeftCorner;
	int ButtonWidth = 200;
	int ButtonHeight = spGameOver.getLocalBounds().height;
	LeftCorner.x = (Window_width - ButtonWidth) / 2;
	LeftCorner.y = (Window_height - ButtonHeight) / 2;
	spGameOver.setPosition(LeftCorner.x, LeftCorner.y);
	if (GameOverState == ncWIN)
		spGameOver.setTextureRect(IntRect(0 * ButtonWidth, 0, ButtonWidth, ButtonHeight));
	if (GameOverState == ncLOSE)
		spGameOver.setTextureRect(IntRect(1 * ButtonWidth, 0, ButtonWidth, ButtonHeight));
	window.draw(spGameOver);
}

void Game::Run()
{
	do
	{
		Initial();
		while (window.isOpen() && !GameOver)
		{
			Input();
			Logic();
			Draw();

		}
	} while (!GameQuit);
}




