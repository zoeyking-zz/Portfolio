#include "UNOGAME.h"

UNOGAME::UNOGAME()
{
	window.setFramerateLimit(60);
	window.create(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), L"UNO游戏开始！by金磨");

	Initial();
	
}
UNOGAME::~UNOGAME()
{
	GameOver = 1;
	GameQuit = 1;
}

void UNOGAME::Initial()
{
	InitialData();
	InitialMeida();
	PD_DecdieCard(&Player1,1);		//Player1第一个出牌也能使用算法
	cout << "初始化结束！" << endl;
}
void UNOGAME::InitialData()
{
	nowStage = cStart;
	nowMouse = cNoPressed;

	GameOver = false;
	GameQuit = false;
	GameStart = false;
	AI_start = false;
	isClockWise = true;		//初始为顺时针
	BG = 1;					//默认使用第一张背景图
	musicNo = 1;

	//初始化按钮是否可按
	BtnNoCardsState = false;
	BtnHandDownState = false;
	BtnPlayState = true;
	BtnUnoState = false;
	BtnChooseColorState = false;
	//初始化按钮未被按下
	isNoCardsPressed = false;
	isHandDownPressed = false;
	isPlayPressed = false;
	isUnoPressed = false;

	CardOut_i = 0;
	PlusCard = 0;
	PlusCardStart = false;

	//初始化牌堆
	for (int i = 0; i < 108; i++)
		isCardTaken[i] = false;

	nowCard.cType = cZERO;
	nowCard.cColor = cBLACK;

	//注意要先初始化卡牌信息
	InitialCardsData();
	InitialPlayerData();
}
void UNOGAME::InitialPlayerData()
{
	//初始化now_Card
	Player1.Init();
	Player2.Init();
	Player3.Init();
	Player4.Init();

	//初始化出牌位置
	Player1.CardOut_pos.x = CARDOUT_CORNER_X1;
	Player1.CardOut_pos.y = CARDOUT_CORNER_Y1;
	Player2.CardOut_pos.x = CARDOUT_CORNER_X2;
	Player2.CardOut_pos.y = CARDOUT_CORNER_Y2;
	Player3.CardOut_pos.x = CARDOUT_CORNER_X3;
	Player3.CardOut_pos.y = CARDOUT_CORNER_Y3;
	Player4.CardOut_pos.x = CARDOUT_CORNER_X4;
	Player4.CardOut_pos.y = CARDOUT_CORNER_Y4;

	//初始化手牌链表
	Player1.InitHandCardList();
	Player2.InitHandCardList();
	Player3.InitHandCardList();
	Player4.InitHandCardList();
	
	//发牌，每人5张
	DealCards(Player1, 5);
	DealCards(Player2, 5);
	DealCards(Player3, 5);
	DealCards(Player4, 5);

	//初始各玩家状态
	Player1.isOver = cNoOver;
	Player2.isOver = cNoOver;
	Player3.isOver = cNoOver;
	Player4.isOver = cNoOver;

	Player1.isRound = true;
	Player2.isRound = false;
	Player3.isRound = false;
	Player4.isRound = false;


	//初始化玩家手牌选择信息
	for (int i = 0; i < MAXCARDS; i++)
		Player1.isCardChosen[i] = false;
	for (int i = 0; i < MAXCARDS; i++)
		Player2.isCardChosen[i] = false;
	for (int i = 0; i < MAXCARDS; i++)
		Player3.isCardChosen[i] = false;
	for (int i = 0; i < MAXCARDS; i++)
		Player4.isCardChosen[i] = false;

	//数值化各玩家AbleCards数组
	Player1.InitAblbCards();
	Player2.InitAblbCards();
	Player3.InitAblbCards();
	Player4.InitAblbCards();
	
}
void UNOGAME::InitialCardsData()
{
	int i = 0;
	//黑色牌
	//四张换色+罚牌4张（Card 0 0）
	while (i < 4)
	{
		Cards[i].cColor = cBLACK;
		Cards[i].cType = cPlus4;
		i++;
	}
	//四张黑色换色（Card 0 1）
	while (i > 3 && i < 8)
	{
		Cards[i].cColor = cBLACK;
		Cards[i].cType = cChangeColor;
		i++;
	}

	while (i > 7 && i < 108)
	{
		//决定颜色
		int j = (i - 8) / 25;
		if (j == 0)
			Cards[i].cColor = cBLUE;
		else if (j == 1)
			Cards[i].cColor = cRED;
		else if (j == 2)
			Cards[i].cColor = cGREEN;
		else if (j == 3)
			Cards[i].cColor = cYELLOW;

		//决定牌号
		j = (i - 8) % 25;
		if (j < 1)
			Cards[i].cType = cZERO;
		else if (j < 3)		Cards[i].cType = cONE;
		else if (i < 5)		Cards[i].cType = cTWO;
		else if (j < 7)		Cards[i].cType = cTHREE;
		else if (j < 9)		Cards[i].cType = cFOUR;
		else if (j < 11)	Cards[i].cType = cFIVE;
		else if (j < 13)	Cards[i].cType = cSIX;
		else if (j < 15)	Cards[i].cType = cSEVEN;
		else if (j < 17)	Cards[i].cType = cEIGHT;
		else if (j < 19)	Cards[i].cType = cNINE;
		else if (j < 21)	Cards[i].cType = cPlus2;
		else if (j < 23)	Cards[i].cType = cSkip;
		else if (j < 25)	Cards[i].cType = cReverse;
		i++;
	}
	//卡牌点数初始化
	for (int i = 0; i < 108; i++)
	{
		if (Cards[i].cType == cZERO)
			Cards[i].cScore = cZERO_S;
		if (Cards[i].cType == cONE)
			Cards[i].cScore = cONE_S;
		if (Cards[i].cType == cTWO)
			Cards[i].cScore = cTWO_S;
		if (Cards[i].cType == cTHREE)
			Cards[i].cScore = cTHREE_S;
		if (Cards[i].cType == cFOUR)
			Cards[i].cScore = cFOUR_S;
		if (Cards[i].cType == cFIVE)
			Cards[i].cScore = cFIVE_S;
		if (Cards[i].cType == cSIX)
			Cards[i].cScore = cSIX_S;
		if (Cards[i].cType == cSEVEN)
			Cards[i].cScore = cSEVEN_S;
		if (Cards[i].cType == cEIGHT)
			Cards[i].cScore = cEIGHT_S;
		if (Cards[i].cType == cNINE)
			Cards[i].cScore = cNINE_S;
		if (Cards[i].cType == cPlus2 || Cards[i].cType == cSkip || Cards[i].cType == cReverse)
			Cards[i].cScore = cTWENTY_S;
		if(Cards[i].cType == cPlus4 || Cards[i].cType == cChangeColor)
			Cards[i].cScore = cFOURTY_S;
	}
}
void UNOGAME::InitialMeida()
{

	stringstream ss;
	ss << "data/images/BG" << BG << ".png";
	if (!tBackground.loadFromFile(ss.str()))
		cout << "Background" << BG << "没有找到" << endl;
	ss.str("");//清空字符串

	if (!tGameStart.loadFromFile("data/images/Start.png"))				//开始场景图片
		cout << "StartScene Image not found." << endl;
	if (!tStartBtn.loadFromFile("data/images/ButtonStart1.png"))
		cout << "StartBtn Image not found." << endl;
	if (!tStartRule.loadFromFile("data/images/ButtonStart2.png"))
		cout << "StartRule Image not found." << endl;

	if (!tRuleBG.loadFromFile("data/images/Setting.png"))				//开始场景图片
		cout << "StartScene Image not found." << endl;
	if (!tBack.loadFromFile("data/images/ButtonBack.png"))
		std::cout << "ButtonBack Image not found." << std::endl;

	if (!tButton.loadFromFile("data/images/Button.png"))				//六个小按钮，两个中按钮，一个大按钮
		cout << "Buttons image not found." <<endl;
	if (!tButtonAI.loadFromFile("data/images/ButtonAI.png"))				//AI按钮
		cout << "AIButtons image not found." << endl;
	if (!tPlayer.loadFromFile("data/images/Player.png"))				//玩家头像图片
		cout << "Player Profile Photos not found." << endl;
	if (!tCardBack.loadFromFile("data/images/CardBack.png"))			//牌背图片
		cout << "CardBack Image not found." << endl;
	if (!tCardCount.loadFromFile("data/images/CardCount.png"))			//数字图片
		cout << "cardNum Image not found." << endl;
	if (!tGameOver.loadFromFile("data/images/GameOver.png"))			//输赢图片
		cout << "GameOver Image not found." << endl;
	if (!tChangeColor.loadFromFile("data/images/ChangeColor.png"))		//变色
		cout << "ChangeColor Image not found." << endl;
	if (!tPlus4Color.loadFromFile("data/images/Plus4Color.png"))		//+4变色
		cout << "Plus4Color Image not found." << endl;

	spBackground.setTexture(tBackground);
	spGameStart.setTexture(tGameStart);
	spStartBtn.setTexture(tStartBtn);
	spStartRule.setTexture(tStartRule);

	spRuleBG.setTexture(tRuleBG);
	spBack.setTexture(tBack);

	spCardCount.setTexture(tCardCount);
	spGameOver.setTexture(tGameOver);
	spPlayer.setTexture(tPlayer);
	spCardBack.setTexture(tCardBack);
	spButton.setTexture(tButton);
	spButtonAI.setTexture(tButtonAI);

	spChangeColor.setTexture(tChangeColor);
	spPlus4Color.setTexture(tPlus4Color);

	spCardBack.setOrigin(CARD_BACK_W * CARD_BACK_SCALE / 2, CARD_BACK_H * CARD_BACK_SCALE / 2);
	//spPlayer.setOrigin(PLAYERBOX_W * PLAYERBOX_SCALE / 2, PLAYERBOX_H * PLAYERBOX_SCALE / 2);
	//spCardCount.setOrigin(PLAYERBOX_W * PLAYERBOX_SCALE / 2, PLAYERBOX_H * PLAYERBOX_SCALE / 2);
	//其他精灵的原点设置（暂略）

	spCardBack.setScale(CARD_BACK_SCALE, CARD_BACK_SCALE);
	spPlayer.setScale(PLAYERBOX_SCALE, PLAYERBOX_SCALE);

	ss.str("");	//清空字符串
	ss << "data/audio/Music" << musicNo << ".flac";
	if (!bgMusic.openFromFile(ss.str()))								//背景音乐
		std::cout << "BGMusic not found." << std::endl;
	bgMusic.play();
	bgMusic.setLoop(true);

}
void UNOGAME::Input()
{
	Event event;
	while (window.pollEvent(event))
	{
		if (event.type == Event::Closed ||
			(event.type == Event::EventType::KeyReleased
				&& event.key.code == Keyboard::Escape))
		{
			window.close();		//窗口可以移动、调整大小和最小化。但是如果要关闭，需要自己去调用close()函数
			GameQuit = true;
		}

		//松开左键
		if (event.type == sf::Event::MouseButtonReleased
			&& event.mouseButton.button == sf::Mouse::Left)
		{
			nowMouse = cLPressed;
			p_mouse = Mouse::getPosition(window);
		}

		//控制托管
		if (event.type == Event::EventType::KeyReleased && event.key.code == Keyboard::Q)
		{
			AI_start = !AI_start;
		}
	}
}

void UNOGAME::Logic()
{
	//检查按钮是否被按下，只有在State显示状态为能true的时候才会改变
	isButtonPressed(p_mouse);

	//游戏界面
	if (nowStage == cPlaying)
	{
		//检查玩家Player1能不能出牌，改变 Play 和 NoCard 按钮的显示状态
		if (CheckCardDemand(Player1) != 0)
		{
			BtnPlayState = true;
			BtnNoCardsState = false;
		}
		else
		{
			BtnPlayState = false;
			BtnNoCardsState = true;
		}

		if (!AI_start)
		{

			if (nowMouse == cLPressed)
				isCardPressed(p_mouse);	//检查玩家Player1的牌是不是被选择了

			//按下 Play 按钮，出牌
			if (isPlayPressed && BtnChooseColorState == false)
			{
				HumanPlayCard();	//玩家出牌

				//重置时钟和按钮点击状态
				gameClock.restart();
				isPlayPressed = false;

				//下一个玩家出牌
				Player1.isRound = false;
				if (isClockWise == false)
				{
					if (nowCard.cType == cSkip && lastcardNum != nowcardNum)
						Player3.isRound = true;
					else if (nowCard.cType == cReverse && lastcardNum != nowcardNum)
					{
						Player4.isRound = true;
						isClockWise = !isClockWise;
					}
					else
					{
						Player2.isRound = true;
						if (nowCard.cType == cPlus2)
						{
							if (PlusCardStart == false)
							{
								PlusCardStart = true;
								PlusCard = PlusCard + 2;	//保存当前加牌的数量
							}
							else
								PlusCard = PlusCard + 2;	//保存当前加牌的数量
							//DealCards(Player2, 2);
						}
						else if (nowCard.cType == cPlus4 && nowCard.cColor == cBLACK)
						{
							if (PlusCardStart == false)
							{
								PlusCardStart = true;
								PlusCard = PlusCard + 4;	//保存当前加牌的数量
							}
							else
								PlusCard = PlusCard + 4;	//保存当前加牌的数量
							//DealCards(Player2, 4);
						}
					}
				}
				else if (isClockWise == true)
				{
					if (nowCard.cType == cSkip && lastcardNum != nowcardNum)
						Player3.isRound = true;

					else if (nowCard.cType == cReverse && lastcardNum != nowcardNum)
					{
						Player2.isRound = true;
						isClockWise = !isClockWise;
					}
					else
					{
						Player4.isRound = true;
						if (nowCard.cType == cPlus2)
						{
							if (PlusCardStart == false)
							{
								PlusCardStart = true;
								PlusCard = PlusCard + 2;	//保存当前加牌的数量
							}
							else
								PlusCard = PlusCard + 2;	//保存当前加牌的数量
							//DealCards(Player2, 2);
						}
						else if (nowCard.cType == cPlus4 && nowCard.cColor == cBLACK)
						{
							if (PlusCardStart == false)
							{
								PlusCardStart = true;
								PlusCard = PlusCard + 4;	//保存当前加牌的数量
							}
							else
								PlusCard = PlusCard + 4;	//保存当前加牌的数量
							//DealCards(Player2, 4);
						}
					}
				}
			}
			//人类没牌可出，摸一张
			else if (Player1.isRound && isNoCardsPressed && BtnChooseColorState == false)
			{
				DealCards(Player1, 1);

				//重置时钟和按钮点击状态
				isNoCardsPressed = false;
				gameClock.restart();

				//下一个玩家出牌
				Player1.isRound = false;
				if (isClockWise == false)
					Player2.isRound = true;
				else if (isClockWise == true)
					Player4.isRound = true;
			}
		}


		//AI 
		if (AI_start && Player1.isRound && gameClock.getElapsedTime().asMilliseconds() > 2000 && BtnChooseColorState == false)
		{
			//AIPlayCard_PD(&Player1,1);
			
			//AI2:Player1多步遍历树决策，Player234单步单步决策-颜色决策
			AIPlayCard_PD(&Player1,2);
			AIPlayCard_PD(&Player2,1);	//比Player1的AIPlayCard_PD少颜色决策，因为颜色决策函数以Player1最优设计，某程序员懒得改
			AIPlayCard_PD(&Player2,1);
			AIPlayCard_PD(&Player2,1);
			
			

			//重置时钟
			gameClock.restart();

			Player1.ReSetAbleCards();
			//AI2:重置玩家234
			/*
			Player2.ReSetAbleCards();
			Player3.ReSetAbleCards();
			Player4.ReSetAbleCards();
			*/

			//下一个玩家出牌
			Player1.isRound = false;
			if (isClockWise == false)
			{
				if (nowCard.cType == cSkip && lastcardNum != nowcardNum)
					Player3.isRound = true;
				else if (nowCard.cType == cReverse && lastcardNum != nowcardNum)
				{
					Player4.isRound = true;
					isClockWise = !isClockWise;
				}
				else
				{
					Player2.isRound = true;
					if (nowCard.cType == cPlus2)
					{
						if (PlusCardStart == false)
						{
							PlusCardStart = true;
							PlusCard = PlusCard + 2;	//保存当前加牌的数量
						}
						else
							PlusCard = PlusCard + 2;	//保存当前加牌的数量
						//DealCards(Player2, 2);
					}
					else if (nowCard.cType == cPlus4 && nowcardNum >= 0 && nowcardNum < 8)
					{
						if (PlusCardStart == false)
						{
							PlusCardStart = true;
							PlusCard = PlusCard + 4;	//保存当前加牌的数量
						}
						else
							PlusCard = PlusCard + 4;	//保存当前加牌的数量
						//DealCards(Player2, 4);
					}
						
				}
			}
			else if (isClockWise == true)
			{
				if (nowCard.cType == cSkip && lastcardNum != nowcardNum)
					Player3.isRound = true;

				else if (nowCard.cType == cReverse && lastcardNum != nowcardNum)
				{
					Player2.isRound = true;
					isClockWise = !isClockWise;
				}
				else
				{
					Player4.isRound = true;
					if (nowCard.cType == cPlus2)
					{
						if (PlusCardStart == false)
						{
							PlusCardStart = true;
							PlusCard = PlusCard + 2;	//保存当前加牌的数量
						}
						else
							PlusCard = PlusCard + 2;	//保存当前加牌的数量
						//DealCards(Player2, 2);
					}
					else if (nowCard.cType == cPlus4 && nowcardNum >= 0 && nowcardNum < 8)
					{
						if (PlusCardStart == false)
						{
							PlusCardStart = true;
							PlusCard = PlusCard + 4;	//保存当前加牌的数量
						}
						else
							PlusCard = PlusCard + 4;	//保存当前加牌的数量
						//DealCards(Player2, 4);
					}
				}
			}
			if (GameStart == false)
				GameStart = true;
		}

		//AI出牌回合，延时2s

		//玩家2
		else if (Player2.isRound && gameClock.getElapsedTime().asMilliseconds() > 2000 && BtnChooseColorState == false)
		{
			AIPlayCard(&Player2);

			//重置时钟
			gameClock.restart();

			//下一个玩家出牌
			Player2.isRound = false;
			if (isClockWise == false)
			{
				if (nowCard.cType == cSkip && lastcardNum != nowcardNum)
					Player4.isRound = true;


				else if (nowCard.cType == cReverse && lastcardNum != nowcardNum)
				{
					Player1.isRound = true;
					isClockWise = !isClockWise;
				}
				else
				{
					Player3.isRound = true;
					if (nowCard.cType == cPlus2)
					{
						if (PlusCardStart == false)
						{
							PlusCardStart = true;
							PlusCard = PlusCard + 2;	//保存当前加牌的数量
						}
						else
							PlusCard = PlusCard + 2;	//保存当前加牌的数量
						//DealCards(Player2, 2);
					}
					else if (nowCard.cType == cPlus4 && nowcardNum >= 0 && nowcardNum < 8)
					{
						if (PlusCardStart == false)
						{
							PlusCardStart = true;
							PlusCard = PlusCard + 4;	//保存当前加牌的数量
						}
						else
							PlusCard = PlusCard + 4;	//保存当前加牌的数量
						//DealCards(Player2, 4);
					}
				}
			}
			else if (isClockWise == true)
			{
				if (nowCard.cType == cSkip && lastcardNum != nowcardNum)
					Player4.isRound = true;


				else if (nowCard.cType == cReverse && lastcardNum != nowcardNum)
				{
					Player3.isRound = true;
					isClockWise = !isClockWise;
				}
				else
				{
					Player1.isRound = true;
					if (nowCard.cType == cPlus2)
					{
						if (PlusCardStart == false)
						{
							PlusCardStart = true;
							PlusCard = PlusCard + 2;	//保存当前加牌的数量
						}
						else
							PlusCard = PlusCard + 2;	//保存当前加牌的数量
						//DealCards(Player2, 2);
					}
					else if (nowCard.cType == cPlus4 && nowcardNum >= 0 && nowcardNum < 8)
					{
						if (PlusCardStart == false)
						{
							PlusCardStart = true;
							PlusCard = PlusCard + 4;	//保存当前加牌的数量
						}
						else
							PlusCard = PlusCard + 4;	//保存当前加牌的数量
						//DealCards(Player2, 4);
					}
				}

			}
		}
		//玩家3
		else if (Player3.isRound && gameClock.getElapsedTime().asMilliseconds() > 2000 && BtnChooseColorState == false)
		{
			AIPlayCard(&Player3);

			//重置时钟
			gameClock.restart();

			//下一个玩家出牌
			Player3.isRound = false;
			if (isClockWise == false)
			{

				if (nowCard.cType == cSkip && lastcardNum != nowcardNum)
					Player1.isRound = true;

				else if (nowCard.cType == cReverse && lastcardNum != nowcardNum)
				{
					Player2.isRound = true;
					isClockWise = !isClockWise;
				}
				else
				{
					Player4.isRound = true;
					if (nowCard.cType == cPlus2)
					{
						if (PlusCardStart == false)
						{
							PlusCardStart = true;
							PlusCard = PlusCard + 2;	//保存当前加牌的数量
						}
						else
							PlusCard = PlusCard + 2;	//保存当前加牌的数量
						//DealCards(Player2, 2);
					}
					else if (nowCard.cType == cPlus4 && nowcardNum >= 0 && nowcardNum < 8)
					{
						if (PlusCardStart == false)
						{
							PlusCardStart = true;
							PlusCard = PlusCard + 4;	//保存当前加牌的数量
						}
						else
							PlusCard = PlusCard + 4;	//保存当前加牌的数量
						//DealCards(Player2, 4);
					}
				}
			}
			else if (isClockWise == true)
			{
				if (nowCard.cType == cSkip && lastcardNum != nowcardNum)
					Player1.isRound = true;

				else if (nowCard.cType == cReverse && lastcardNum != nowcardNum)
				{
					Player4.isRound = true;
					isClockWise = !isClockWise;
				}
				else
				{
					Player2.isRound = true;
					if (nowCard.cType == cPlus2)
					{
						if (PlusCardStart == false)
						{
							PlusCardStart = true;
							PlusCard = PlusCard + 2;	//保存当前加牌的数量
						}
						else
							PlusCard = PlusCard + 2;	//保存当前加牌的数量
						//DealCards(Player2, 2);
					}
					else if (nowCard.cType == cPlus4 && nowcardNum >= 0 && nowcardNum < 8)
					{
						if (PlusCardStart == false)
						{
							PlusCardStart = true;
							PlusCard = PlusCard + 4;	//保存当前加牌的数量
						}
						else
							PlusCard = PlusCard + 4;	//保存当前加牌的数量
						//DealCards(Player2, 4);
					}
				}
			}
		}
		//玩家4
		else if (Player4.isRound && gameClock.getElapsedTime().asMilliseconds() > 2000 && BtnChooseColorState == false)
		{
			AIPlayCard(&Player4);

			//重置时钟
			gameClock.restart();

			//下一个玩家出牌
			Player4.isRound = false;
			if (isClockWise == false)
			{
				if (nowCard.cType == cSkip && lastcardNum != nowcardNum)
					Player2.isRound = true;

				else if (nowCard.cType == cReverse && lastcardNum != nowcardNum)
				{
					Player3.isRound = true;
					isClockWise = !isClockWise;
				}
				else
				{
					Player1.isRound = true;
					if (nowCard.cType == cPlus2)
					{
						if (PlusCardStart == false)
						{
							PlusCardStart = true;
							PlusCard = PlusCard + 2;	//保存当前加牌的数量
						}
						else
							PlusCard = PlusCard + 2;	//保存当前加牌的数量
						//DealCards(Player2, 2);
					}
					else if (nowCard.cType == cPlus4 && nowcardNum >= 0 && nowcardNum < 8)
					{
						if (PlusCardStart == false)
						{
							PlusCardStart = true;
							PlusCard = PlusCard + 4;	//保存当前加牌的数量
						}
						else
							PlusCard = PlusCard + 4;	//保存当前加牌的数量
						//DealCards(Player2, 4);
					}
				}
			}
			else if (isClockWise == true)
			{
				if (nowCard.cType == cSkip && lastcardNum != nowcardNum)
					Player2.isRound = true;

				else if (nowCard.cType == cReverse && lastcardNum != nowcardNum)
				{
					Player1.isRound = true;
					isClockWise = !isClockWise;
				}
				else
				{
					Player3.isRound = true;
					if (nowCard.cType == cPlus2)
					{
						if (PlusCardStart == false)
						{
							PlusCardStart = true;
							PlusCard = PlusCard + 2;	//保存当前加牌的数量
						}
						else
							PlusCard = PlusCard + 2;	//保存当前加牌的数量
						//DealCards(Player2, 2);
					}
					else if (nowCard.cType == cPlus4 && nowcardNum >= 0 && nowcardNum < 8)
					{
						if (PlusCardStart == false)
						{
							PlusCardStart = true;
							PlusCard = PlusCard + 4;	//保存当前加牌的数量
						}
						else
							PlusCard = PlusCard + 4;	//保存当前加牌的数量
						//DealCards(Player2, 4);
					}
				}
			}
		}
		
		isGameOver();
	}
}
void UNOGAME::DealCards(Player player, int cardCount)
{
	int cardNum;	//卡牌
	srand(time(NULL));		//用当前系统时间作为随机数生成器的种子
	for (int i = 0; i < cardCount; i++)
	{
		do
		{
			cardNum = rand() % 107;
			if(isCardTaken[cardNum] == false)
				break;
		} while (isCardTaken[cardNum] == true);
		isCardTaken[cardNum] = true;
		player.ListInsert(cardNum);
	}

	cout << "发牌结束！" << endl;
}
int  UNOGAME::CheckCardDemand(Player player)
{
	int cardOrder = 0;
	int skip = 25;	//每种颜色有25张牌

	//初始状态,功能牌无法最后出
	if (nowCard.cType == cBLACK)
		cardOrder = 1;

	//// 如果上家出了换色/+4 牌 
	//if ((nowCard.cType == cPlus4 || nowCard.cType == cChangeColor) && nowcardNum >= 0 && nowcardNum < 8)
	//{
	//	if (nowCard.cColor == cBLUE && player.ListSearch(8, 32) != 0)
	//		cardOrder = player.ListSearch(8, 32);
	//	else if (nowCard.cColor == cRED && player.ListSearch(33, 57) != 0)
	//		cardOrder = player.ListSearch(33, 57);
	//	else if (nowCard.cColor == cGREEN && player.ListSearch(58, 82) != 0)
	//		cardOrder = player.ListSearch(58, 82);
	//	else if (nowCard.cColor == cYELLOW && player.ListSearch(83, 107) != 0)
	//		cardOrder = player.ListSearch(83, 107);
	//}

	//如果有黑色的牌，则出黑色牌
	if (player.ListSearch(0, 7) != 0)
		cardOrder = player.ListSearch(0, 7);
	//如果当前要求是xx色，按0-12的顺序遍历，若有，则出xx色牌
	else if (nowCard.cColor == cBLUE && player.ListSearch(8, 32) != 0)
		cardOrder = player.ListSearch(8, 32);
	else if (nowCard.cColor == cRED && player.ListSearch(33, 57) != 0)
		cardOrder = player.ListSearch(33, 57);
	else if (nowCard.cColor == cGREEN && player.ListSearch(58, 82) != 0)
		cardOrder = player.ListSearch(58, 82);
	else if (nowCard.cColor == cYELLOW && player.ListSearch(83, 107) != 0)
		cardOrder = player.ListSearch(83, 107);

	//如果手上没有xx色的牌，按蓝红绿黄的顺序遍历，若有相同数字牌，则出相同数字的牌
	else if (nowCard.cType == cZERO && nowcardNum > 7)
	{
		for (int i = 0; i < 4; i++)
		{
			if (player.ListSearch(8 + skip * i, 8 + skip * i) != 0)
			{
				cardOrder = player.ListSearch(8 + skip * i, 8 + skip * i); break;
			}
		}
	}
	else if (nowCard.cType == cONE && nowcardNum > 7)
	{
		for (int i = 0; i < 4; i++)
		{
			if (player.ListSearch(9 + skip * i, 10 + skip * i) != 0)
			{
				cardOrder = player.ListSearch(9 + skip * i, 10 + skip * i); break;
			}
		}
	}
	else if (nowCard.cType == cTWO && nowcardNum > 7)
	{
		for (int i = 0; i < 4; i++)
		{
			if (player.ListSearch(11 + skip * i, 12 + skip * i) != 0)
			{
				cardOrder = player.ListSearch(11 + skip * i, 12 + skip * i); break;
			}
		}
	}
	else if (nowCard.cType == cTHREE && nowcardNum > 7)
	{
		for (int i = 0; i < 4; i++)
		{
			if (player.ListSearch(13 + skip * i, 14 + skip * i) != 0)
			{
				cardOrder = player.ListSearch(13 + skip * i, 14 + skip * i); break;
			}
		}
	}
	else if (nowCard.cType == cFOUR && nowcardNum > 7)
	{
		for (int i = 0; i < 4; i++)
		{
			if (player.ListSearch(15 + skip * i, 16 + skip * i) != 0)
			{
				cardOrder = player.ListSearch(15 + skip * i, 16 + skip * i); break;
			}
		}
	}
	else if (nowCard.cType == cFIVE && nowcardNum > 7)
	{
		for (int i = 0; i < 4; i++)
		{
			if (player.ListSearch(17 + skip * i, 18 + skip * i) != 0)
			{
				cardOrder = player.ListSearch(17 + skip * i, 18 + skip * i); break;
			}
		}
	}
	else if (nowCard.cType == cSIX && nowcardNum > 7)
	{
		for (int i = 0; i < 4; i++)
		{
			if (player.ListSearch(19 + skip * i, 20 + skip * i) != 0)
			{
				cardOrder = player.ListSearch(19 + skip * i, 20 + skip * i); break;
			}
		}
	}
	else if (nowCard.cType == cSEVEN && nowcardNum > 7)
	{
		for (int i = 0; i < 4; i++)
		{
			if (player.ListSearch(21 + skip * i, 22 + skip * i) != 0)
			{
				cardOrder = player.ListSearch(21 + skip * i, 22 + skip * i); break;
			}
		}
	}
	else if (nowCard.cType == cEIGHT && nowcardNum > 7)
	{
		for (int i = 0; i < 4; i++)
		{
			if (player.ListSearch(23 + skip * i, 24 + skip * i) != 0)
			{
				cardOrder = player.ListSearch(23 + skip * i, 24 + skip * i); break;
			}
		}
	}
	else if (nowCard.cType == cNINE && nowcardNum > 7)
	{
		for (int i = 0; i < 4; i++)
		{
			if (player.ListSearch(25 + skip * i, 26 + skip * i) != 0)
			{
				cardOrder = player.ListSearch(25 + skip * i, 26 + skip * i); break;
			}
		}
	}
	else if (nowCard.cType == cPlus2 && nowcardNum > 7)
	{
		for (int i = 0; i < 4; i++)
		{
			if (player.ListSearch(27 + skip * i, 28 + skip * i) != 0)
			{
				cardOrder = player.ListSearch(27 + skip * i, 28 + skip * i); break;
			}
		}
	}
	else if (nowCard.cType == cSkip && nowcardNum > 7)
	{
		for (int i = 0; i < 4; i++)
		{
			if (player.ListSearch(29 + skip * i, 30 + skip * i) != 0)
			{
				cardOrder = player.ListSearch(29 + skip * i, 30 + skip * i); break;
			}
		}
	}
	else if (nowCard.cType == cReverse && nowcardNum > 7)
	{
		for (int i = 0; i < 4; i++)
		{
			if (player.ListSearch(31 + skip * i, 32 + skip * i) != 0)
			{
				cardOrder = player.ListSearch(31 + skip * i, 32 + skip * i); break;
			}
		}
	}

	return cardOrder;
}
bool UNOGAME::HumanPlayCard()
{
	int cardOrder = 0, cardNum = -1;

	for (int i = 0; i < MAXCARDS; i++)
	{
		//通过for循环遍历isCardChosen数组，得到该牌在链表中的顺序
		if (Player1.isCardChosen[i] == true)
		{
			cardOrder = i;
			break;
		}
	}
	cardOrder++;
	if (cardOrder != 0)
		cardNum = Player1.ListLocate(cardOrder);	//得到该牌在链表中的顺序后，遍历链表得到该节点的牌的序号
	if (cardNum != -1)
	{
		if (GameStart == true)	//游戏开始时判断是否可出牌
		{
			//Player1.drawing = true;
			//Player2.drawing = false;
			//Player3.drawing = false;
			//Player4.drawing = false;
			//如果该牌的序号对应的颜色或类型满足出牌要求
			if (Cards[cardNum].cColor == cBLACK ||
				Cards[cardNum].cColor == nowCard.cColor || Cards[cardNum].cType == nowCard.cType)
			{
				//最后一张牌不能出功能牌
				if (cardNum > 0 && cardNum < 8 && Player1.ListCount() == 1)
				{
					DealCards(Player1, 1);		//发牌
					return false;
				}
				//更新上一次出的牌
				lastCard.cType = nowCard.cType;
				lastCard.cColor = nowCard.cColor;
				lastcardNum = nowcardNum;

				//改变当前出牌要求
				nowCard.cColor = Cards[cardNum].cColor;
				nowCard.cType = Cards[cardNum].cType;
				nowcardNum = cardNum;
				Player1.now_Card.cNum = cardNum;//将当前牌信息赋值给player.now_Card		
				Player1.now_Card.cType = Cards[cardNum].cType;
				Player1.now_Card.cColor = Cards[cardNum].cColor;

				//如果要出的牌不是 +2 / +4 将累加的牌发给玩家
				if (PlusCardStart == true)
				{
					if ((cardNum >= 0 && cardNum <= 3) || (cardNum >= 27 && cardNum <= 28) || (cardNum >= 52 && cardNum <= 53) || (cardNum >= 77 && cardNum <= 78) || (cardNum >= 102 && cardNum <= 103));
					else
					{
						PlusCardStart = false;
						DealCards(Player1, PlusCard);
						PlusCard = 0;
					}
				}

				if (nowCard.cColor == cBLACK)
					BtnChooseColorState = true;

				cout << Cards[cardNum].cColor << " " << Cards[cardNum].cType << endl;

				//删除这张牌，加入牌堆
				//Player1.isCardChosen[cardNum] == false;
				isCardTaken[cardNum] == false;
				Player1.ListDelete(cardOrder);
				return true;
			}
			//发牌
			else
			{
				DealCards(Player1, 1);
				return false;
			}
		}
		else if (GameStart == false)	//游戏未开始时初始化当前出牌要求
		{
			//改变当前出牌要求
			Player1.drawing = true;
			Player2.drawing = false;
			Player3.drawing = false;
			Player4.drawing = false;

			nowCard.cType = Cards[cardNum].cType;
			nowCard.cColor = Cards[cardNum].cColor;
			nowcardNum = cardNum;
			Player1.now_Card.cNum = cardNum;	//将当前牌信息赋值给player.now_Card
			Player1.now_Card.cType = Cards[cardNum].cType;
			Player1.now_Card.cColor = Cards[cardNum].cColor;

			lastCard.cType = nowCard.cType;
			lastCard.cColor = nowCard.cColor;
			lastcardNum = nowcardNum;

			if (nowCard.cColor == cBLACK)
				BtnChooseColorState = true;

			cout << Cards[cardNum].cColor << " " << Cards[cardNum].cType << endl;

			//删除这张牌，加入牌堆
			isCardTaken[cardNum] == false;
			Player1.ListDelete(cardOrder);

			GameStart = true;	//游戏开始
			return true;
		}
	}
}
bool UNOGAME::AIPlayCard(Player *player)
{
	std::cout << "AI Play Card(s) !!" << std::endl;
	int cardNum = -1;
	int cardOrder = 0;

	cardOrder = CheckCardDemand(*player);

	//更新上一次出的牌
	lastCard.cType = nowCard.cType;
	lastCard.cColor = nowCard.cColor;
	lastcardNum = nowcardNum;

	//如果找到了符合要求的牌，则出牌，否则发牌一张
	if (cardOrder != 0)
	{
		player->drawing = true;
		//改变当前出牌要求
		cardNum = player->ListLocate(cardOrder);		//得到该牌在链表中的顺序后，遍历链表得到该节点的牌的序号



		//如果要出的牌不是 +2 / +4 将累加的牌发给玩家
		if (PlusCardStart == true)
		{
			if ((cardNum >= 0 && cardNum <= 3) || (cardNum >= 27 && cardNum <= 28) || (cardNum >= 52 && cardNum <= 53) || (cardNum >= 77 && cardNum <= 78) || (cardNum >= 102 && cardNum <= 103));
			else
			{
				PlusCardStart = false;
				DealCards(*player, PlusCard);
				PlusCard = 0;
			}
		}

		nowCard.cType = Cards[cardNum].cType;
		nowCard.cColor = Cards[cardNum].cColor;
		nowcardNum = cardNum;
		player->now_Card.cType = Cards[cardNum].cType;//将当前牌信息赋值给player.now_Card
		player->now_Card.cColor = Cards[cardNum].cColor;
		player->now_Card.cNum = nowcardNum;


		cout << Cards[cardNum].cColor << " " << Cards[cardNum].cType << endl;

		//如果AI出了黑色的牌，随机选取颜色
		if (nowCard.cColor == cBLACK)
		{
			int i;
			i = rand() % 4 + 1;
			if (i == cBLUE)
			{
				nowCard.cColor = cBLUE;
				player->now_Card.cColor = cBLUE;
			}

			if (i == cRED)
			{
				nowCard.cColor = cRED;
				player->now_Card.cColor = cRED;
			}
			if (i == cGREEN)
			{
				nowCard.cColor = cGREEN;
				player->now_Card.cColor = cGREEN;
			}
			if (i == cYELLOW)
			{
				nowCard.cColor = cYELLOW;
				player->now_Card.cColor = cYELLOW;
			}
			//lastCard.cColor = cBLACK;


		}

		//删除这张牌，加入牌堆
		isCardTaken[cardOrder] = false;
		player->ListDelete(cardOrder);
		return true;
	}
	else if (cardOrder == 0)
	{
		DealCards(*player, 1);
		return false;
	}
}
void UNOGAME::isCardPressed(Vector2i mPoint)
{
	int cardNum = Player1.ListCount();


	//点击某张牌，将该牌置于被选择状态，其他牌都置于未被选择状态
	for (int cardOrder = 0; cardOrder < cardNum; cardOrder++)
	{
		if (Player1.P_Card[cardOrder].contains(mPoint.x, mPoint.y) && nowMouse != 0)
		{
			Player1.isCardChosen[cardOrder] = !Player1.isCardChosen[cardOrder];
			nowMouse = cNoPressed;
			continue;
		}
		Player1.isCardChosen[cardOrder] = false;
	}
}
void UNOGAME::isButtonPressed(Vector2i mPoint)
{
	//开始场景
	if (nowStage == cStart && nowMouse == cLPressed)
	{
		if (intStartBtn.contains(mPoint.x, mPoint.y))
		{
			cout << "Create Game!" << endl;
			nowStage = cPlaying;
			nowMouse = cNoPressed;
		}

		if (intStartRule.contains(mPoint.x, mPoint.y))
		{
			cout << "Setting!" << endl;
			nowStage = cRule;
			nowMouse = cNoPressed;
			DrawRule();
		}
	}

	//游戏场景
	if (nowStage == cPlaying && nowMouse == cLPressed)
	{
		// NoCards
		if (BtnNoCards.contains(mPoint.x, mPoint.y) && BtnNoCardsState)
		{
			nowMouse = cNoPressed;
			isNoCardsPressed = true;
		}
		// HandDown
		if (BtnHandDown.contains(mPoint.x, mPoint.y) && BtnNoCardsState)
		{
			nowMouse = cNoPressed;
		}
		// Play
		if (BtnPlay.contains(mPoint.x, mPoint.y) && BtnPlayState)
		{
			//isCardPressed(mPoint);
			nowMouse = cNoPressed;
			isPlayPressed = true;
		}
		// Uno
		if (BtnUno.contains(mPoint.x, mPoint.y) && BtnUnoState)
		{
			nowMouse = cNoPressed;
		}
		// BG
		if (BtnChooseBG.contains(mPoint.x, mPoint.y))
		{
			cout << "Background Image Changed!" << endl;
			BG += 1;
			if (BG > 8) BG = 1;
			std::stringstream ss;
			ss << "data/images/BG" << BG << ".png";
			if (!tBackground.loadFromFile(ss.str()))
				std::cout << "BGImage not found." << std::endl;
			spBackground.setTexture(tBackground);
			nowMouse = cNoPressed;
		}
		if (BtnChooseMusic.contains(mPoint.x, mPoint.y))
		{
			std::cout << "Background Music Changed!" << std::endl;
			musicNo += 1;
			if (musicNo > 4)musicNo = 1;
			std::stringstream ss;
			ss << "data/audio/Music" << musicNo << ".flac";
			if (!bgMusic.openFromFile(ss.str()))								//背景音乐
				std::cout << "BGMusic not found." << std::endl;
			bgMusic.play();
			bgMusic.setLoop(true);
			nowMouse = cNoPressed;
		}
		if (BtnSettings.contains(mPoint.x, mPoint.y))
		{
			cout << "Settings, Display Rule Scene!" << endl;
			LastStage = cPlaying;
			nowStage = cRule;
			nowMouse = cNoPressed;
		}
		//换色
		if (BtnChooseColor.contains(mPoint.x, mPoint.y) && BtnChooseColorState //可以换色，点击圆形范围内
			&& (mPoint.x - 640) * (mPoint.x - 640) + (mPoint.y - 480) * (mPoint.y - 480) <= 10000)
		{
			if (mPoint.x > 640 && mPoint.y > 480)
			{
				std::cout << "Color Changed, Blue Now!" << std::endl;
				nowCard.cColor = cBLUE;
				lastCard.cColor = cBLACK;
			}
			if (mPoint.x > 640 && mPoint.y < 480)
			{
				std::cout << "Color Changed, Red Now!" << std::endl;
				nowCard.cColor = cRED;
				lastCard.cColor = cBLACK;
			}
			if (mPoint.x < 640 && mPoint.y > 480)
			{
				std::cout << "Color Changed, Green Now!" << std::endl;
				nowCard.cColor = cGREEN;
				lastCard.cColor = cBLACK;
			}
			if (mPoint.x < 640 && mPoint.y < 480)
			{
				std::cout << "Color Changed, Yellow Now!" << std::endl;
				nowCard.cColor = cYELLOW;
				lastCard.cColor = cBLACK;
			}
			BtnChooseColorState = false;
			nowMouse = cNoPressed;
		}
		//重新开始
		if (BtnRestart.contains(mPoint.x, mPoint.y))
		{
			std::cout << "Restart!" << std::endl;
			Initial();
			nowMouse = cNoPressed;
		}
		//退出
		if (BtnQuit.contains(mPoint.x, mPoint.y))
		{
			std::cout << "Quit!" << std::endl;
			window.close();
			GameOver = false;
			GameQuit = true;
			nowMouse = cNoPressed;
		}
		//AI开始/结束
		if (BtnAIStart.contains(mPoint.x, mPoint.y)||BtnAIPause.contains(mPoint.x, mPoint.y))
		{
			AI_start = !AI_start;
			nowMouse = cNoPressed;
		}
	}
	
	//菜单场景
	if (nowStage == cRule && nowMouse == cLPressed)
	{
		if (intBack.contains(mPoint.x, mPoint.y))
		{
			cout << "Meum" << endl;
			if (LastStage == cPlaying)
				nowStage = cPlaying;
			else if (LastStage == cStart)
				nowStage = cStart;
			nowMouse = cNoPressed;
		}
	}

	//游戏结束
	if (nowStage == cGameOver && nowMouse == cLPressed)
	{
		//重新开始
		if (BtnRestart.contains(mPoint.x, mPoint.y))
		{
			std::cout << "Restart!" << std::endl;
			Initial();
			nowMouse = cNoPressed;
		}
		//退出
		if (BtnQuit.contains(mPoint.x, mPoint.y))
		{
			std::cout << "Quit!" << std::endl;
			window.close();
			GameOver = false;
			GameQuit = true;
			nowMouse = cNoPressed;
		}
	}
}
void UNOGAME::isGameOver()
{
	//任何一个玩家出完牌
	Player1.CardCount = Player1.ListCount();
	Player2.CardCount = Player2.ListCount();
	Player3.CardCount = Player3.ListCount();
	Player4.CardCount = Player4.ListCount();

	if (Player1.CardCount == 0)
	{
		GameOver = true;
		Player1.isOver = cWin;
		Player2.isOver = cLose;
		Player3.isOver = cLose;
		Player4.isOver = cLose;
		nowStage = cGameOver;
		std::cout << "Player 1 Win !!" << std::endl;
	}
	else if (Player2.CardCount == 0)
	{
		GameOver = true;
		Player1.isOver = cLose;
		Player2.isOver = cWin;
		Player3.isOver = cLose;
		Player4.isOver = cLose;
		nowStage = cGameOver;
		std::cout << "Player 2 Win !!" << std::endl;
	}
	else if (Player3.CardCount == 0)
	{
		GameOver = true;
		Player1.isOver = cLose;
		Player2.isOver = cLose;
		Player3.isOver = cWin;
		Player4.isOver = cLose;
		nowStage = cGameOver;
		std::cout << "Player 3 Win !!" << std::endl;
	}
	else if (Player4.CardCount == 0)
	{
		GameOver = true;
		Player1.isOver = cLose;
		Player2.isOver = cLose;
		Player3.isOver = cLose;
		Player4.isOver = cWin;
		nowStage = cGameOver;
		std::cout << "Player 4 Win !!" << std::endl;
	}
}
void UNOGAME::Score()
{
	Player1.Score = Player1.ScorePoint(Cards);
	Player2.Score = Player2.ScorePoint(Cards);
	Player3.Score = Player3.ScorePoint(Cards);
	Player4.Score = Player4.ScorePoint(Cards);

	if (Player1.isOver == cWin)
	{
		Player1.Score = Player2.Score + Player3.Score + Player4.Score;
		Player2.Score = -Player2.Score;
		Player3.Score = -Player3.Score;
		Player4.Score = -Player4.Score;
	}
	else if (Player2.isOver == cWin)
	{
		Player2.Score = Player1.Score + Player3.Score + Player4.Score;
		Player1.Score = -Player1.Score;
		Player3.Score = -Player3.Score;
		Player4.Score = -Player4.Score;
	}
	else if (Player3.isOver == cWin)
	{
		Player3.Score = Player1.Score + Player2.Score + Player4.Score;
		Player1.Score = -Player1.Score;
		Player2.Score = -Player2.Score;
		Player4.Score = -Player4.Score;
	}
	else if (Player4.isOver == cWin)
	{
		Player4.Score = Player1.Score + Player2.Score + Player3.Score;
		Player1.Score = -Player1.Score;
		Player2.Score = -Player2.Score;
		Player3.Score = -Player3.Score;
	}
}
void UNOGAME::logic()
{
	//检查按钮是否被按下，只有在State显示状态为能true的时候才会改变
	isButtonPressed(p_mouse);

	//游戏界面
	if (nowStage == cPlaying)
	{
		//检查玩家Player1能不能出牌，改变 Play 和 NoCard 按钮的显示状态
		if (CheckCardDemand(Player1) != 0)
		{
			BtnPlayState = true;
			BtnNoCardsState = false;
		}
		else
		{
			BtnPlayState = false;
			BtnNoCardsState = true;
		}

		if (!AI_start)
		{

			if (nowMouse == cLPressed)
				isCardPressed(p_mouse);	//检查玩家Player1的牌是不是被选择了

			//按下 Play 按钮，出牌
			if (isPlayPressed && BtnChooseColorState == false)
			{
				HumanPlayCard();	//玩家出牌

				//重置时钟和按钮点击状态
				gameClock.restart();
				isPlayPressed = false;

				//下一个玩家出牌
				Player1.isRound = false;
				if (isClockWise == false)
				{
					if (nowCard.cType == cSkip && lastcardNum != nowcardNum)
						Player3.isRound = true;
					else if (nowCard.cType == cReverse && lastcardNum != nowcardNum)
					{
						Player4.isRound = true;
						isClockWise = !isClockWise;
					}
					else
					{
						Player2.isRound = true;
						if (nowCard.cType == cPlus2)
						{
							if (PlusCardStart == false)
							{
								PlusCardStart = true;
								PlusCard = PlusCard + 2;	//保存当前加牌的数量
							}
							else
								PlusCard = PlusCard + 2;	//保存当前加牌的数量
							//DealCards(Player2, 2);
						}
						else if (nowCard.cType == cPlus4 && nowCard.cColor == cBLACK)
						{
							if (PlusCardStart == false)
							{
								PlusCardStart = true;
								PlusCard = PlusCard + 4;	//保存当前加牌的数量
							}
							else
								PlusCard = PlusCard + 4;	//保存当前加牌的数量
							//DealCards(Player2, 4);
						}
					}
				}
				else if (isClockWise == true)
				{
					if (nowCard.cType == cSkip && lastcardNum != nowcardNum)
						Player3.isRound = true;

					else if (nowCard.cType == cReverse && lastcardNum != nowcardNum)
					{
						Player2.isRound = true;
						isClockWise = !isClockWise;
					}
					else
					{
						Player4.isRound = true;
						if (nowCard.cType == cPlus2)
						{
							if (PlusCardStart == false)
							{
								PlusCardStart = true;
								PlusCard = PlusCard + 2;	//保存当前加牌的数量
							}
							else
								PlusCard = PlusCard + 2;	//保存当前加牌的数量
							//DealCards(Player2, 2);
						}
						else if (nowCard.cType == cPlus4 && nowCard.cColor == cBLACK)
						{
							if (PlusCardStart == false)
							{
								PlusCardStart = true;
								PlusCard = PlusCard + 4;	//保存当前加牌的数量
							}
							else
								PlusCard = PlusCard + 4;	//保存当前加牌的数量
							//DealCards(Player2, 4);
						}
					}
				}
			}
			//人类没牌可出，摸一张
			else if (Player1.isRound && isNoCardsPressed && BtnChooseColorState == false)
			{
				DealCards(Player1, 1);

				//重置时钟和按钮点击状态
				isNoCardsPressed = false;
				gameClock.restart();

				//下一个玩家出牌
				Player1.isRound = false;
				if (isClockWise == false)
					Player2.isRound = true;
				else if (isClockWise == true)
					Player4.isRound = true;
			}
		}


		//AI 
		if (AI_start && Player1.isRound && gameClock.getElapsedTime().asMilliseconds() > 2000 && BtnChooseColorState == false)
		{
			AIPlayCard_PD(&Player1, 1);

			//AI2:Player1多步遍历树决策，Player234单步单步决策-颜色决策
			/*
			AIPlayCard_PD(&Player1,2);
			AIPlayCard_PD(&Player2,1);	//比Player1的AIPlayCard_PD少颜色决策，因为颜色决策函数以Player1最优设计，某程序员懒得改
			AIPlayCard_PD(&Player2,1);
			AIPlayCard_PD(&Player2,1);

			*/

			//重置时钟
			gameClock.restart();

			Player1.ReSetAbleCards();
			//AI2:重置玩家234
			/*
			Player2.ReSetAbleCards();
			Player3.ReSetAbleCards();
			Player4.ReSetAbleCards();
			*/

			//下一个玩家出牌
			Player1.isRound = false;
			if (isClockWise == false)
			{
				if (nowCard.cType == cSkip && lastcardNum != nowcardNum)
					Player3.isRound = true;
				else if (nowCard.cType == cReverse && lastcardNum != nowcardNum)
				{
					Player4.isRound = true;
					isClockWise = !isClockWise;
				}
				else
				{
					Player2.isRound = true;
					if (nowCard.cType == cPlus2)
					{
						if (PlusCardStart == false)
						{
							PlusCardStart = true;
							PlusCard = PlusCard + 2;	//保存当前加牌的数量
						}
						else
							PlusCard = PlusCard + 2;	//保存当前加牌的数量
						//DealCards(Player2, 2);
					}
					else if (nowCard.cType == cPlus4 && nowcardNum >= 0 && nowcardNum < 8)
					{
						if (PlusCardStart == false)
						{
							PlusCardStart = true;
							PlusCard = PlusCard + 4;	//保存当前加牌的数量
						}
						else
							PlusCard = PlusCard + 4;	//保存当前加牌的数量
						//DealCards(Player2, 4);
					}

				}
			}
			else if (isClockWise == true)
			{
				if (nowCard.cType == cSkip && lastcardNum != nowcardNum)
					Player3.isRound = true;

				else if (nowCard.cType == cReverse && lastcardNum != nowcardNum)
				{
					Player2.isRound = true;
					isClockWise = !isClockWise;
				}
				else
				{
					Player4.isRound = true;
					if (nowCard.cType == cPlus2)
					{
						if (PlusCardStart == false)
						{
							PlusCardStart = true;
							PlusCard = PlusCard + 2;	//保存当前加牌的数量
						}
						else
							PlusCard = PlusCard + 2;	//保存当前加牌的数量
						//DealCards(Player2, 2);
					}
					else if (nowCard.cType == cPlus4 && nowcardNum >= 0 && nowcardNum < 8)
					{
						if (PlusCardStart == false)
						{
							PlusCardStart = true;
							PlusCard = PlusCard + 4;	//保存当前加牌的数量
						}
						else
							PlusCard = PlusCard + 4;	//保存当前加牌的数量
						//DealCards(Player2, 4);
					}
				}
			}
			if (GameStart == false)
				GameStart = true;
		}

		//AI出牌回合，延时2s

		//玩家2
		else if (Player2.isRound && gameClock.getElapsedTime().asMilliseconds() > 2000 && BtnChooseColorState == false)
		{
			AIPlayCard(&Player2);

			//重置时钟
			gameClock.restart();

			//下一个玩家出牌
			Player2.isRound = false;
			if (isClockWise == false)
			{
				if (nowCard.cType == cSkip && lastcardNum != nowcardNum)
					Player4.isRound = true;


				else if (nowCard.cType == cReverse && lastcardNum != nowcardNum)
				{
					Player1.isRound = true;
					isClockWise = !isClockWise;
				}
				else
				{
					Player3.isRound = true;
					if (nowCard.cType == cPlus2)
					{
						if (PlusCardStart == false)
						{
							PlusCardStart = true;
							PlusCard = PlusCard + 2;	//保存当前加牌的数量
						}
						else
							PlusCard = PlusCard + 2;	//保存当前加牌的数量
						//DealCards(Player2, 2);
					}
					else if (nowCard.cType == cPlus4 && nowcardNum >= 0 && nowcardNum < 8)
					{
						if (PlusCardStart == false)
						{
							PlusCardStart = true;
							PlusCard = PlusCard + 4;	//保存当前加牌的数量
						}
						else
							PlusCard = PlusCard + 4;	//保存当前加牌的数量
						//DealCards(Player2, 4);
					}
				}
			}
			else if (isClockWise == true)
			{
				if (nowCard.cType == cSkip && lastcardNum != nowcardNum)
					Player4.isRound = true;


				else if (nowCard.cType == cReverse && lastcardNum != nowcardNum)
				{
					Player3.isRound = true;
					isClockWise = !isClockWise;
				}
				else
				{
					Player1.isRound = true;
					if (nowCard.cType == cPlus2)
					{
						if (PlusCardStart == false)
						{
							PlusCardStart = true;
							PlusCard = PlusCard + 2;	//保存当前加牌的数量
						}
						else
							PlusCard = PlusCard + 2;	//保存当前加牌的数量
						//DealCards(Player2, 2);
					}
					else if (nowCard.cType == cPlus4 && nowcardNum >= 0 && nowcardNum < 8)
					{
						if (PlusCardStart == false)
						{
							PlusCardStart = true;
							PlusCard = PlusCard + 4;	//保存当前加牌的数量
						}
						else
							PlusCard = PlusCard + 4;	//保存当前加牌的数量
						//DealCards(Player2, 4);
					}
				}

			}
		}
		//玩家3
		else if (Player3.isRound && gameClock.getElapsedTime().asMilliseconds() > 2000 && BtnChooseColorState == false)
		{
			AIPlayCard(&Player3);

			//重置时钟
			gameClock.restart();

			//下一个玩家出牌
			Player3.isRound = false;
			if (isClockWise == false)
			{

				if (nowCard.cType == cSkip && lastcardNum != nowcardNum)
					Player1.isRound = true;

				else if (nowCard.cType == cReverse && lastcardNum != nowcardNum)
				{
					Player2.isRound = true;
					isClockWise = !isClockWise;
				}
				else
				{
					Player4.isRound = true;
					if (nowCard.cType == cPlus2)
					{
						if (PlusCardStart == false)
						{
							PlusCardStart = true;
							PlusCard = PlusCard + 2;	//保存当前加牌的数量
						}
						else
							PlusCard = PlusCard + 2;	//保存当前加牌的数量
						//DealCards(Player2, 2);
					}
					else if (nowCard.cType == cPlus4 && nowcardNum >= 0 && nowcardNum < 8)
					{
						if (PlusCardStart == false)
						{
							PlusCardStart = true;
							PlusCard = PlusCard + 4;	//保存当前加牌的数量
						}
						else
							PlusCard = PlusCard + 4;	//保存当前加牌的数量
						//DealCards(Player2, 4);
					}
				}
			}
			else if (isClockWise == true)
			{
				if (nowCard.cType == cSkip && lastcardNum != nowcardNum)
					Player1.isRound = true;

				else if (nowCard.cType == cReverse && lastcardNum != nowcardNum)
				{
					Player4.isRound = true;
					isClockWise = !isClockWise;
				}
				else
				{
					Player2.isRound = true;
					if (nowCard.cType == cPlus2)
					{
						if (PlusCardStart == false)
						{
							PlusCardStart = true;
							PlusCard = PlusCard + 2;	//保存当前加牌的数量
						}
						else
							PlusCard = PlusCard + 2;	//保存当前加牌的数量
						//DealCards(Player2, 2);
					}
					else if (nowCard.cType == cPlus4 && nowcardNum >= 0 && nowcardNum < 8)
					{
						if (PlusCardStart == false)
						{
							PlusCardStart = true;
							PlusCard = PlusCard + 4;	//保存当前加牌的数量
						}
						else
							PlusCard = PlusCard + 4;	//保存当前加牌的数量
						//DealCards(Player2, 4);
					}
				}
			}
		}
		//玩家4
		else if (Player4.isRound && gameClock.getElapsedTime().asMilliseconds() > 2000 && BtnChooseColorState == false)
		{
			AIPlayCard(&Player4);

			//重置时钟
			gameClock.restart();

			//下一个玩家出牌
			Player4.isRound = false;
			if (isClockWise == false)
			{
				if (nowCard.cType == cSkip && lastcardNum != nowcardNum)
					Player2.isRound = true;

				else if (nowCard.cType == cReverse && lastcardNum != nowcardNum)
				{
					Player3.isRound = true;
					isClockWise = !isClockWise;
				}
				else
				{
					Player1.isRound = true;
					if (nowCard.cType == cPlus2)
					{
						if (PlusCardStart == false)
						{
							PlusCardStart = true;
							PlusCard = PlusCard + 2;	//保存当前加牌的数量
						}
						else
							PlusCard = PlusCard + 2;	//保存当前加牌的数量
						//DealCards(Player2, 2);
					}
					else if (nowCard.cType == cPlus4 && nowcardNum >= 0 && nowcardNum < 8)
					{
						if (PlusCardStart == false)
						{
							PlusCardStart = true;
							PlusCard = PlusCard + 4;	//保存当前加牌的数量
						}
						else
							PlusCard = PlusCard + 4;	//保存当前加牌的数量
						//DealCards(Player2, 4);
					}
				}
			}
			else if (isClockWise == true)
			{
				if (nowCard.cType == cSkip && lastcardNum != nowcardNum)
					Player2.isRound = true;

				else if (nowCard.cType == cReverse && lastcardNum != nowcardNum)
				{
					Player1.isRound = true;
					isClockWise = !isClockWise;
				}
				else
				{
					Player3.isRound = true;
					if (nowCard.cType == cPlus2)
					{
						if (PlusCardStart == false)
						{
							PlusCardStart = true;
							PlusCard = PlusCard + 2;	//保存当前加牌的数量
						}
						else
							PlusCard = PlusCard + 2;	//保存当前加牌的数量
						//DealCards(Player2, 2);
					}
					else if (nowCard.cType == cPlus4 && nowcardNum >= 0 && nowcardNum < 8)
					{
						if (PlusCardStart == false)
						{
							PlusCardStart = true;
							PlusCard = PlusCard + 4;	//保存当前加牌的数量
						}
						else
							PlusCard = PlusCard + 4;	//保存当前加牌的数量
						//DealCards(Player2, 4);
					}
				}
			}
		}

		isGameOver();
	}
}

void UNOGAME::Draw()
{
	window.clear();
	spBackground.setPosition(0, 0);
	window.draw(spBackground);


	if (nowStage == cStart)
		DrawStart();
	else if (nowStage == cRule || isSettingsPressed)
		DrawRule();
	else if (nowStage == cPlaying)
	{
		DrawButton();
		DrawPlayerInfo();
		DrawCard();
		DrawCardOut();
		DrawCardCount();
	}
	if (GameOver)
	{
		DrawGameOver();
		DrawButton();
	}
		
	window.display();
}
void UNOGAME::DrawStart()
{
	spGameStart.setPosition(0, 0);
	window.draw(spGameStart);
	//确定三行文字的区域
	Vector2i LeftCorner;
	LeftCorner.x = (WINDOW_WIDTH - STARTBTN_W) / 2;
	LeftCorner.y = WINDOW_HEIGHT / 5 * 3;
	intStartBtn.left = LeftCorner.x;
	intStartBtn.top = LeftCorner.y;
	intStartBtn.width = STARTBTN_W;
	intStartBtn.height = STARTBTN_H;
	spStartBtn.setPosition(LeftCorner.x, LeftCorner.y);
	window.draw(spStartBtn);

	LeftCorner.y += (STARTRULE_W + STARTBTN_H) / 2;
	intStartRule.left = LeftCorner.x;
	intStartRule.top = LeftCorner.y;
	intStartRule.width = STARTRULE_W;
	intStartRule.height = STARTRULE_W;
	spStartRule.setPosition(LeftCorner.x, LeftCorner.y);
	window.draw(spStartRule);
}
void UNOGAME::DrawRule()
{
	spRuleBG.setPosition(0, 0);
	window.draw(spRuleBG);

	Vector2i LeftCorner;
	LeftCorner.x = (WINDOW_WIDTH - STARTBTN_W) / 2;
	LeftCorner.y = WINDOW_HEIGHT / 5 * 3;
	intBack.left = LeftCorner.x;
	intBack.top = LeftCorner.y;
	intBack.width = BACKBTN_W;
	intBack.height = BACKBTN_H;
	spBack.setPosition(LeftCorner.x, LeftCorner.y);
	window.draw(spBack);
}
void UNOGAME::DrawButton()
{
	//按钮左右上顶点
	Vector2i LeftCorner;
	LeftCorner.x = BUTTON1_CORNER_X;
	LeftCorner.y = BUTTON1_CORNER_Y;

	//Play按钮
	if (BtnPlayState)
		spButton.setTextureRect(IntRect(2 * BUTTON_W1, 0, BUTTON_W1, BUTTON_H1));
	else
		spButton.setTextureRect(IntRect(2 * BUTTON_W1, BUTTON_H1, BUTTON_W1, BUTTON_H1));
	spButton.setPosition(LeftCorner.x, LeftCorner.y);
	BtnPlay.left = LeftCorner.x;
	BtnPlay.top = LeftCorner.y;
	BtnPlay.width = BUTTON_W1;
	BtnPlay.height = BUTTON_H1;
	window.draw(spButton);

	//UNO按钮
	LeftCorner.x += BUTTON1_DETA_X;
	if (BtnUnoState)
		spButton.setTextureRect(IntRect(3 * BUTTON_W1, 0, BUTTON_W1, BUTTON_H1));
	else
		spButton.setTextureRect(IntRect(3 * BUTTON_W1, BUTTON_H1, BUTTON_W1, BUTTON_H1));
	spButton.setPosition(LeftCorner.x, LeftCorner.y);
	BtnUno.left = LeftCorner.x;
	BtnUno.top = LeftCorner.y;
	BtnUno.width = BUTTON_W1;
	BtnUno.height = BUTTON_H1;
	window.draw(spButton);

	//HandDown按钮
	LeftCorner.x = BUTTON1_CORNER_X;
	LeftCorner.y += BUTTON1_DETA_Y;
	if (BtnHandDownState)
		spButton.setTextureRect(IntRect(1 * BUTTON_W1, 0, BUTTON_W1, BUTTON_H1));
	else
		spButton.setTextureRect(IntRect(1 * BUTTON_W1, BUTTON_H1, BUTTON_W1, BUTTON_H1));
	spButton.setPosition(LeftCorner.x, LeftCorner.y);
	BtnHandDown.left = LeftCorner.x;
	BtnHandDown.top = LeftCorner.y;
	BtnHandDown.width = BUTTON_W1;
	BtnHandDown.height = BUTTON_H1;
	window.draw(spButton);

	//NoCard按钮按钮
	LeftCorner.x += BUTTON1_DETA_X;
	if (BtnNoCardsState)	//按下与未按下纹理不同
		spButton.setTextureRect(IntRect(0 * BUTTON_W1, 0, BUTTON_W1, BUTTON_H1));
	else
		spButton.setTextureRect(IntRect(0 * BUTTON_W1, BUTTON_H1, BUTTON_W1, BUTTON_H1));
	spButton.setPosition(LeftCorner.x, LeftCorner.y);
	BtnNoCards.left = LeftCorner.x;
	BtnNoCards.top = LeftCorner.y;
	BtnNoCards.width = BUTTON_W1;
	BtnNoCards.height = BUTTON_H1;
	window.draw(spButton);



	//Quit按钮
	LeftCorner.x = BUTTON1_CORNER_X;
	LeftCorner.y += BUTTON1_DETA_Y;
	spButton.setTextureRect(IntRect(4 * BUTTON_W1, 0, BUTTON_W1, BUTTON_H1));
	spButton.setPosition(LeftCorner.x, LeftCorner.y);
	BtnQuit.left = LeftCorner.x;
	BtnQuit.top = LeftCorner.y;
	BtnQuit.width = BUTTON_W1;
	BtnQuit.height = BUTTON_H1;
	window.draw(spButton);

	//Restart按钮
	LeftCorner.x += BUTTON1_DETA_X;
	spButton.setTextureRect(IntRect(5 * BUTTON_W1, 0, BUTTON_W1, BUTTON_H1));
	spButton.setPosition(LeftCorner.x, LeftCorner.y);
	BtnRestart.left = LeftCorner.x;
	BtnRestart.top = LeftCorner.y;
	BtnRestart.width = BUTTON_W1;
	BtnRestart.height = BUTTON_H1;
	window.draw(spButton);

	//第二类按钮
	LeftCorner.x = BUTTON2_CORNER_X;
	LeftCorner.y = BUTTON2_CORNER_Y;
	//换背景按钮
	spButton.setTextureRect(IntRect(0, 2 * BUTTON_H1, BUTTON_W2, BUTTON_H2));
	spButton.setPosition(LeftCorner.x, LeftCorner.y);
	BtnChooseBG.left = LeftCorner.x;
	BtnChooseBG.top = LeftCorner.y;
	BtnChooseBG.width = BUTTON_W2;
	BtnChooseBG.height = BUTTON_H2;
	window.draw(spButton);
	//音乐按钮
	LeftCorner.x += BUTTON2_DETA_X;
	spButton.setTextureRect(IntRect(BUTTON_W2, 2 * BUTTON_H1, BUTTON_W2, BUTTON_H2));
	spButton.setPosition(LeftCorner.x, LeftCorner.y);
	BtnChooseMusic.left = LeftCorner.x;
	BtnChooseMusic.top = LeftCorner.y;
	BtnChooseMusic.width = BUTTON_W2;
	BtnChooseMusic.height = BUTTON_H2;
	window.draw(spButton);
	//设置按钮
	LeftCorner.x += BUTTON2_DETA_X;
	spButton.setTextureRect(IntRect(2 * BUTTON_W2, 2 * BUTTON_H1, BUTTON_W2, BUTTON_H2));
	spButton.setPosition(LeftCorner.x, LeftCorner.y);
	BtnSettings.left = LeftCorner.x;
	BtnSettings.top = LeftCorner.y;
	BtnSettings.width = BUTTON_W2;
	BtnSettings.height = BUTTON_H2;
	window.draw(spButton);

	//第三类按钮
	LeftCorner.x = BUTTON3_CORNER_X;
	LeftCorner.y = BUTTON3_CORNER_Y;
	//切换颜色按钮
	spButton.setTextureRect(IntRect(0, 2 * BUTTON_H1 + BUTTON_H2, BUTTON_W3, BUTTON_H3));
	spButton.setPosition(LeftCorner.x, LeftCorner.y);
	BtnChooseColor.left = LeftCorner.x;
	BtnChooseColor.top = LeftCorner.y;
	BtnChooseColor.width = BUTTON_W3;
	BtnChooseColor.height = BUTTON_H3;
	if (BtnChooseColorState)
		window.draw(spButton);


	LeftCorner.x = P1_PHOTO_CORNER_X * 2 / 3;
	LeftCorner.y = P1_PHOTO_CORNER_Y + PLAYERBOX_H * PLAYERBOX_SCALE * 5 / 4;
	if (!AI_start)
		spButtonAI.setTextureRect(IntRect(0, 0, BTN_AI_W, BTN_AI_H));
	else
		spButtonAI.setTextureRect(IntRect(0, BTN_AI_H, BTN_AI_W, BTN_AI_H));
	spButtonAI.setPosition(LeftCorner.x, LeftCorner.y);
	BtnAIStart.left = LeftCorner.x;
	BtnAIStart.top = LeftCorner.y;
	BtnAIStart.width = BTN_AI_W;
	BtnAIStart.height = BTN_AI_H;
	window.draw(spButtonAI);

	LeftCorner.y += BTN_AI_H * 5 / 4;
	if (AI_start)
		spButtonAI.setTextureRect(IntRect(0, BTN_AI_H * 2, BTN_AI_W, BTN_AI_H ));
	else
		spButtonAI.setTextureRect(IntRect(0, BTN_AI_H * 3, BTN_AI_W, BTN_AI_H ));
	spButtonAI.setPosition(LeftCorner.x, LeftCorner.y);
	BtnAIPause.left = LeftCorner.x;
	BtnAIPause.top = LeftCorner.y;
	BtnAIPause.width = BTN_AI_W;
	BtnAIPause.height = BTN_AI_H;
	window.draw(spButtonAI);



}
void UNOGAME::DrawPlayerInfo()
{

	//Player2牌背
	for (int i = 0; i < Player2.CardCount; i++)
	{
		spCardBack.setRotation(CARD_BACK_ROTATION);
		spCardBack.setPosition(P2_CARD_CORNER_X, P2_CARD_CORNER_Y + i * 30);
		window.draw(spCardBack);
	}
	//Player3牌背
	for (int i = 0; i < Player3.CardCount; i++)
	{
		spCardBack.setRotation(2 * CARD_BACK_ROTATION);
		spCardBack.setPosition(P3_CARD_CORNER_X + i * 30, P3_CARD_CORNER_Y);
		window.draw(spCardBack);
	}
	//Player4牌背
	for (int i = 0; i < Player4.CardCount; i++)
	{
		spCardBack.setRotation(-CARD_BACK_ROTATION);
		spCardBack.setPosition(P4_CARD_CORNER_X, P4_CARD_CORNER_Y - i * 30);
		window.draw(spCardBack);
	}

	//Player1头像
	spPlayer.setTextureRect(IntRect(PLAYERBOX_W, PLAYERBOX_H, PLAYERBOX_W, PLAYERBOX_H));
	spPlayer.setPosition(P1_PHOTO_CORNER_X, P1_PHOTO_CORNER_Y);
	window.draw(spPlayer);
	//Player2头像
	spPlayer.setTextureRect(IntRect(PLAYERBOX_W, 0, PLAYERBOX_W, PLAYERBOX_H));
	spPlayer.setPosition(P2_PHOTO_CORNER_X, P2_PHOTO_CORNER_Y);
	window.draw(spPlayer);
	//Player3头像
	spPlayer.setTextureRect(IntRect(0, PLAYERBOX_H, PLAYERBOX_W, PLAYERBOX_H));
	spPlayer.setPosition(P3_PHOTO_CORNER_X, P3_PHOTO_CORNER_Y);
	window.draw(spPlayer);
	//Player4头像
	spPlayer.setTextureRect(IntRect(0, 0, PLAYERBOX_W, PLAYERBOX_H));
	spPlayer.setPosition(P4_PHOTO_CORNER_X, P4_PHOTO_CORNER_Y);
	window.draw(spPlayer);




}
void UNOGAME::DrawCard()
{
	Vector2i LeftCorner;
	int CardWidth = CARD_BACK_W;
	int CardHeight = CARD_BACK_H;
	LeftCorner.x = P1_CARD_CORNER_X;
	LeftCorner.y = P1_CARD_CORNER_Y;
	int detaX = CARD_BACK_W * 4 / 5;
	int detaY = CARD_BACK_H / 5;
	int cardNum = -1;

	Player1.CardCount = Player1.ListCount();
	int cardcount = Player1.CardCount;

	std::stringstream ss;

	for (int cardOrder = 1; cardOrder <= cardcount; cardOrder++)
	{
		ss.str("");
		cardNum = Player1.ListLocate(cardOrder);
		if (cardNum != -1)
		{
			ss << "data/images/Cards/Card" << Cards[cardNum].cColor << Cards[cardNum].cType << ".png";
			//加载图片，绑定纹理
			if (!tCard.loadFromFile(ss.str()))
				std::cout << "Card" << Cards[cardNum].cColor << Cards[cardNum].cType << " Image not found." << std::endl;
			spCard.setTexture(tCard);
			
			int CardLine = int((cardOrder - 1) / 10);
			//是否被选择，是则上移20px，否则不变
			if (Player1.isCardChosen[cardOrder - 1] == true)
				LeftCorner.y = P1_CARD_CORNER_Y + CARD_BACK_H / 3 * CardLine - detaY;
			else
				LeftCorner.y = P1_CARD_CORNER_Y + CARD_BACK_H / 3 * CardLine;
			
			//设定位置，绘制
			spCard.setPosition(LeftCorner.x, LeftCorner.y);
			window.draw(spCard);

			//记录该卡片所在位置，Player1.P_Card[cardOrder - 1]
			Player1.P_Card[cardOrder - 1].left = LeftCorner.x;
			Player1.P_Card[cardOrder - 1].top = LeftCorner.y;
			//是否是最后一张牌，是则点击区域宽度为100px，否则80
			if (cardOrder == cardcount)
				Player1.P_Card[cardOrder - 1].width = CardWidth;
			else 
				Player1.P_Card[cardOrder - 1].width = detaX;
			//下行是否还有牌，是则点击区域高度为200px，否则40
			if (cardOrder + 10 > cardcount)
				Player1.P_Card[cardOrder - 1].height = CardHeight;
			else
				Player1.P_Card[cardOrder - 1].height = detaY;

			LeftCorner.x += detaX;
			if (cardOrder % 10 == 0)
				LeftCorner.x = P1_CARD_CORNER_X;
			
		}
	}
}
void UNOGAME::DrawCardOut()
{
	Draw_playerCard(Player1);
	Draw_playerCard(Player2);
	Draw_playerCard(Player3);
	Draw_playerCard(Player4);
}
void UNOGAME::Draw_playerCard(Player player)
{
	if (GameStart == true)
	{

		Vector2i LeftCorner;
		std::stringstream ss;
		if (player.now_Card.cType == cChangeColor && player.now_Card.cNum < 8 && player.now_Card.cNum >= 0)
		{
			if (player.now_Card.cColor == cGREEN)
			{

				spChangeColor.setTextureRect(IntRect(0 * 300, 0 * 300, 300, 300));
				spChangeColor.setTexture(tChangeColor);
				//LeftCorner.x = CARDOUT_CORNER_X1 + CardOut_i * 80;
				spChangeColor.setPosition(player.CardOut_pos.x + CHANGECLOLR_OFFSET_X, player.CardOut_pos.y + CHANGECLOLR_OFFSET_Y);
				window.draw(spChangeColor);
			}
			if (player.now_Card.cColor == cRED)
			{

				spChangeColor.setTextureRect(IntRect(1 * 300, 0 * 300, 300, 300));
				spChangeColor.setTexture(tChangeColor);
				//LeftCorner.x = CARDOUT_CORNER_X1 + CardOut_i * 80;
				spChangeColor.setPosition(player.CardOut_pos.x + CHANGECLOLR_OFFSET_X, player.CardOut_pos.y + CHANGECLOLR_OFFSET_Y);
				window.draw(spChangeColor);
			}
			if (player.now_Card.cColor == cYELLOW)
			{

				spChangeColor.setTextureRect(IntRect(0 * 300, 1 * 300, 300, 300));
				spChangeColor.setTexture(tChangeColor);
				//LeftCorner.x = CARDOUT_CORNER_X1 + CardOut_i * 80;
				spChangeColor.setPosition(player.CardOut_pos.x + CHANGECLOLR_OFFSET_X, player.CardOut_pos.y + CHANGECLOLR_OFFSET_Y);
				window.draw(spChangeColor);
			}
			if (player.now_Card.cColor == cBLUE)
			{

				spChangeColor.setTextureRect(IntRect(1 * 300, 1 * 300, 300, 300));
				spChangeColor.setTexture(tChangeColor);
				//LeftCorner.x = CARDOUT_CORNER_X1 + CardOut_i * 80;
				spChangeColor.setPosition(player.CardOut_pos.x + CHANGECLOLR_OFFSET_X, player.CardOut_pos.y + CHANGECLOLR_OFFSET_Y);
				window.draw(spChangeColor);
			}
		}
		else if (player.now_Card.cType == cPlus4 && player.now_Card.cNum < 8 && player.now_Card.cNum >= 0)
		{
			if (player.now_Card.cColor == cGREEN)
			{

				spPlus4Color.setTextureRect(IntRect(0 * 300, 0 * 300, 300, 300));
				spPlus4Color.setTexture(tPlus4Color);
				//LeftCorner.x = CARDOUT_CORNER_X1 + CardOut_i * 80;
				spPlus4Color.setPosition(player.CardOut_pos.x + CHANGECLOLR_OFFSET_X, player.CardOut_pos.y + CHANGECLOLR_OFFSET_Y);
				window.draw(spPlus4Color);
			}
			if (player.now_Card.cColor == cRED)
			{

				spPlus4Color.setTextureRect(IntRect(1 * 300, 0 * 300, 300, 300));
				spPlus4Color.setTexture(tPlus4Color);
				//LeftCorner.x = CARDOUT_CORNER_X1 + CardOut_i * 80;
				spPlus4Color.setPosition(player.CardOut_pos.x + CHANGECLOLR_OFFSET_X, player.CardOut_pos.y + CHANGECLOLR_OFFSET_Y);
				window.draw(spPlus4Color);
			}
			if (player.now_Card.cColor == cYELLOW)
			{

				spPlus4Color.setTextureRect(IntRect(0 * 300, 1 * 300, 300, 300));
				spPlus4Color.setTexture(tPlus4Color);
				//LeftCorner.x = CARDOUT_CORNER_X1 + CardOut_i * 80;
				spPlus4Color.setPosition(player.CardOut_pos.x + CHANGECLOLR_OFFSET_X, player.CardOut_pos.y + CHANGECLOLR_OFFSET_Y);
				window.draw(spPlus4Color);
			}
			if (player.now_Card.cColor == cBLUE)
			{

				spPlus4Color.setTextureRect(IntRect(1 * 300, 1 * 300, 300, 300));
				spPlus4Color.setTexture(tPlus4Color);
				//LeftCorner.x = CARDOUT_CORNER_X1 + CardOut_i * 80;
				spPlus4Color.setPosition(player.CardOut_pos.x + CHANGECLOLR_OFFSET_X, player.CardOut_pos.y + CHANGECLOLR_OFFSET_Y);
				window.draw(spPlus4Color);
			}
		}
		else if (player.now_Card.cNum != -1)
		{
			ss << "data/images/Cards/Card" << player.now_Card.cColor << player.now_Card.cType << ".png";
			//加载图片，绑定纹理
			if (!tCard.loadFromFile(ss.str()))
				std::cout << "Card" << player.now_Card.cColor << player.now_Card.cType << " Image not found." << std::endl;
			spCard.setTexture(tCard);

			spCard.setPosition(player.CardOut_pos.x, player.CardOut_pos.y);
			window.draw(spCard);
			CardOut_i++;
		}
	}
}
void UNOGAME::DrawCardCount()
{
	int cardcount;
	int CountWidth = 100, CountHeight = 100;
	//绘制玩家2的手牌数
	Player2.CardCount = Player2.ListCount();
	cardcount = Player2.CardCount;
	spCardCount.setPosition(P2_PHOTO_CORNER_X, P2_PHOTO_CORNER_Y);
	spCardCount.setTextureRect(IntRect(cardcount * CountWidth, 0, CountWidth, CountWidth));
	window.draw(spCardCount);
	//绘制玩家3的手牌数
	Player3.CardCount = Player3.ListCount();
	cardcount = Player3.CardCount;
	spCardCount.setPosition(P3_PHOTO_CORNER_X, P3_PHOTO_CORNER_Y);
	spCardCount.setTextureRect(IntRect(cardcount * CountWidth, 0, CountWidth, CountWidth));
	window.draw(spCardCount);
	//绘制玩家4的手牌数
	Player3.CardCount = Player3.ListCount();
	cardcount = Player4.CardCount;
	spCardCount.setPosition(P4_PHOTO_CORNER_X, P4_PHOTO_CORNER_Y);
	spCardCount.setTextureRect(IntRect(cardcount * CountWidth, 0, CountWidth, CountWidth));
	window.draw(spCardCount);
	//绘制在谁的回合
	if (Player1.isRound)
		spCardCount.setPosition(P1_PHOTO_CORNER_X, P1_PHOTO_CORNER_Y);
	else if (Player2.isRound)
		spCardCount.setPosition(P2_PHOTO_CORNER_X, P2_PHOTO_CORNER_Y);
	else if (Player3.isRound)
		spCardCount.setPosition(P3_PHOTO_CORNER_X, P3_PHOTO_CORNER_Y);
	else if (Player4.isRound)
		spCardCount.setPosition(P4_PHOTO_CORNER_X, P4_PHOTO_CORNER_Y);
	spCardCount.setTextureRect(IntRect(0, 0, CountWidth, CountWidth));
	window.draw(spCardCount);

}
void UNOGAME::DrawGameOver()
{
	int OverWidth = 400, OverHeight = 100;
	if (Player1.isOver == cWin)
	{
		spGameOver.setTextureRect(IntRect(0, 0, OverWidth, OverHeight));
	}
	else if (Player1.isOver == cLose)
	{
		spGameOver.setTextureRect(IntRect(0, OverHeight, OverWidth, OverHeight));
	}
	spGameOver.setPosition(GAMEOVER_CORNER_X, GAMEOVER_CORNER_Y);
	window.draw(spGameOver);
}

void UNOGAME::Run()
{
	do
	{
		while (window.isOpen() && !GameOver)
		{
			Input();
			logic();
			Draw();
		}
		Input();
		isButtonPressed(p_mouse);

	} while (!GameQuit && window.isOpen());
}

void UNOGAME::CheckCardDemand1_PD(Player *player)
{
	//player.PlayerCards
	int	i = 0;
	int skip = 25;	//每种颜色有25张牌

	//初始状态,功能牌无法最后出
	if (nowCard.cColor == cBLACK)
		player->ListSearch2(0, 108);

	//如果有黑色的牌，则出黑色牌
	if (player->ListSearch2(0, 7) != 0)
	{
		player->ListSearch2(0, 7);
		//player.Func = true;
	}
	//如果当前要求是xx色，按0-12的顺序遍历，若有，则出xx色牌
	if (nowCard.cColor == cBLUE && player->ListSearch2(8, 32) != 0)
	{
		player->ListSearch2(8, 32);
		//player.SameColor = true;
	}
	if (nowCard.cColor == cRED && player->ListSearch2(33, 57) != 0)
	{
		player->ListSearch2(33, 57);
	}
	if (nowCard.cColor == cGREEN && player->ListSearch2(58, 82) != 0)
	{
		player->ListSearch2(58, 82);
	}
	if (nowCard.cColor == cYELLOW && player->ListSearch2(83, 107) != 0)
	{
		player->ListSearch2(83, 107);
	}


	//如果手上没有xx色的牌，按蓝红绿黄的顺序遍历，若有相同数字牌，则出相同数字的牌
	if (nowCard.cType == cZERO)
	{
		for (int i = 0; i < 4; i++)
		{
			if (player->ListSearch2(8 + skip * i, 8 + skip * i) != 0)
			{
				player->ListSearch2(8 + skip * i, 8 + skip * i);
				//player.SameNum = true;
			}
		}
	}
	else if (nowCard.cType == cONE)
	{
		for (int i = 0; i < 4; i++)
		{
			if (player->ListSearch2(9 + skip * i, 10 + skip * i) != 0)
			{
				player->ListSearch2(9 + skip * i, 10 + skip * i);
				//player.SameNum = true;

			}
		}
	}
	else if (nowCard.cType == cTWO)
	{
		for (int i = 0; i < 4; i++)
		{
			if (player->ListSearch2(11 + skip * i, 12 + skip * i) != 0)
			{
				player->ListSearch2(11 + skip * i, 12 + skip * i);
				//player.SameNum = true;

			}
		}
	}
	else if (nowCard.cType == cTHREE)
	{
		for (int i = 0; i < 4; i++)
		{
			if (player->ListSearch2(13 + skip * i, 14 + skip * i) != 0)
			{
				player->ListSearch2(13 + skip * i, 14 + skip * i);
				//player.SameNum = true;

			}
		}
	}
	else if (nowCard.cType == cFOUR)
	{
		for (int i = 0; i < 4; i++)
		{
			if (player->ListSearch2(15 + skip * i, 16 + skip * i) != 0)
			{
				player->ListSearch2(15 + skip * i, 16 + skip * i);
				//player.SameNum = true;

			}
		}
	}
	else if (nowCard.cType == cFIVE)
	{
		for (int i = 0; i < 4; i++)
		{
			if (player->ListSearch2(17 + skip * i, 18 + skip * i) != 0)
			{
				player->ListSearch2(17 + skip * i, 18 + skip * i);
				//player.SameNum = true;

			}
		}
	}
	else if (nowCard.cType == cSIX)
	{
		for (int i = 0; i < 4; i++)
		{
			if (player->ListSearch2(19 + skip * i, 20 + skip * i) != 0)
			{
				player->ListSearch2(19 + skip * i, 20 + skip * i);
				//player.SameNum = true;

			}
		}
	}
	else if (nowCard.cType == cSEVEN)
	{
		for (int i = 0; i < 4; i++)
		{
			if (player->ListSearch2(21 + skip * i, 22 + skip * i) != 0)
			{
				player->ListSearch2(21 + skip * i, 22 + skip * i);
				//player.SameNum = true;

			}
		}
	}
	else if (nowCard.cType == cEIGHT)
	{
		for (int i = 0; i < 4; i++)
		{
			if (player->ListSearch2(23 + skip * i, 24 + skip * i) != 0)
			{
				player->ListSearch2(23 + skip * i, 24 + skip * i);
				//player.SameNum = true;

			}
		}
	}
	else if (nowCard.cType == cNINE)
	{
		for (int i = 0; i < 4; i++)
		{
			if (player->ListSearch2(25 + skip * i, 26 + skip * i) != 0)
			{
				player->ListSearch2(25 + skip * i, 26 + skip * i);
				//player.SameNum = true;

			}
		}
	}
	else if (nowCard.cType == cPlus2)
	{
		for (int i = 0; i < 4; i++)
		{
			if (player->ListSearch2(27 + skip * i, 28 + skip * i) != 0)
			{
				player->ListSearch2(27 + skip * i, 28 + skip * i);
				//player.SameNum = true;

			}
		}
	}
	else if (nowCard.cType == cSkip)
	{
		for (int i = 0; i < 4; i++)
		{
			if (player->ListSearch2(29 + skip * i, 30 + skip * i) != 0)
			{
				player->ListSearch2(29 + skip * i, 30 + skip * i);
				//player.SameNum = true;
			}
		}
	}
	else if (nowCard.cType == cReverse)
	{
		for (int i = 0; i < 4; i++)
		{
			if (player->ListSearch2(31 + skip * i, 32 + skip * i) != 0)
			{
				player->ListSearch2(31 + skip * i, 32 + skip * i);
				//player.SameNum = true;

			}
		}
	}

	for (int j = 0; j < 30; j++)
		if (player->able[j] != -1)
			player->AbleCards[j] = Cards[player->able[j]];
}
void UNOGAME::myCards_value(Player *player)
{
	//同色>同号>黑牌
	for (int i = 1; i < 30; i++)
	{
		//给同号赋权
		if (player->AbleCards[i].cType == nowCard.cType && player->AbleCards[i].cColor != cBLACK)
			//**+4被记为黑0，可能会被记为同号，所以排除黑色情况
		{
			player->AbleCards[i].weight += 2;
		}

		//给黑牌赋权
		if (player->AbleCards[i].cColor == cBLACK)
		{
			player->AbleCards[i].weight += 6;
			//最后一张为功能牌？？0709有时最后一张功能牌能出
		}

		//给同色赋权
		//同色功能牌在下家牌好时优先
		//else if(player->AbleCards[i].cColor == nowCard.cColor && ???)
		//{
		//
		//}
		//同色数字牌>功能牌
		else if (player->AbleCards[i].cColor == nowCard.cColor && player->AbleCards[i].cType >= 10 && player->AbleCards[i].cType <= 12)
		{
			player->AbleCards[i].weight += 1;
		}

	}
}
void UNOGAME::othersChange_value(CardInfo &AbleCard)
{
	int cardOrder = 0;
	int cardNum = -1;

	if (isClockWise == false)
	{
		cardOrder = CheckCardDemand(Player2);
		if (cardOrder != 0)
		{
			AbleCard.weight++;
			cardNum = Player2.ListLocate(cardOrder);		//得到该牌在链表中的顺序后，遍历链表得到该节点的牌的序号
			nowCard = Cards[cardNum];
			cardOrder = 0;		//重置cardOrder
		}

		cardOrder = CheckCardDemand(Player3);
		if (cardOrder != 0)
		{
			AbleCard.weight++;
			cardNum = Player3.ListLocate(cardOrder);		//得到该牌在链表中的顺序后，遍历链表得到该节点的牌的序号
			nowCard = Cards[cardNum];
			cardOrder = 0;		//重置cardOrder
		}

		cardOrder = CheckCardDemand(Player4);
		if (cardOrder != 0)
		{
			AbleCard.weight++;
			cardNum = Player4.ListLocate(cardOrder);		//得到该牌在链表中的顺序后，遍历链表得到该节点的牌的序号
			nowCard = Cards[cardNum];
			cardOrder = 0;		//重置cardOrder
		}
	}
	else
	{
		cardOrder = CheckCardDemand(Player4);
		if (cardOrder != 0)
		{
			AbleCard.weight++;
			cardNum = Player4.ListLocate(cardOrder);		//得到该牌在链表中的顺序后，遍历链表得到该节点的牌的序号
			nowCard = Cards[cardNum];
			cardOrder = 0;		//重置cardOrder
		}

		cardOrder = CheckCardDemand(Player3);
		if (cardOrder != 0)
		{
			AbleCard.weight++;
			cardNum = Player3.ListLocate(cardOrder);		//得到该牌在链表中的顺序后，遍历链表得到该节点的牌的序号
			nowCard = Cards[cardNum];
			cardOrder = 0;		//重置cardOrder
		}

		cardOrder = CheckCardDemand(Player2);
		if (cardOrder != 0)
		{
			AbleCard.weight++;
			cardNum = Player2.ListLocate(cardOrder);		//得到该牌在链表中的顺序后，遍历链表得到该节点的牌的序号
			nowCard = Cards[cardNum];
			cardOrder = 0;		//重置cardOrder
		}
	}

	
}
void UNOGAME::othersChange_value2(CardInfo &AbleCard,int round)
{	
	//建立孩子兄弟表示法树
	AI2Tree->Create_tree();

	if (AbleCard.cColor != cBLANKCOLOR)
		AI2Tree->Insert_Node(AI2Tree->cbTree, AbleCard, 1, 1);

	if (isClockWise == false)
	{
		AIPlayCard_PD(&Player2, 1);
		AIPlayCard_PD(&Player3, 1);
		AIPlayCard_PD(&Player4, 1);
	}
	else
	{
		AIPlayCard_PD(&Player4, 1);
		AIPlayCard_PD(&Player3, 1);
		AIPlayCard_PD(&Player2, 1);
	}

}
int  UNOGAME::PD_DecdieCard(Player *player,int AIPlan)
{
	int i;
	int cardorder = 0;
	int outweight = 100;

	CardInfo tempCard, outCard;
	tempCard = nowCard;

	//根据自己的牌赋权
	myCards_value(player);

	for (i = 1; i < 30; i++)
	{
		//根据别人的牌赋权
		if (player->AbleCards[i].cColor == cBLACK || player->AbleCards[i].cColor == tempCard.cColor || player->AbleCards[i].cType == tempCard.cType)
		{
			nowCard = player->AbleCards[i];
			if (AIPlan == 1)
				othersChange_value(player->AbleCards[i]);
			else if (AIPlan == 2)
				othersChange_value2(player->AbleCards[i],1);
				
			

			// 取最小权的一张牌
			if (player->AbleCards[i].weight < outweight)
			{
				outweight = player->AbleCards[i].weight;
				outCard = player->AbleCards[i];
				cardorder = i;
			}
		}
	}

	//遍历完毕后恢复原先规则
	nowCard = tempCard;

	return cardorder;
}
int  UNOGAME::PD_DecdieColor()
{
	//统计各玩家手牌颜色情况
	Player1.PD_DecdieColor_CountCard();
	Player2.PD_DecdieColor_CountCard();
	Player3.PD_DecdieColor_CountCard();
	Player4.PD_DecdieColor_CountCard();

	//各色优先级 优先级越高，优先选择
	int Pr = 0, Pb = 0, Pg = 0, Py = 0;
	//给自己MaxColor赋优先级
	switch (Player1.MaxColor)
	{
	case cRED:
		Pr += 4;
		break;
	case cBLUE:
		Pb += 4;
		break;
	case cGREEN:
		Pg += 4;
		break;
	case cYELLOW:
		Py += 4;
		break;
	default:
		break;
	}
	//给玩家234MinColor赋优先级
	switch (Player2.MinColor)
	{
	case cRED:
		Pr++;
		break;
	case cBLUE:
		Pb++;
		break;
	case cGREEN:
		Pg++;
		break;
	case cYELLOW:
		Py++;
		break;
	default:
		break;
	}
	switch (Player3.MinColor)
	{
	case cRED:
		Pr++;
		break;
	case cBLUE:
		Pb++;
		break;
	case cGREEN:
		Pg++;
		break;
	case cYELLOW:
		Py++;
		break;
	default:
		break;
	}
	switch (Player4.MinColor)
	{
	case cRED:
		Pr++;
		break;
	case cBLUE:
		Pb++;
		break;
	case cGREEN:
		Pg++;
		break;
	case cYELLOW:
		Py++;
		break;
	default:
		break;
	}

	//选择优先级最高的颜色
	int MaxP = max(max(Pr, Pb), max(Pg, Py));
	if (MaxP == Pr)
		return cRED;
	else if (MaxP == Pb)
		return cBLUE;
	else if (MaxP == Pg)
		return cGREEN;
	else if (MaxP == Py)
		return cYELLOW;

}
bool UNOGAME::AIPlayCard_PD(Player *player, int AIPlan)
{
	cout << "AI Play 1 !!" << endl;
	int cardNum = -1;
	int cardorder = 0;

	lastCard = nowCard;
	lastcardNum = nowcardNum;
	//lastColor = nowCard.cColor;
	//lastType = nowType;

	//找到能出的牌
	CheckCardDemand1_PD(player);

	//遍历所有能出的牌，返回特殊值最小的牌
	//cardorder = PD_DecdieCard(player, AIPlan);
	cardorder = Tree_DecdieCard(&Player1);

	//如果有符合要求的牌，则出牌，否则发牌一张
	if (cardorder != 0)
	{
		player->drawing = true;
		//出牌实现
		cardNum = player->ListLocate(cardorder);		//得到该牌在链表中的顺序后，遍历链表得到该节点的牌的序号
		//nowType = Cards[cardNum].cType;				//*AI报错
		//nowCard.cColor = Cards[cardNum].cColor;


		//如果要出的牌不是 +2 / +4 将累加的牌发给玩家
		if (PlusCardStart == true)
		{
			if ((cardNum >= 0 && cardNum <= 3) ||
				(cardNum >= 27 && cardNum <= 28) || (cardNum >= 52 && cardNum <= 53) ||
				(cardNum >= 77 && cardNum <= 78) || (cardNum >= 102 && cardNum <= 103));
			else
			{
				PlusCardStart = false;
				DealCards(*player, PlusCard);
				PlusCard = 0;
			}
		}

		nowCard = Cards[cardNum];
		nowcardNum = cardNum;
		player->now_Card.cType = Cards[cardNum].cType;//将当前牌信息赋值给player.now_Card
		player->now_Card.cColor = Cards[cardNum].cColor;
		player->now_Card.cNum = nowcardNum;

		if (GameStart == false)
		{
			//lastColor = nowCard.cColor;
			//lastType = nowType;

			lastCard = nowCard;
			lastcardNum = nowcardNum;
		}

		cout << Cards[cardNum].cColor << " " << Cards[cardNum].cType << endl;

		//AI出黑牌，依据玩家234的颜色情况选取颜色
		if (nowCard.cColor == cBLACK)
		{
			if (player == &Player1)
			{
				//换自己多、别人少的颜色
				switch (PD_DecdieColor())
				{
				case cRED:
					nowCard.cColor = cRED;
					player->now_Card.cColor = cRED;
					break;
				case cBLUE:
					nowCard.cColor = cBLUE;
					player->now_Card.cColor = cBLUE;
					break;
				case cGREEN:
					nowCard.cColor = cGREEN;
					player->now_Card.cColor = cGREEN;
					break;
				case cYELLOW:
					nowCard.cColor = cYELLOW;
					player->now_Card.cColor = cYELLOW;
					break;
				default:
					break;
				}
			}
			else
			{
				int i = rand() % 4;
				switch (i)
				{
				case 0:
					nowCard.cColor = cRED;
					player->now_Card.cColor = cRED;
					break;
				case 1:
					nowCard.cColor = cBLUE;
					player->now_Card.cColor = cBLUE;
					break;
				case 2:
					nowCard.cColor = cGREEN;
					player->now_Card.cColor = cGREEN;
					break;
				case 3:
					nowCard.cColor = cYELLOW;
					player->now_Card.cColor = cYELLOW;
					break;
				default:
					break;
				}
			}
	
		}

		//删除这张牌，加入牌堆
		isCardTaken[cardNum] = false;
		player->ListDelete(cardorder);

		//若为玩家234，把自己的AbleCards中非空数组以结点形式接入树种，并计算胜率
		if (player == &Player2 || player == &Player3 || player == &Player4)
		{
			for (int i = 0;i < 30;i++)
			{
				//count =??
				
				if (player->AbleCards[i].cColor != cBLANKCOLOR)
					AI2Tree->Insert_Node(AI2Tree->cbTree, player->AbleCards[i], 1,1);
			}
		}

		return true;
	}
	else if (cardorder == 0)
	{
		DealCards(*player, 1);
		return false;
	}
}
//int  UNOGAME::Tree_DecdieCard(Player *player)
//{
//	int i, j = 0, changeValue = 0;
//	int cardOrder = 0, cardNum;
//	int outValue = 0;
//
//	CardInfo tempCard, outCard;
//	tempCard = nowCard;
//
//	//找到所有能出的牌
//	CheckCardDemand1_PD(player);
//
//	for (i = 1; i < 30; i++)
//	{
//		changeValue = 0;
//
//		//根据别人的牌赋权
//		if (player->AbleCards[i].cColor == cBLACK || player->AbleCards[i].cColor == tempCard.cColor || player->AbleCards[i].cType == tempCard.cType)
//		{
//			nowCard = player->AbleCards[i];
//
//			if (isClockWise == false)
//			{
//				j = PD_DecdieCard(&Player2, 1);
//				if (j != 0)	//有能出的牌
//				{
//					changeValue = changeValue - 1;
//					cardNum = Player2.ListLocate(j);
//					nowCard = Cards[cardNum];
//				}
//				else
//					changeValue = changeValue + 1;
//
//				j = PD_DecdieCard(&Player3, 1);
//				if (j != 0)	//有能出的牌
//				{
//					changeValue = changeValue - 1;
//					cardNum = Player3.ListLocate(j);
//					nowCard = Cards[cardNum];
//				}
//				else
//					changeValue = changeValue + 1;
//
//				j = PD_DecdieCard(&Player4, 1);
//				if (j != 0)	//有能出的牌
//				{
//					changeValue = changeValue - 1;
//					cardNum = Player4.ListLocate(j);
//					nowCard = Cards[cardNum];
//				}
//				else
//					changeValue = changeValue + 1;
//
//			}
//			else
//			{
//				j = PD_DecdieCard(&Player4, 1);
//				if (j != 0)	//有能出的牌
//				{
//					changeValue = changeValue - 1;
//					cardNum = Player4.ListLocate(j);
//					nowCard = Cards[cardNum];
//				}
//				else
//					changeValue = changeValue + 1;
//
//				j = PD_DecdieCard(&Player3, 1);
//				if (j != 0)	//有能出的牌
//				{
//					changeValue = changeValue - 1;
//					cardNum = Player3.ListLocate(j);
//					nowCard = Cards[cardNum];
//				}
//				else
//					changeValue = changeValue + 1;
//
//				j = PD_DecdieCard(&Player2, 1);
//				if (j != 0)	//有能出的牌
//				{
//					changeValue = changeValue - 1;
//					cardNum = Player2.ListLocate(j);
//					nowCard = Cards[cardNum];
//				}
//				else
//					changeValue = changeValue + 1;
//			}
//
//			// 取最大值的一张牌
//			if (outValue < changeValue)
//			{
//				outValue = changeValue;
//				outCard = player->AbleCards[i];
//				cardOrder = i;
//			}
//		}
//	}
//
//	nowCard = tempCard;
//	return cardOrder;
//}
int  UNOGAME::Tree_DecdieCard(Player* player)
{
	int i, j = 0, changeValue = 0;
	int cardOrder = 0, cardNum;
	int outValue = 0;
	int least_Weight = 1;

	CardInfo tempCard, outCard;
	tempCard = nowCard;

	//找到所有能出的牌
	CheckCardDemand1_PD(player);

	//找到所有能出的牌
	CheckCardDemand1_PD(player);
	//找到牌最少的玩家及其手牌数
	int least;
	Player P_leastCards;
	int P2_CardsCount = Player2.ListCount();
	int P3_CardsCount = Player2.ListCount();
	int P4_CardsCount = Player2.ListCount();
	least = P2_CardsCount >= P3_CardsCount ? P3_CardsCount : P2_CardsCount;

	least = least >= P4_CardsCount ? P4_CardsCount : least;
	if (least = P2_CardsCount)
	{
		P_leastCards = Player2;
	}
	else if (least = P3_CardsCount)
	{
		P_leastCards = Player3;
	}
	else if (least = P4_CardsCount)
	{
		P_leastCards = Player4;
	}
	if (least <= 3)
	{
		least_Weight = 4;
	}
	for (i = 1; i < 30; i++)
	{
		changeValue = 0;

		//根据别人的牌赋权
		if (player->AbleCards[i].cColor == cBLACK || player->AbleCards[i].cColor == tempCard.cColor || player->AbleCards[i].cType == tempCard.cType)
		{
			nowCard = player->AbleCards[i];

			if (isClockWise == false)
			{
				j = PD_DecdieCard(&Player2, 1);
				if (j != 0)	//有能出的牌
				{
					if (P_leastCards.role == Player2.role)
					{
						changeValue = least_Weight * (changeValue - 1);
					}
					else
					{
						changeValue = changeValue - 1;
					}
					cardNum = Player2.ListLocate(j);
					nowCard = Cards[cardNum];
				}
				else
				{
					if (P_leastCards.role == Player2.role)
					{
						changeValue = least_Weight * (changeValue + 1);
					}
					else
					{
						changeValue = changeValue + 1;
					}
				}


				j = PD_DecdieCard(&Player3, 1);
				if (j != 0)	//有能出的牌
				{
					if (P_leastCards.role == Player3.role)
					{
						changeValue = least_Weight * (changeValue - 1);
					}
					else
					{
						changeValue = changeValue - 1;
					}
					cardNum = Player3.ListLocate(j);
					nowCard = Cards[cardNum];
				}
				else
				{
					if (P_leastCards.role == Player3.role)
					{
						changeValue = least_Weight * (changeValue + 1);
					}
					else
					{
						changeValue = changeValue + 1;
					}
				}
				j = PD_DecdieCard(&Player4, 1);
				if (j != 0)	//有能出的牌
				{
					if (P_leastCards.role == Player4.role)
					{
						changeValue = least_Weight * (changeValue - 1);
					}
					else
					{
						changeValue = changeValue - 1;
					}
					cardNum = Player4.ListLocate(j);
					nowCard = Cards[cardNum];
				}
				else
				{
					if (P_leastCards.role == Player4.role)
					{
						changeValue = least_Weight * (changeValue + 1);
					}
					else
					{
						changeValue = changeValue + 1;
					}
				}
			}
			else
			{
				j = PD_DecdieCard(&Player4, 1);
				if (j != 0)	//有能出的牌
				{
					changeValue = changeValue - 1;
					cardNum = Player4.ListLocate(j);
					nowCard = Cards[cardNum];
				}
				else
					changeValue = changeValue + 1;

				j = PD_DecdieCard(&Player3, 1);
				if (j != 0)	//有能出的牌
				{
					changeValue = changeValue - 1;
					cardNum = Player3.ListLocate(j);
					nowCard = Cards[cardNum];
				}
				else
					changeValue = changeValue + 1;

				j = PD_DecdieCard(&Player2, 1);
				if (j != 0)	//有能出的牌
				{
					changeValue = changeValue - 1;
					cardNum = Player2.ListLocate(j);
					nowCard = Cards[cardNum];
				}
				else
					changeValue = changeValue + 1;
			}

			// 取最大值的一张牌
			if (outValue < changeValue)
			{
				outValue = changeValue;
				outCard = player->AbleCards[i];
				cardOrder = i;
			}
		}
	}

	nowCard = tempCard;
	return cardOrder;
}