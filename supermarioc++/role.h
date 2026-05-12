#pragma once
#include"graphics.h"
#include"mydefine.h"
const int ENEMY_TOTE = 30;
const int BOMB_NUMBER = 5;
const int BULLET_NUMBER = 30;
const int BULLET_INTERVAL = WIDTH;
const int LEHGTH_INTERVAL_BULLET = 4;
const double TIME_INTERVAL_BULLET = 0.2;

class scene;
struct Mario
{
	int x;
	int y;
	double xx;
	double yy;
	double x0;//mario原点坐标
	double vX;//水平方向速度
	double vY;//竖直方向速度
	int turn;//运动方向
	POINT direction;//移动方向点
	bool isFly;
	bool isShoot;
	bool died;
	bool ending;
	bool passed;
};
struct Enemy
{
	int x;
	int y;
	int turn;
};
struct Bullet
{
	int x;
	int y;
	int turn;//子弹飞行方向
};
#ifndef _MAP
#define _MAP
struct Map
{
	int x;
	int y;
	int id;//地图块类型
	int xAmount;//横向地图块数
	int yAmount;//纵向的总块数
	double u;//地面摩擦因数
};
#endif
class role
{
private:
	//成员变量
	Mario mario;//定义mario对象
	scene* myScene;
	Enemy myEnemy[ENEMY_TOTE];
	POINT bombs[BOMB_NUMBER];
	Bullet bullets[BULLET_NUMBER];

	IMAGE img_mario;//mario精灵图对象
	IMAGE img_enemy;//敌人的
	IMAGE img_showBomb;//爆炸效果的
	IMAGE img_bullet;//子弹的
	 
	bool shootButtonDown;
	double shootTimeInterval;
	int mario_iframe;//mario动画帧图
	double enemy_iframe;//敌人动画帧
	double bomb_iframe[BOMB_NUMBER];//爆炸动画帧
	double bullet_iframe[BULLET_NUMBER];//子弹动画帧
	int score;
	//成员函数
	//碰撞检测
	Map* touchMap(int x, int y, scene* myscene);
	POINT* touchCoins(int x, int y, scene* myScene);
	POINT* touchFood(int x, int y, scene* myscene);
	Enemy* touchEnemy(int x, int y, Enemy* emy);
	bool isTouch(POINT* p1, POINT* p2);//两点是否相碰
	void setBomb(int x, int y);
	void setBullet(int x, int y);//创建子弹
	void bulletFlying(Bullet* p, scene* myscene);//子弹飞行逻辑
public:
	role(int world);//根据关卡初始化角色
	~role(void);
	Mario* getMario()
	{
		return &mario;
	}
	bool isDied()
	{
		return mario.died;
	}
	bool isPassed()
	{
		return mario.passed;
	}
	void createEnemy(int world);
	void show();//绘制角色
	void action(int controlkey, scene* myScene);//角色行为逻辑
	int getScore()//获取当前分数
	{
		return score;
	}
};