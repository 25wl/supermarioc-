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
	void DisableConsoleInput();//关闭键盘输入
	void showScore(int score);//显示mario得分
	void showLevel(int level);//显示当前所处关卡
	void showDied(int lief);//显示生命数
	void showGameOver();//显示游戏结束动画
	void showPassed(int word);//mario过关动画显示
	void showPassedAll();//全部通关时的动画
 };
