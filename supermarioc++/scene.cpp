#include"scene.h"
#include"graphics.h"
#include"math.h"
#include"role.h"
#include"mydefine.h"

scene::scene(int world)
{
	// 加载所有图片资源
	loadimage(&img_bg, _T("res\\mapsky.bmp"), XSIZE, 4 * YSIZE);    // 背景图
	loadimage(&img_map, _T("res\\map.bmp"));                        // 地图块图片
	loadimage(&img_scenery, _T("res\\scenery.bmp"));                // 风景装饰图
	loadimage(&img_coin, _T("res\\ani.bmp"));                       // 金币图片
	loadimage(&img_food, _T("res\\ani.bmp"));                       // 道具图片

	// 初始化背景坐标
	xBg = 0;
	yBg = 0;

	// 初始化地图坐标
	xMap = 0;
	yMap = 0;

	// 初始化动画帧
	scenery_iframe = 1;    // 风景动画帧
	coin_iframe = 1;       // 金币动画帧
	food_iframe = 1;       // 道具动画帧

	// 初始化所有地图块
	for (int i = 0; i < MAP_NUMBER; i++)
	{
		map[i].x = 0;
		map[i].y = 0;
		map[i].id = 0;
		map[i].u = 0;
		map[i].xAmount = 0;
		map[i].yAmount = 0;
	}

	// 初始化所有金币：全部清空
	for (int j = 0; j < COINS_NUMBER; j++)
	{
		coins[j].x = 0;
		coins[j].y = 0;
	}

	// 初始化得分飘字
	for (int k = 0; k < SCORE_NUMBER; k++)
	{
		score[k].x = 0;
		score[k].y = 0;
		score_iframe[k] = 0;
	}

	// 初始化所有道具
	for (int l = 0; l < FOOD_NUMBER; l++)
	{
		food[l].x = 0;
		food[l].y = 0;
	}

	// 保存当前关卡号
	this->world = world;

	// 创建金币、道具、地图
	createCoin();
	createFood();
	createMap(world);
}
scene::~scene(void)
{
}

// 判断角色是否到达终点
bool scene::isEnding(int distance)
{
	// 根据不同关卡，判断是否走到终点位置
	switch (world)
	{
	case 1:
		if (distance > 94 * WIDTH)  // 第1关终点距离
			return true;
		return false;
	//第二关
	//第三关
	}
	return false;
}
//创建地图
void scene::createMap(int world)
{
	if (world == 1) // 第1关地图
	{
		// 地图数组：x坐标、y坐标、id、宽度块数、高度块数
		Map m[] = {
			{0,9,1,15,3},{18,8,1,9,4},{27,9,1,37,3},{67,9,1,4,3},{74,9,1,1,3},{78,9,1,28,3},
			{10,6,2,4,1},{40,4,2,3,1},{62,6,2,1,1},{65,4,2,5,1},
			{36,7,10,1,1},{45,7,10,1,1},{80,7,10,1,1},{90,7,10,1,1},
			{19,6,11,1,1},{23,6,11,1,1},{32,7,11,1,1},{49,7,11,1,1},{53,7,11,1,1},{57,7,11,1,1},{93,7,11,1,1},
			{101,7,12,1,1},
			{15,10,13,1,1},{64,10,13,1,1},{71,10,13,1,1},{75,10,13,1,1}
		};

		int i = 0;
		// 把地图数据复制到类的map数组
		while (m[i].id > 0 && m[i].id < 15 && i < MAP_NUMBER)
		{
			// 根据不同地面类型，设置不同摩擦力
			switch (m[i].id)
			{
			case 1:case 3:case 4:case 5:case 6:
				m[i].u = (V_MAX / T1) / G; break; // 普通地面摩擦力
			case 2:
				m[i].u = (V_MAX / T2) / G; break; // 砖块摩擦力
			default:
				m[i].u = (V_MAX / T3) / G; break; // 其他地面
			}
			map[i] = m[i];
			i++;
		}
	}
	else if (world == 2)
	{
		//编辑地图
		Map m[] = { {18,9,2,4,1},{23,6,2,6,1},{25,3,2,3,1},{31,10,2,2,1},{39,7,2,1,1},{39,4,2,1,1},{96,7,2,2,1},{99,5,2,2,1},{102,3,2,12,1},
				 {0,10,4,15,2},{38,10,4,10,2},{50,10,4,1,2},{53,10,4,1,2},{56,10,4,1,2},{61,10,4,10,2},{75,10,4,3,2},{82,10,4,11,2},
				 {67,9,8,1,1},{68,8,8,1,2},{69,7,8,1,3},{70,6,8,1,4},{75,6,8,1,4,},{76,7,8,1,3},{77,8,8,1,2},
				 {42,8,11,1,1},
				 {111,1,12,1,1},
				 {4,8,14,1,1},{9,8,14,1,1},{83,8,14,1,1},{89,8,14,1,1},
		};
		int i = 0;
		while (m[i].id > 0 && m[i].id < 15 && i < MAP_NUMBER)
		{
			switch (m[i].id)
			{
			case 1:
			case 3:
			case 4:
			case 5:
			case 6:
				m[i].u = (V_MAX / T1) / G;
				break;
			case 2:
				m[i].u = (V_MAX / T2) / G;
				break;
			default:
				m[i].u = (V_MAX / T3) / G;
				break;
			}
			map[i] = m[i];
			i++;
		}
	}
	//第三关
}

//创建金币
void scene::createCoin()
{
	if (world == 1)
	{
		// 金币坐标数组
		POINT p[] = {
			{10,5},{11,5},{12,5},{13,5},{66,3},{67,3},{68,3},{69,3},{74,5},{74,6},{74,7},{74,8},
			{82,7},{83,8},{84,7},{85,8},{86,7},{87,8},{88,7},{89,8}
		};
		int i = 0;
		// 复制到coins数组
		while (i <= sizeof(p) / sizeof(p[0]))
		{
			coins[i].x = p[i].x;
			coins[i].y = p[i].y;
			i++;
		}
	}
	else if (world == 2)
	{
		POINT p[] = { {25,2},{26,2},{27,2},{32,4},{32,5},{32,6},{32,7},{32,8},{50,6},{53,6},{56,6} };
		int i = 0;
		while (i <= sizeof(p) / sizeof(p[0]))
		{
			coins[i].x = p[i].x;
			coins[i].y = p[i].y;
			i++;
		}
	}
	//第三关
}
//创建道具
void scene::createFood()
{
	if (world == 1)
	{
		POINT p[] = { {41 * WIDTH,8 * HEIGHT + HEIGHT / 5} };
		int i = 0;
		while (i <= sizeof(p) / sizeof(p[0]))
		{
			food[i].x = p[i].x;
			food[i].y = p[i].y;
			i++;
		}
	}
	else if (world == 2)
	{
		POINT p[] = { {39 * WIDTH - WIDTH / 3,3 * HEIGHT + HEIGHT / 5} };
		int i = 0;
		while (i <= sizeof(p) / sizeof(p[0]))
		{
			food[i].x = p[i].x;
			food[i].y = p[i].y;
			i++;
		}
	}
}

//得分飘字位置
void scene::setScorePos(int x, int y)
{
	// 找一个空的得分位置
	for (int i = 0; i < SCORE_NUMBER; i++)
	{
		if (score[i].x == 0 && score[i].y == 0)
		{
			score[i].x = x;
			score[i].y = y;
			score_iframe[i] = 1; // 开始动画
			break;
		}
	}
}
//设置道具位置
void scene::setFood(int x, int y)
{
	for (int i = 0; i < FOOD_NUMBER; i++)
	{
		if (food[i].x == 0 && food[i].y == 0)
		{
			food[i].x = x;
			food[i].y = y;
			break;
		}
	}
}
//背景滚动
void scene::action(role* mario)
{
	// 记录旧地图坐标
	int xmap = xMap;
	// 获取mario的地图偏移量
	xMap = (int)(mario->getMario())->x0;

	// 计算背景滚动速度
	double bgStep = fabs((mario->getMario())->vX) * TIME * UNREAL_HEIGHT / REAL_HEIGHT / K_MAP_BG;

	// 如果mario到达右边界，背景开始滚动（营造前进效果）
	if ((mario->getMario())->x == XRIGHT && (mario->getMario())->vX > 0 && xmap != xMap)
		xBg -= bgStep;
}
//绘制全部场景
void scene::show()
{
	// 背景循环滚动
	if (xBg <= -img_bg.getwidth())
		xBg = 0;

	// 背景Y坐标：不同关卡显示不同背景
	yBg = -(world - 1) * YSIZE;

	// 绘制背景，两张拼接实现无限滚动
	putimage((int)xBg, (int)yBg, &img_bg);
	putimage(img_bg.getwidth() + (int)xBg, (int)yBg, &img_bg);

	// 绘制地图 
	int i = 0;
	while (map[i].id > 0 && i < MAP_NUMBER)
	{
		// 循环绘制横向、纵向的地图块
		for (int j = 0; j < map[i].xAmount; j++)
			for (int k = 0; k < map[i].yAmount; k++)
			{
				// 计算地图块实际屏幕坐标
				int x = xMap + map[i].x * WIDTH + j * WIDTH;
				int y = yMap + map[i].y * HEIGHT + k * HEIGHT;
				int x0 = 0;
				int y0 = (map[i].id - 1) * HEIGHT;

				// 管道、装饰
				if (map[i].id > 9)
				{
					if (map[i].id == 10) // 管道
						putimage(x, y, 2 * WIDTH, 2 * HEIGHT, &img_map, x0, y0);
					else // 风景装饰
					{
						y0 = (map[i].id - 11) * 4 * HEIGHT;
						// 透明贴图
						putimage(x, y, 3 * WIDTH, 2 * HEIGHT, &img_scenery, ((int)scenery_iframe - 1) * 3 * WIDTH, y0 + 2 * HEIGHT, SRCAND);
						putimage(x, y, 3 * WIDTH, 2 * HEIGHT, &img_scenery, ((int)scenery_iframe - 1) * 3 * WIDTH, y0, SRCPAINT);
						// 动画帧切换
						scenery_iframe += F;
						if (scenery_iframe >= 3)
							scenery_iframe = 1;
					}
				}
				else // 普通地面、砖块
					putimage(x, y, WIDTH, HEIGHT, &img_map, x0, y0);
			}
		i++;
	}

	// 绘制金币旋转动画
	coin_iframe += TIME * 7;
	if ((int)coin_iframe == 5)
		coin_iframe = 1;

	int j = 0;
	while (j < COINS_NUMBER)
	{
		if (coins[j].x != 0)
		{
			int x = coins[j].x * WIDTH;
			int y = coins[j].y * HEIGHT;
			// 透明贴图显示金币
			putimage(xMap + x, y, WIDTH, HEIGHT, &img_coin, ((int)coin_iframe - 1) * WIDTH, 9 * HEIGHT, SRCAND);
			putimage(xMap + x, y, WIDTH, HEIGHT, &img_coin, ((int)coin_iframe - 1) * WIDTH, 8 * HEIGHT, SRCPAINT);
		}
		j++;
	}

	// 绘制得分飘字
	for (int k = 0; k < SCORE_NUMBER; k++)
	{
		if (score[k].x != 0 && score[k].y != 0)
		{
			score_iframe[k] += TIME * 8;
			if ((int)score_iframe[k] == 5) // 动画结束
			{
				score_iframe[k] = 1;
				score[k].x = 0;
				score[k].y = 0;
			}
			else
			{
				int x = (int)xMap + score[k].x * WIDTH;
				int y = score[k].y * HEIGHT;
				// 透明贴图
				putimage(x, y, WIDTH, HEIGHT, &img_coin, ((int)score_iframe[k] - 1) * WIDTH, 11 * HEIGHT, SRCAND);
				putimage(x, y, WIDTH, HEIGHT, &img_coin, ((int)score_iframe[k] - 1) * WIDTH, 10 * HEIGHT, SRCPAINT);
			}
		}
	}

	//绘制道具
	food_iframe += TIME * 7;
	if ((int)food_iframe == 3)
		food_iframe = 1;

	int l = 0;
	while (l < FOOD_NUMBER)
	{
		if (food[l].x != 0)
		{
			int x = food[l].x;
			int y = food[l].y;
			// 透明贴图显示道具
			putimage(xMap + x, y, 3 * WIDTH / 2 + 4, HEIGHT * 4 / 5, &img_food, ((int)food_iframe - 1) * (3 * WIDTH / 2 + 4), 12 * HEIGHT + HEIGHT * 4 / 5, SRCAND);
			putimage(xMap + x, y, 3 * WIDTH / 2 + 4, HEIGHT * 4 / 5, &img_food, ((int)food_iframe - 1) * (3 * WIDTH / 2 + 4), 12 * HEIGHT, SRCPAINT);
		}
		l++;
	}
}