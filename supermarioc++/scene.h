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
	int x;         
	int y;         
	int id;         
	int xAmount;    
	int yAmount;    
	double u;       
};
#endif
class scene
{
private:
	double xBg;         
	double yBg;        
	int xMap;           
	int yMap;           

	Map map[MAP_NUMBER];       
	POINT coins[COINS_NUMBER];  
	POINT score[SCORE_NUMBER];  
	POINT food[FOOD_NUMBER];   

	IMAGE img_bg;        
	IMAGE img_map;       
	IMAGE img_scenery;   
	IMAGE img_coin;      
	IMAGE img_food;      

	double scenery_iframe; 
	double coin_iframe;    
	double score_iframe[SCORE_NUMBER]; 
	double food_iframe;   

	int world; // 当前关卡号

public:
	
	scene(int world);

	
	~scene(void);

	// 获取地图数组指针，外部读取地图用
	Map* getMap() { return map; }

	POINT* getCoins() { return coins; }
	POINT* getFood() { return food; }

	
	void setScorePos(int x, int y);

	void setFood(int x, int y);

	void createMap(int world);

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