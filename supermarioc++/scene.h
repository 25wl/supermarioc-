#pragma once
#include"graphics.h"
#include"mydefine.h"
//supermario游戏场景全局变量定义
const int MAP_NUMBER = 30;
const int COINS_NUMBER = 70;
const int SCORE_NUMBER = 5;
const int FOOD_NUMBER = 5;

class role;
#ifndef _MAP
#define _MAP
// 地图块结构体
// 存储每一块地图（地面、砖块、管道等）的信息
struct Map
{
	int x;          // 地图块的 X 坐标
	int y;          // 地图块的 Y 坐标
	int id;         // 地图块类型ID
	int xAmount;    // 横向有多少个小地图块
	int yAmount;    // 纵向有多少个小地图块
	double u;       // 摩擦因数,控制角色在上面移动的阻力
};
#endif
class scene
{
private:
	double xBg;         // 背景图片 X 坐标（浮点型，平滑滚动）
	double yBg;         // 背景图片 Y 坐标
	int xMap;           // 地图整体 X 坐标
	int yMap;           // 地图整体 Y 坐标

	Map map[MAP_NUMBER];        // 地图块数组,用于存储所有地图
	POINT coins[COINS_NUMBER];  // 金币坐标数组
	POINT score[SCORE_NUMBER];  // 得分飘字坐标数组
	POINT food[FOOD_NUMBER];    // 道具坐标数组

	IMAGE img_bg;        // 背景图片对象
	IMAGE img_map;       // 地图图片对象
	IMAGE img_scenery;   // 风景装饰图片对象
	IMAGE img_coin;      // 金币图片对象
	IMAGE img_food;      // 道具图片对象

	double scenery_iframe; // 风景装饰动画帧
	double coin_iframe;    // 金币旋转动画帧
	double score_iframe[SCORE_NUMBER]; // 得分飘字动画帧
	double food_iframe;   // 道具动画帧

	int world; // 当前关卡号

public:
	// 构造函数：根据关卡号 world 初始化场景
	scene(int world);

	// 析构函数：释放资源
	~scene(void);

	// 获取地图数组指针，外部读取地图用
	Map* getMap() { return map; }

	// 获取金币数组指针
	POINT* getCoins() { return coins; }

	// 获取道具数组指针
	POINT* getFood() { return food; }

	// 设置得分飘字出现的坐标，吃金币或者踩敌人时调用
	void setScorePos(int x, int y);

	// 设置道具坐标
	void setFood(int x, int y);

	// 根据关卡号创建地图
	void createMap(int world);

	// 创建金币，即初始化金币位置
	void createCoin();

	// 创建道具，初始化道具位置
	void createFood();

	// 场景逻辑更新
	void action(role* myRole);

	// 绘制场景
	void show();

	// 判断角色是否到达终点
	bool isEnding(int distance);
};