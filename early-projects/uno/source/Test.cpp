#include "UNOGAME.h"
#include <iomanip>


void UNOGAME::testInitial()
{
	gameTime = 0;
	AITime = 0;
	isGameBegin = false;
	isTest = ISTEST;

}

void UNOGAME::isTestTime()
{
	if (isTest)
		AITime = 20;
	else
		AITime = 2000;
}

void UNOGAME::testStart()
{
	if (!isGameBegin)
	{
		isGameBegin = true;
		GBClock.restart();
	}
	if (isTest)
	{
		//默认自动托管
		AI_start = !AI_start;
		nowMouse = cNoPressed;
	}
}

void UNOGAME::testRestart()
{
	//游戏结束
	if (nowStage == cGameOver)
	{
		if (testNum < TESTSUM)
		{
			//重新开始
			std::cout << "Restart!" << std::endl;
			Initial();
			cout << "Create Game!" << endl;
			nowStage = cPlaying;

			testStart();

			testNum++;
		}
		else
		{
			testOutput_list();
		}
	}
}

void UNOGAME::testOutput(int p)
{

	std::cout << "Player " << p << " Win !!" << std::endl;

	gameTime = GBClock.getElapsedTime().asSeconds();
	std::cout << "gameTime = " << gameTime << std::endl;
	std::cout << std::endl;

	testList[testNum - 1][0] = p;
	testList[testNum - 1][1] = gameTime;

}

void UNOGAME::testOutput_list()
{
	winNum = 0;
	winPro = 0;
	std::cout << left;
	std::cout << setw(5) << "序号" << setw(1) << "|" << setw(6) << "胜者" << setw(1) << "|" << setw(6) << "时长" << endl;
	std::cout << "-------------------" << endl;

	for (int i = 1; i <= TESTSUM; i++)
	{
		//std::cout << setprecision(3) << fixed;
		std::cout << setw(5) << i << setw(1) << "|" << setw(6) << testList[i - 1][0] << setw(1) << "|" << setw(6) << testList[i - 1][1] << endl;

		if (testList[i - 1][0] == 1)
		{
			winNum++;
		}
	}
	std::cout << endl;

	winPro = winNum / TESTSUM;
	std::cout << "胜率：" << winPro << endl;
	std::cout << endl;
}