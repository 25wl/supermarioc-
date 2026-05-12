#pragma once
#include"graphics.h"
class control
{
private:
	int controlkey;
	int GetCommand();
	void pauseClick();
public:
	control(void);
	~control(void);
	int getKey();//获取玩家按键
	void gameStart();
	void DisableConsoleInput();
	void showScore(int score);
	void showLevel(int level);
	void showDied(int lief);
	void showGameOver();//显示游戏结束动画
	void showPassed(int word);//mario过关动画显示
	void showPassedAll();//全部通关时的动画
 };
