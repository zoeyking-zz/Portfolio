#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>

#define STAGE_WIDTH 20	//舞台宽度
#define STAGE_HEIGHT 20	//舞台高度
#define WINDOWS_WIDTH 50	//窗口宽度
#define WINDOWS_HEIGHT 25	//窗口高度
#define GAP_STAGE_INFO 5 //舞台与说明文字边缘宽度差
#define GRIDSIZE 25	//纹理尺寸
#define SCALE 0.5	//缩放比
#define MAXLENGTH 100	//蛇身最大长度
#define INFO_WIDTH 400
#define STEP 0.1

using namespace sf; 
using namespace std;

bool GameQuit;
bool isPause = false;
int fruitFlash;
int Level, score;
int x, y;
float stepX, stepY;
int fruitX, fruitY;
const int width = STAGE_WIDTH;
const int height = STAGE_HEIGHT;
int nTail = 1;
int tailX[MAXLENGTH], tailY[MAXLENGTH];
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir, dir_ing;
int HeadRotation;
int Delay;
bool GameOver;
bool GameMode;	//0普通动画模式,1动画细进模式
Font font;
Text text;
SoundBuffer sbEat, sbDie;
Sound SEat, SDie;
Music BGM;
int SoundVolumeBGM;
bool BGMon;


RenderWindow window(VideoMode(width*GRIDSIZE + INFO_WIDTH + GAP_STAGE_INFO,
	height*GRIDSIZE + GRIDSIZE), L"贪吃蛇 works!");
Texture tBackground, tSnakeHead, tSnakeBody, tFruit;
Sprite spBackground, spSnakeHead, spSnakeBody, spFruit;

void Initial()
{
	VideoMode mode = VideoMode::getDesktopMode();
	Vector2i p = { 0,0 };
	p.x = (mode.width - WINDOWS_WIDTH) / 2;
	window.setPosition(p);
	window.setFramerateLimit(60);	//帧频

	if (!font.loadFromFile("data/Fonts/simsun.ttc"))
		cout << "字体未找到"<< endl;
	text.setFont(font);

	if (!tBackground.loadFromFile("data/images/BK.png"))
		cout << "BK.png未找到" << endl;
	if (!tSnakeHead.loadFromFile("data/images/sh01.png"))
		cout << "sh01.png未找到" << endl;
	if (!tSnakeBody.loadFromFile("data/images/sb0102.png"))
		cout << "sb0102.png未找到" << endl;
	if (!tFruit.loadFromFile("data/images/sb0202.png"))
		cout << "sb0202.png未找到" << endl;
	
	spBackground.setTexture(tBackground);
	spSnakeHead.setTexture(tSnakeHead);
	spSnakeBody.setTexture(tSnakeBody);
	spFruit.setTexture(tFruit);

	spBackground.setOrigin(GRIDSIZE / SCALE / 2, GRIDSIZE / SCALE / 2);
	spSnakeHead.setOrigin(GRIDSIZE / SCALE / 2, GRIDSIZE / SCALE / 2);
	spSnakeBody.setOrigin(GRIDSIZE / SCALE / 2, GRIDSIZE / SCALE / 2);
	spFruit.setOrigin(GRIDSIZE / SCALE / 2, GRIDSIZE / SCALE / 2);
		
	spBackground.setScale(SCALE, SCALE);
	spSnakeHead.setScale(SCALE, SCALE);
	spSnakeBody.setScale(SCALE, SCALE);
	spFruit.setScale(SCALE, SCALE);

	if (!sbEat.loadFromFile("data/Audios/Eat01.ogg"))
		cout << "Eat01.ogg没有找到" << endl;
	if (!sbDie.loadFromFile("data/Audios/Die01.ogg"))
		cout << "Die01.ogg没有找到" << endl;
	if (!BGM.openFromFile("data/Audios/BGM01.ogg"))
		cout << "BGM01.ogg没有找到" << endl;

	SEat.setBuffer(sbEat);
	SDie.setBuffer(sbDie);
	BGM.play();
	BGM.setLoop(1);
	SoundVolumeBGM = 50;
	BGMon = 0;


	GameOver = false;
	GameQuit = false;
	fruitFlash = 0;
	Delay = 0;
	dir = STOP;
	//dir_ing = STOP;
	x = width / 2;
	y = height / 2;
	fruitX = rand() % width;
	fruitY = rand() % height;
	score = 0;
	nTail = 1;
	
	for (int i = 0; i < MAXLENGTH; i++)
	{
		tailX[i] = 0;
		tailY[i] = 0;
	}
}

void Input()
{
	Event event;

	//检测用户有无主动关闭游戏窗口
	while (window.pollEvent(event))
		if (event.type == sf::Event::Closed)
		{
			window.close();
			GameOver = 1;
			GameQuit = 1;
		}

	//模式切换
	if (event.type == Event::EventType::KeyReleased && event.key.code == sf::Keyboard::Space)
		GameMode = !GameMode;

	//游戏暂停
	if (Keyboard::isKeyPressed(Keyboard::E))
		GameQuit = !GameQuit;

	//游戏结束
	if (event.type == Event::EventType::KeyReleased && event.key.code == sf::Keyboard::Escape)
		GameOver = 1;

	//音量调整
	if (event.type == Event::EventType::KeyReleased && event.key.code == sf::Keyboard::Add)
		SoundVolumeBGM += 5;
	if (event.type == Event::EventType::KeyReleased && event.key.code == sf::Keyboard::Subtract)
		SoundVolumeBGM -= 5;

	//BGM开关
	if (event.type == Event::EventType::KeyReleased 
		&& (event.key.code == sf::Keyboard::Multiply 
			|| event.key.code == sf::Keyboard::Enter))
	{
		if(BGMon)
			BGM.play();
		else
			BGM.stop();
		BGMon = !BGMon;
	}
	
	//方向输入
	if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))
		if (dir != RIGHT)
			dir = LEFT;
	if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))
		if (dir != LEFT)
			dir = RIGHT;
	if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W))
		if (dir != DOWN)
			dir = UP;
	if (Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S))
		if (dir != UP)
			dir = DOWN;

}

void Logic()
{
	int prevX = tailX[0];
	int prevY = tailY[0];
	int prev2X, prev2Y;
	tailX[0] = x;
	tailY[0] = y;

	//蛇持续移动
	switch (dir)
	{
	case LEFT:
		x--;
		HeadRotation = -90;
		break;
	case RIGHT:
		x++;
		HeadRotation = 90;
		break;
	case UP:
		y--;
		HeadRotation = 0;
		break;
	case DOWN:
		y++;
		HeadRotation = 180;
		break;
	default:
		break;
	}

	//吃到果子
	if (x == fruitX && y == fruitY)
	{	
		SEat.play();
		score += 10;
		fruitX = rand() % width;
		fruitY = rand() % height;
		nTail++;
	}

	//吃到果子新增蛇尾后立刻调整蛇尾位置
	for (int i = 1; i < nTail; i++)
	{
		//将数值往后传递
		prev2X = tailX[i];
		prev2Y = tailY[i];
		tailX[i] = prevX;
		tailY[i] = prevY;
		prevX = prev2X;
		prevY = prev2Y;
	}

	//游戏结束
	for (int i = 1; i < nTail; i++)
		if (tailX[i] == tailX[0] && tailY[i] == tailY[0])
		{
			GameOver = true;
			SDie.play();
		}

	//蛇穿墙
	if (x >= width)  x = 0;else if (x < 0) x = width - 1;
	if (y >= height) y = 0; else if (y < 0) y = height -1;

}
void LogicStep()
{
	int prevX = tailX[0];
	int prevY = tailY[0];
	int prev2X, prev2Y;
	bool updateFlag = 0;

	//蛇持续移动
	switch (dir_ing)
	{
	case LEFT:
		stepX -= STEP;
		if (stepX < -0.99999 || stepX > 0.99999)
		{
			x--;
			stepX = 0;
			stepY = 0;
			dir_ing = dir;
			HeadRotation = -90;
			updateFlag = 1;
		}
		break;
	case RIGHT:
		stepX += STEP;
		if (stepX < -0.99999 || stepX > 0.99999)
		{
			x++;
			stepX = 0;
			stepY = 0;
			dir_ing = dir;
			HeadRotation = 90;
			updateFlag = 1;
		}
		break;
	case UP:
		stepY -= STEP;
		if (stepY < -0.99999 || stepY > 0.99999)
		{
			y--;
			stepX = 0;
			stepY = 0;
			dir_ing = dir;
			HeadRotation = 0;
			updateFlag = 1;
		}
		break;
	case DOWN:
		stepY += STEP;
		if (stepY < -0.99999 || stepY > 0.99999)
		{
			y++;
			stepX = 0;
			stepY = 0;
			dir_ing = dir;
			HeadRotation = 180;
			updateFlag = 1;
		}
		break;
	default:
		dir_ing = dir;
		break;
	}

	//蛇穿墙
	if (x >= width)  x = 0; else if (x < 0) x = width - 1;
	if (y >= height) y = 0; else if (y < 0) y = height - 1;

	tailX[0] = x;
	tailY[0] = y;

	if (updateFlag)
	{
		//吃到果子
		if (x == fruitX && y == fruitY)
		{	
			score += 10;
			fruitX = rand() % width;
			fruitY = rand() % height;
			nTail++;
		}

		//吃到果子新增蛇尾后立刻调整蛇尾位置
		for (int i = 1; i < nTail; i++)
		{
			//将数值往后传递
			prev2X = tailX[i];
			prev2Y = tailY[i];
			tailX[i] = prevX;
			tailY[i] = prevY;
			prevX = prev2X;
			prevY = prev2Y;
		}

		//游戏结束
		for (int i = 1; i < nTail; i++)
			if (tailX[i] == x && tailY[i] == y)
			{
				GameOver = true;
				SDie.play();
			}
	}
}

void Prompt_info(int _x, int _y)
{
	int InitialX = 20;
	int InitialY = 0;
	int CharacterSize = 24;
	text.setCharacterSize(CharacterSize);
	text.setFillColor(Color(0, 0, 0, 120));
	text.setStyle(Text::Bold);

	text.setPosition(_x + InitialX, _y + InitialY);
	text.setString(L"▼开始游戏：任意方向键"); window.draw(text);
	InitialY += CharacterSize * 2;


	text.setPosition(_x + InitialX, _y + InitialY);
	text.setString(L"▼游戏说明："); window.draw(text);
	InitialY += CharacterSize;

	text.setPosition(_x + InitialX, _y + InitialY);
	text.setString(L"1.蛇身自撞，游戏结束！"); window.draw(text);
	InitialY += CharacterSize;

	text.setPosition(_x + InitialX, _y + InitialY);
	text.setString(L"2.蛇身可穿墙"); window.draw(text);
	InitialY += CharacterSize * 2;
	

	text.setPosition(_x + InitialX, _y + InitialY);
	text.setString(L"▼操作说明："); window.draw(text);
	InitialY += CharacterSize;

	text.setPosition(_x + InitialX, _y + InitialY);
	text.setString(L"	1.向右移动：d"); window.draw(text);
	InitialY += CharacterSize;

	text.setPosition(_x + InitialX, _y + InitialY);
	text.setString(L"	2.向上移动：w"); window.draw(text);
	InitialY += CharacterSize;

	text.setPosition(_x + InitialX, _y + InitialY);
	text.setString(L"	3.向左移动：a"); window.draw(text);
	InitialY += CharacterSize;

	text.setPosition(_x + InitialX, _y + InitialY);
	text.setString(L"	4.向下移动：s"); window.draw(text);
	InitialY += CharacterSize * 2;
	   	

	text.setPosition(_x + InitialX, _y + InitialY);
	text.setString(L"▼动画模式切换：空格键"); window.draw(text);
	InitialY += CharacterSize;

	//模式切换提示文字
	text.setPosition(_x + InitialX, _y + InitialY);
	if (!GameMode)
	{
		text.setFillColor(Color(0, 0, 255, 255));//蓝色字体
		text.setString(L"		步进移动");
	}
	else
	{
		text.setFillColor(Color(0, 255, 0, 255));//绿色字体
		text.setString(L"		连续移动");
	}
	window.draw(text);
	InitialY += CharacterSize;

	text.setFillColor(Color(0, 0, 0, 120));
	text.setPosition(_x + InitialX, _y + InitialY);
	text.setString(L"▼背景音乐开关：*键/回车键"); window.draw(text);
	InitialY += CharacterSize;

	text.setPosition(_x + InitialX, _y + InitialY);
	text.setString(L"	增加音量：+"); window.draw(text);
	InitialY += CharacterSize;

	text.setPosition(_x + InitialX, _y + InitialY);
	text.setString(L"	减低音量：-"); window.draw(text);
	InitialY += CharacterSize * 2;

	//得分提示文字
	text.setPosition(_x + InitialX, _y + InitialY);
	text.setString(L"▼当前得分：");window.draw(text);
	text.setFillColor(Color(255, 0, 0, 100));//粉红色字体
	text.setPosition(_x + InitialX + CharacterSize * strlen("▼当前得分：") / 2, _y + InitialY - CharacterSize / 3);
	CharacterSize = 36;
	text.setCharacterSize(CharacterSize);
	stringstream ss;
	ss << score;
	text.setString(ss.str());window.draw(text);
	InitialY += CharacterSize * 2;

	CharacterSize = 24;
	text.setCharacterSize(CharacterSize);
	text.setPosition(_x + InitialX, _y + InitialY);
	text.setString(L"▼结束游戏：x"); window.draw(text);
	InitialY += CharacterSize;
	

}
void GameOver_info(int _x,int _y)
{
	int InitialX = 20, InitialY = 0;
	int CharacterSize = 48;
	text.setCharacterSize(CharacterSize);
	text.setFillColor(Color(0, 0, 255, 120));
	text.setStyle(Text::Bold);

	text.setPosition(_x + InitialX, _y + InitialY);
	text.setString(L"!!!GAMEOVER!!!"); window.draw(text);
	InitialY += CharacterSize;

	text.setPosition(_x + InitialX, _y + InitialY);
	text.setString(L"Y重新开始/N退出"); window.draw(text);

}
void Draw()
{
	window.clear(Color::Color(255,255,255,255));	//清屏
	Prompt_info((width + 1)*GRIDSIZE, GRIDSIZE);

	//偏移量
	int detaX = GRIDSIZE / SCALE / 2;
	int detaY = GRIDSIZE / SCALE / 2;

	//绘制背景
	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
		{
			spBackground.setPosition(i*GRIDSIZE + detaX, j*GRIDSIZE + detaY);
			window.draw(spBackground);
		}

	//绘制蛇
	spSnakeHead.setPosition(tailX[0] * GRIDSIZE + detaX, tailY[0] * GRIDSIZE + detaY);
	spSnakeHead.setRotation(HeadRotation);
	window.draw(spSnakeHead);

	for (int i = 1; i < nTail; i++)
	{
		spSnakeBody.setPosition(tailX[i] * GRIDSIZE + detaX, tailY[i] * GRIDSIZE + detaY);
		window.draw(spSnakeBody);
	}

	spFruit.setPosition(fruitX * GRIDSIZE + detaX, fruitY * GRIDSIZE + detaY);
	window.draw(spFruit);

	if (GameOver)
		GameOver_info(width / 8 * GRIDSIZE, height / 4 * GRIDSIZE);

	window.display();
}
void DrawStep()
{
	window.clear(Color::Color(255, 255, 255, 255));	//清屏
	
	//偏移量
	int detaX = GRIDSIZE / SCALE / 2;
	int detaY = GRIDSIZE / SCALE / 2;

	//绘制背景
	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
		{
			spBackground.setPosition(i*GRIDSIZE + detaX, j*GRIDSIZE + detaY);
			window.draw(spBackground);
		}

	//绘制蛇
	float StepLength;
	StepLength = stepX + stepY;
	if (StepLength < 0)
		StepLength = -StepLength;

	spSnakeHead.setPosition((tailX[0] + stepX) * GRIDSIZE + detaX, (tailY[0] + stepY) * GRIDSIZE + detaY);
	spSnakeHead.setRotation(HeadRotation);
	window.draw(spSnakeHead);

	//蛇身追随
	for (int i = 1; i < nTail; i++)
	{
		//左右
		if (tailX[i] != tailX[i - 1] && tailY[i] == tailY[i - 1])
			if (tailX[i] >= width )
			{
				spSnakeBody.setPosition(stepX * GRIDSIZE + detaX, (tailY[0] + stepY) * GRIDSIZE + detaY);
				window.draw(spSnakeBody);
			}
			else if (tailX[i] <= 0)
			{

			}
			else
			{
				spSnakeBody.setPosition(
					((tailX[i - 1] - tailX[i])*StepLength + tailX[i]) * GRIDSIZE + detaX,
					tailY[i] * GRIDSIZE + detaY);
			}

		//上下
		if (tailX[i] == tailX[i - 1] && tailY[i] != tailY[i - 1])
			spSnakeBody.setPosition(
				tailX[i] * GRIDSIZE + detaX,
				(tailY[i] + (tailY[i - 1] - tailY[i])*StepLength)* GRIDSIZE + detaY);
		
		window.draw(spSnakeBody);
	}

	spFruit.setPosition(fruitX * GRIDSIZE + detaX, fruitY * GRIDSIZE + detaY);
	window.draw(spFruit);

	Prompt_info((width + 1)*GRIDSIZE, GRIDSIZE);

	window.display();
}


int main()
{
	do 
	{
		Initial();

		while (window.isOpen() && !GameOver)
		{
			Input();
			switch (GameMode)
			{
			case 0:
				Delay++;
				if (Delay == 10)
				{
					Delay = 0;
					if(!GameOver)
						Logic();
				}
				Draw();
				break;
			case 1:
				if (!GameOver)
					LogicStep();
				DrawStep();
				break;
			}

		}

		if (GameOver)
		{
			Event e;
			while (window.pollEvent(e))
			{
				if (e.type == Event::Closed)
				{
					window.close();
					GameOver = 0;
					GameQuit = 1;
				}
				if (e.type == Event::EventType::KeyReleased&&e.key.code == Keyboard::Y)
					GameOver = 0;
				if (e.type == Event::EventType::KeyReleased&&e.key.code == Keyboard::N)
				{
					GameOver = 1;
					GameQuit = 1;
				}
			}
		}

	} 
	while (!GameOver);
	

	return 0;
}
