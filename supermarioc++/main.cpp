#include"graphics.h"
#include"conio.h"
#include"MyTimer.h"
#include"mydefine.h"
#include"control.h"
#include"role.h"
#include"scene.h"
#include"windows.h"
#pragma comment(lib,"Winmm.lib")
//默认mario生命数为3条
int life = LIFE;
int world = 1;
int main()
{
	// 创建游戏窗口（宽XSIZE，高YSIZE）
	initgraph(XSIZE, YSIZE);

	// 创建游戏控制器
	control myCtrl;

	//创建角色对象
	role myRole(world);

	//创建场景对象
	scene myScene(world);
	myCtrl.DisableConsoleInput();
	//  示游戏开始界面
	myCtrl.gameStart();

	// 加载所有游戏音乐
	// 打开背景音乐
	mciSendString(_T("open res\\背景音乐.mp3 alias music_bg"), NULL, 0, NULL);
	// 打开胜利音乐
	mciSendString(_T("open res\\胜利.mp3 alias music_win"), NULL, 0, NULL);
	// 打开通关音乐
	mciSendString(_T("open res\\通关.mp3 alias music_passedAll"), NULL, 0, NULL);
	// 打开游戏结束音乐
	mciSendString(_T("open res\\游戏结束.mp3 alias music_end"), NULL, 0, NULL);
	// 循环播放背景音乐
	mciSendString(_T("play music_bg repeat"), NULL, 0, NULL);

	// 创建高精度计时器
	MyTimer mytimer;


	while (true)
	{
		// 获取玩家按下的键
		int key = myCtrl.getKey();

		// 如果玩家按了“重新开始”
		if (key == VIR_RESTART)
		{
			myScene = scene(world);
			mciSendString(_T("play music_bg from 0"), NULL, 0, NULL);
			myRole = role(world);
		}
		// 如果玩家按了“返回主菜单”
		else if (key == VIR_HOME)
		{
			mciSendString(_T("stop music_bg"), NULL, 0, NULL);
			life = LIFE;
			world = 1;
			myRole = role(world);
			myScene = scene(world);
			myCtrl.gameStart();
			mciSendString(_T("play music_bg from 0"), NULL, 0, NULL);
		}
		//更新角色逻辑
		myRole.action(key, &myScene);
		//更新场景逻辑
		myScene.action(&myRole);
		if (myRole.isDied())
		{
			mciSendString(_T("stop music_bg"), NULL, 0, NULL);

			BeginBatchDraw();
			myScene.show();
			myRole.show();
			myCtrl.showScore(myRole.getScore());
			myCtrl.showLevel(world);
			EndBatchDraw();
			mytimer.Sleep(3500);

			// 生命减1
			life--;

			// 如果生命为0,游戏结束
			if (life == 0)
			{
				// 播放游戏结束音乐
				mciSendString(_T("play music_end from 0"), NULL, 0, NULL);
				myCtrl.showGameOver();
				life = LIFE;
				world = 1;
				// 回到开始界面
				myCtrl.gameStart();
				myRole = role(world);
				myScene = scene(world);
				mciSendString(_T("play music_bg from 0"), NULL, 0, NULL);
			}
			// 如果还有生命,复活重来
			else
			{
				// 显示死亡界面
				myCtrl.showDied(life);
				myRole = role(world);
				myScene = scene(world);
				mciSendString(_T("play music_bg from 0"), NULL, 0, NULL);
			}
		}
		//判断是否过关
		if (myRole.isPassed())
		{
			// 停止背景音乐
			mciSendString(_T("stop music_bg"), NULL, 0, NULL);

			// 如果是第3关，通关全部
			if (world == 3)
			{
				// 播放胜利音乐
				mciSendString(_T("play music_win from 0"), NULL, 0, NULL);
				mytimer.Sleep(6500);
				mciSendString(_T("play music_passedAll from 0"), NULL, 0, NULL);
				myCtrl.showPassedAll();
				life = LIFE;
				world = 1;
				myRole = role(world);
				myScene = scene(world);
				myCtrl.gameStart();
				mciSendString(_T("play music_bg from 0"), NULL, 0, NULL);
			}
			// 否则进入下一关
			else
			{
				// 播放胜利音乐
				mciSendString(_T("play music_win from 0"), NULL, 0, NULL);
				mytimer.Sleep(6500);
				world++;
				myCtrl.showPassed(world);
				myRole = role(world);
				myScene = scene(world);
				mciSendString(_T("play music_bg from 0"), NULL, 0, NULL);
			}
		}

		
		//绘制整个游戏画面
		
		BeginBatchDraw();    // 开始批量绘制
		myScene.show();      // 画背景、地图、金币
		myRole.show();       // 画主角、敌人、子弹、爆炸
		myCtrl.showScore(myRole.getScore()); // 显示分数
		myCtrl.showLevel(world);            // 显示关卡
		EndBatchDraw();      // 结束绘制

		mytimer.Sleep((int)(TIME * 1000));
	}

	// 关闭所有音乐
	mciSendString(_T("close all"), NULL, 0, NULL);

	// 关闭图形窗口
	closegraph();
	return 0;
}