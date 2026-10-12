#include <SFML/Graphics.hpp>
#include <iostream>
#define STAGE_WIDTH 20	//舞台宽度
#define STAGE_HEIGHT 20	//舞台高度
#define WINDOWS_WIDTH 50	//窗口宽度
#define WINDOWS_HEIGHT 25	//窗口高度
#define GAP_STAGE_INFO 5 //舞台与说明文字边缘宽度差
#define GRIDSIZE 25	//纹理尺寸
#define SCALE 0.5	//缩放比
#define MAXLENGTH 100	//蛇身最大长度
#define INFO_WIDTH 400


using namespace sf; 
using namespace std;

bool gameQuit = false;
bool isPause = false;
int fruitFlash;
int Level, score;
int x, y;

int fruitX, fruitY;
const int width = STAGE_WIDTH;
const int height = STAGE_HEIGHT;
int nTail = 1;
int tailX[MAXLENGTH], tailY[MAXLENGTH];
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;
int HeadRotation;
int Delay;
bool GameOver;
Font font;
Text text;

RenderWindow window(VideoMode(width*GRIDSIZE + INFO_WIDTH, height*GRIDSIZE + GRIDSIZE), L"贪吃蛇 works!");
Texture tBackground, tSnakeHead, tSnakeBody, tFruit;
Sprite spBackground, spSnakeHead, spSnakeBody, spFruit;

void Initial()
{
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

	GameOver = false;
	fruitFlash = 0;
	Delay = 0;
	dir = STOP;
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
	while (window.pollEvent(event))
		if (event.type == sf::Event::Closed)
			window.close();

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


	switch (dir)//蛇持续移动
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


	if (x == fruitX && y == fruitY)
	{	//吃到果子
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

	for (int i = 1; i < nTail; i++)
		if (tailX[i] == tailX[0] && tailY[i] == tailY[0])
			GameOver = true;

	//蛇穿墙
	if (x >= width)  x = 0;else if (x < 0) x = width - 1;
	if (y >= height) y = 0; else if (y < 0) y = height -1;

}

void Prompt_info(int _x, int _y)
{
	int InitialX = 20;
	int InitialY = 0;
	int CharacterSize = 24;
	text.setCharacterSize(CharacterSize);
	text.setFillColor(Color(0, 0, 255, 125));
	text.setStyle(Text::Bold);

	text.setPosition(_x + InitialX, _y + InitialY);
	text.setString(L"▼游戏说明："); window.draw(text);
	InitialY += CharacterSize;


	text.setPosition(_x + InitialX, _y + InitialY);
	text.setString(L"1.蛇身自撞，游戏结束！"); window.draw(text);
	InitialY += CharacterSize;

	text.setPosition(_x + InitialX, _y + InitialY);
	text.setString(L"2.蛇身可穿墙"); window.draw(text);
	InitialY += CharacterSize;
	

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
	text.setString(L"▼开始游戏：任意方向键"); window.draw(text);
	InitialY += CharacterSize;


	text.setPosition(_x + InitialX, _y + InitialY);
	text.setString(L"▼结束游戏：x"); window.draw(text);
	InitialY += CharacterSize;
	

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

	window.display();
}



int main()
{
	
	Initial();
	while (window.isOpen() && !GameOver)
	{
		Input();
		Delay++;
		if (Delay == 5)
		{
			Delay = 0;
			Logic();
		}
		
		Draw();
	}
	

	return 0;
}
