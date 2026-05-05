#include"role.h"
#include"graphics.h"
#include"mydefine.h"
#include"math.h"
#include"rule.h"
#pragma comment(lib,"Winmm.lib")
role::role(int world)
{
	mario.x = X0;
	mario.y = X0;
	mario.xx = mario.x;
	mario.yy = mario.y;
	mario.x0 = 0;
	mario.vX = 0;
	mario.vY = 0;
	mario.turn = 1;//初始方向默认向右
	mario.direction.x = 0;
	mario.direction.y = 0;
	//状态量初始化
	mario.isFly = true;
	mario.isShoot = false;
	mario.died = false;
	mario.ending = false;
	mario.passed = false;
	//角色动画帧初始化
	myScene = 0;
	mario_iframe = 1;
	enemy_iframe = 1;

	shootButtonDown = false;
	shootTimeInterval = 0;
	score = 0;
	//加载精灵图
	loadimage(&img_mario, _T("res\\role.bmp"));
	loadimage(&img_enemy, _T("res\\ani.bmp"));
	loadimage(&img_showBomb, _T("res\\ani.bmp"));
	loadimage(&img_bullet, _T("res\\ani.bmp"));

	//初始化所有敌人
	for (int i = 0; i < ENEMY_TOTE; i++)
	{
		myEnemy[i].x = 0;
		myEnemy[i].y = 0;
		myEnemy[i].turn = 0;
	}
	// 初始化所有爆炸点
	for (int j = 0; j < BOMB_NUMBER; j++)
	{
		bombs[j].x = 0;
		bombs[j].y = 0;
		bomb_iframe[j] = 0;
	}

	// 初始化所有子弹
	for (int k = 0; k < BULLET_NUMBER; k++)
	{
		bullets[k].x = 0;
		bullets[k].y = 0;
		bullets[k].turn = 0;
		bullet_iframe[k] = 0;
	}
	// 根据关卡创建敌人
	createEnemy(world);

	// 打开所有音效
	mciSendString(_T("open res\\死亡1.mp3 alias music_died"), NULL, 0, NULL);
	mciSendString(_T("open res\\跳.mp3 alias music_jump"), NULL, 0, NULL);
	mciSendString(_T("open res\\金币.mp3 alias music_coin"), NULL, 0, NULL);
	mciSendString(_T("open res\\踩敌人.mp3 alias music_tread"), NULL, 0, NULL);
	mciSendString(_T("open res\\吃到武器.mp3 alias music_getWeapon"), NULL, 0, NULL);
	mciSendString(_T("open res\\子弹.mp3 alias music_bullet"), NULL, 0, NULL);
	mciSendString(_T("open res\\子弹撞墙.mp3 alias music_boom"), NULL, 0, NULL);
	mciSendString(_T("open res\\子弹打到敌人.mp3 alias music_boom2"), NULL, 0, NULL);
}
role::~role(void)
{

}
void role::createEnemy(int world)
{
	if (world == 1)
	{
		Enemy emy[]={{3,8,1},{18,7,-1},{25,7,1},{28,8,-1},{33,8,-1},{39,8,1},{68,3,1},{66,8,-1},{81,6,1},{92,6,1} };//敌人位置
		int i = 0;
		while (i < sizeof(emy) / sizeof(emy[0]))
		{
			myEnemy[i].x = emy[i].x * WIDTH;
			myEnemy[i].y = emy[i].y * HEIGHT;
			myEnemy[i].turn = emy[i].turn;
			i++;
	
		}
	}
	//第二关
	//第三关
}
//爆炸效果
void role::setBomb(int x, int y)
{
	for (int i = 0; i < BOMB_NUMBER; i++)
	{
		if (bombs[i].x == 0 && bombs[i].y == 0)
		{
			bombs[i].x = x;
			bombs[i].y = y;
			bomb_iframe[i] = 1;
			break;
		}
	}
}
void role::setBullet(int x, int y)
{
	// 找一个空的子弹位置
	for (int i = 0; i < BULLET_NUMBER; i++)
	{
		if (bullets[i].x == 0 && bullets[i].y == 0)
		{
			// 设置子弹坐标
			bullets[i].x = x;
			bullets[i].y = y;
			// 子弹方向 = mario当前方向
			bullets[i].turn = mario.turn;
			// 播放子弹动画
			bullet_iframe[i] = 1;
			break;
		}
	}
}
//子弹的飞行逻辑
void role::bulletFlying(Bullet* p, scene* myScene)
{
	// 如果子弹未激活，直接返回
	if (p->x == 0 && p->y == 0)
		return;

	// 如果子弹飞出屏幕，销毁
	if (p->x > XSIZE || p->x < -WIDTH)
	{
		p->x = 0;
		p->y = 0;
		p->turn = 0;
		return;
	}
	// 检测子弹是否碰到敌人或地图
	Enemy* emy = touchEnemy(p->x, p->y, myEnemy);
	Map* map = touchMap(p->x, p->y, myScene);
	// 如果碰到敌人或墙
	if (emy != NULL || map != NULL)
	{
		// 播放对应音效
		if (emy != NULL)
			mciSendString(_T("play music_boom2 from 0"), NULL, 0, NULL);
		else
			mciSendString(_T("play music_boom from 0"), NULL, 0, NULL);

		score += 5; // 加分
		int x, y;
		// 如果打到敌人
		if (emy != NULL)
		{
			x = emy->x;
			y = emy->y;
			// 销毁敌人
			emy->x = 0;
			emy->y = 0;
			emy->turn = 0;
			// 销毁子弹
			p->x = 0;
			p->y = 0;
			p->turn = 0;
		}
		else // 打到墙
		{
			x = (-(int)mario.x0 + p->x) / WIDTH * WIDTH;
			if (p->turn == -1)
				x = (-(int)mario.x0 + p->x + WIDTH) / WIDTH * WIDTH;
			y = p->y;
			// 销毁子弹
			p->x = 0;
			p->y = 0;
			p->turn = 0;
		}
		setBomb(x, y); // 播放爆炸
	}
	else // 没碰到，继续飞
		p->x += LEHGTH_INTERVAL_BULLET * p->turn;
}
//两个矩形是否接触
bool role::isTouch(POINT* p1, POINT* p2)
{
	// 主角4个角任意一个在目标内 → 碰撞
	for (int i = 0; i < 4; i++)
	{
		if (p1[i].x >= p2[0].x && p1[i].y >= p2[0].y && p1[i].x <= p2[1].x && p1[i].y <= p2[1].y)
		{
			return true;
		}
	}
	return false;
}
//检测是否碰到地图块
Map* role::touchMap(int x, int y, scene* myScene)
{
	int xmap = (int)mario.x0;
	POINT r[4]; // 主角4个顶点（缩小一圈，防止误触）

	r[0].x = -xmap + x + 1;
	r[0].y = y + 1;
	r[1].x = -xmap + x + WIDTH - 1;
	r[1].y = y + 1;
	r[2].x = -xmap + x + 1;
	r[2].y = y + HEIGHT - 1;
	r[3].x = -xmap + x + WIDTH - 1;
	r[3].y = y + HEIGHT - 1;

	int i = 0;
	// 遍历所有地图块
	while (myScene->getMap()[i].id > 0 && myScene->getMap()[i].id < 11 && i < MAP_NUMBER)
	{
		POINT m[2]; // 地图块的左上角、右下角
		m[0].x = myScene->getMap()[i].x * WIDTH;
		m[0].y = myScene->getMap()[i].y * HEIGHT;

		// 特殊地图块更大
		if (myScene->getMap()[i].id == 10)
		{
			m[1].x = myScene->getMap()[i].x * WIDTH + myScene->getMap()[i].xAmount * 2 * WIDTH;
			m[1].y = myScene->getMap()[i].y * HEIGHT + myScene->getMap()[i].yAmount * 2 * HEIGHT;
		}
		else
		{
			m[1].x = myScene->getMap()[i].x * WIDTH + myScene->getMap()[i].xAmount * WIDTH;
			m[1].y = myScene->getMap()[i].y * HEIGHT + myScene->getMap()[i].yAmount * HEIGHT;
		}

		if (isTouch(r, m)) // 碰撞
			return &(myScene->getMap()[i]);
		i++;
	}
	return NULL; // 没碰到
}
//检测是否吃到金币
POINT* role::touchCoins(int x, int y, scene* myScene)
{
	int xmap = (int)mario.x0;
	POINT r[4]; // 主角4个顶点

	r[0].x = -xmap + x + 1;
	r[0].y = y + 1;
	r[1].x = -xmap + x + WIDTH - 1;
	r[1].y = y + 1;
	r[2].x = -xmap + x + 1;
	r[2].y = y + HEIGHT - 1;
	r[3].x = -xmap + x + WIDTH - 1;
	r[3].y = y + HEIGHT - 1;

	int i = 0;
	while (i < COINS_NUMBER)
	{
		POINT m[2];
		m[0].x = myScene->getCoins()[i].x * WIDTH;
		m[0].y = myScene->getCoins()[i].y * HEIGHT;
		m[1].x = m[0].x + WIDTH;
		m[1].y = m[0].y + HEIGHT;

		if (isTouch(r, m)) // 碰到金币
			return &(myScene->getCoins()[i]);
		i++;
	}
	return NULL;
}
//检测是否吃到道具
POINT* role::touchFood(int x, int y, scene* myScene)
{
	int xmap = (int)mario.x0;
	POINT r[4];

	r[0].x = -xmap + x + 1;
	r[0].y = y + 1;
	r[1].x = -xmap + x + WIDTH - 1;
	r[1].y = y + 1;
	r[2].x = -xmap + x + 1;
	r[2].y = y + HEIGHT - 1;
	r[3].x = -xmap + x + WIDTH - 1;
	r[3].y = y + HEIGHT - 1;

	int i = 0;
	while (i < FOOD_NUMBER)
	{
		POINT m[2];
		m[0].x = myScene->getFood()[i].x;
		m[0].y = myScene->getFood()[i].y;
		m[1].x = m[0].x + 3 * WIDTH / 2 + 4;
		m[1].y = m[0].y + 4 * HEIGHT / 5;

		if (isTouch(r, m)) // 碰到道具
			return &(myScene->getFood()[i]);
		i++;
	}
	return NULL;
}
//检测是否碰到敌人
Enemy* role::touchEnemy(int x, int y, Enemy* emy)
{
	int xmap = (int)mario.x0;
	POINT r[4];

	r[0].x = -xmap + x + 1;
	r[0].y = y + 1;
	r[1].x = -xmap + x + WIDTH - 1;
	r[1].y = y + 1;
	r[2].x = -xmap + x + 1;
	r[2].y = y + HEIGHT - 1;
	r[3].x = -xmap + x + WIDTH - 1;
	r[3].y = y + HEIGHT - 1;

	int i = 0;
	while (i < ENEMY_TOTE)
	{
		if (emy[i].turn != 0) // 敌人激活
		{
			POINT m[2];
			m[0].x = emy[i].x;
			m[0].y = emy[i].y;
			m[1].x = m[0].x + WIDTH;
			m[1].y = m[0].y + HEIGHT;

			if (isTouch(r, m)) // 碰到敌人
			{
				return &emy[i];
			}
		}
		i++;
	}
	return NULL;
}
//绘制全部的role
void role::show()
{
	// 控制主角走路动画帧
	int rolePos = -(int)mario.x0 + mario.x;
	if (rolePos / STEP % 2 == 0 && rolePos / STEP % 4 != 0)
		mario_iframe = 2;
	else if (rolePos / STEP % 4 == 0)
		mario_iframe = 1;

	// 控制主角方向
	if (mario.direction.x == 1)
		mario.turn = 1;
	else if (mario.direction.x == -1)
		mario.turn = -1;

	// 如果主角死亡
	if (mario.died == true)
	{
		putimage(mario.x, mario.y, WIDTH, HEIGHT, &img_mario, 2 * WIDTH, HEIGHT, SRCAND);
		putimage(mario.x, mario.y, WIDTH, HEIGHT, &img_mario, 2 * WIDTH, 0, SRCPAINT);
	}
	else // 正常显示
	{
		if (mario.turn == 1) // 向右
		{
			putimage(mario.x, mario.y, WIDTH, HEIGHT, &img_mario, (mario_iframe - 1) * WIDTH, HEIGHT, SRCAND);
			putimage(mario.x, mario.y, WIDTH, HEIGHT, &img_mario, (mario_iframe - 1) * WIDTH, 0, SRCPAINT);
		}
		else // 向左
		{
			putimage(mario.x, mario.y, WIDTH, HEIGHT, &img_mario, (mario_iframe - 1) * WIDTH + 3 * WIDTH, HEIGHT, SRCAND);
			putimage(mario.x, mario.y, WIDTH, HEIGHT, &img_mario, (mario_iframe - 1) * WIDTH + 3 * WIDTH, 0, SRCPAINT);
		}
	}

	// 敌人动画帧
	enemy_iframe += TIME * 5;
	if ((int)enemy_iframe == 3)
		enemy_iframe = 1;

	// 绘制敌人
	int i = 0;
	while (i < ENEMY_TOTE)
	{
		if (myEnemy[i].turn != 0)
		{
			putimage((int)mario.x0 + myEnemy[i].x, myEnemy[i].y, WIDTH, HEIGHT, &img_enemy, ((int)enemy_iframe - 1) * WIDTH, HEIGHT, SRCAND);
			putimage((int)mario.x0 + myEnemy[i].x, myEnemy[i].y, WIDTH, HEIGHT, &img_enemy, ((int)enemy_iframe - 1) * WIDTH, 0, SRCPAINT);
		}
		i++;
	}

	// 绘制爆炸
	for (int j = 0; j < BOMB_NUMBER; j++)
	{
		if (bombs[j].x != 0 && bombs[j].y != 0)
		{
			bomb_iframe[j] += TIME * 10;
			if ((int)bomb_iframe[j] == 5) // 动画结束
			{
				bomb_iframe[j] = 1;
				bombs[j].x = 0;
				bombs[j].y = 0;
			}
			else // 播放爆炸
			{
				putimage((int)mario.x0 + bombs[j].x - WIDTH / 2, bombs[j].y - HEIGHT / 2, 2 * WIDTH, 2 * HEIGHT, &img_showBomb, ((int)bomb_iframe[j] - 1) * 2 * WIDTH, 6 * HEIGHT, SRCAND);
				putimage((int)mario.x0 + bombs[j].x - WIDTH / 2, bombs[j].y - HEIGHT / 2, 2 * WIDTH, 2 * HEIGHT, &img_showBomb, ((int)bomb_iframe[j] - 1) * 2 * WIDTH, 4 * HEIGHT, SRCPAINT);
			}
		}
	}

	// 绘制子弹
	for (int k = 0; k < BULLET_NUMBER; k++)
	{
		if (bullets[k].x != 0 && bullets[k].y != 0)
		{
			bullet_iframe[k] += TIME * 10;
			if ((int)bullet_iframe[k] == 3)
			{
				bullet_iframe[k] = 1;
			}
			else
			{
				putimage(bullets[k].x, bullets[k].y, WIDTH, HEIGHT, &img_bullet, ((int)bullet_iframe[k] - 1) * WIDTH, 3 * HEIGHT, SRCAND);
				putimage(bullets[k].x, bullets[k].y, WIDTH, HEIGHT, &img_bullet, ((int)bullet_iframe[k] - 1) * WIDTH, 2 * HEIGHT, SRCPAINT);
			}
			bulletFlying(&bullets[k], myScene); // 子弹飞行
		}
	}
}
// 角色行为逻辑
void role::action(int KEY, scene* myScene)
{
	this->myScene = myScene;
	// 清空方向
	mario.direction.x = 0;
	mario.direction.y = 0;

	double a = 0;   // 加速度
	double a1 = 0;  // 摩擦加速度
	Map* map = NULL;

	//跳跃逻辑
	if ((KEY & CMD_UP) && mario.isFly == false && mario.ending == false)
	{
		mciSendString(_T("play music_jump from 0"), NULL, 0, NULL);
		mario.isFly = true;        // 设为空中
		mario.vY = -sqrt(2 * G * REAL_HEIGHT); // 跳跃初速度
	}

	//空中运动
	if (mario.isFly == true)
	{
		// 计算垂直位移
		mario.yy = mario.yy - (-rule::move(mario.vY, TIME, G) * UNREAL_HEIGHT / REAL_HEIGHT);
		mario.y = (int)mario.yy;

		// 检测是否碰到地面
		map = touchMap(mario.x, mario.y + 1, myScene);
		if (map != NULL)
		{
			if (mario.vY > 0) // 下落碰到地面
				mario.isFly = false;
			mario.vY = 0; // 速度清零
			// 校准坐标
			mario.y = (mario.y + HEIGHT / 2) / HEIGHT * HEIGHT;
			mario.yy = mario.y;
			mario.direction.y += 1;
		}

		// 下落时检测是否踩敌人
		if (mario.vY > 0)
		{
			Enemy* emy = touchEnemy(mario.x, mario.y, myEnemy);
			if (emy != NULL)
			{
				mciSendString(_T("play music_tread from 0"), NULL, 0, NULL);
				score += 5;
				setBomb(emy->x, emy->y); // 爆炸
				// 销毁敌人
				emy->x = 0;
				emy->y = 0;
				emy->turn = 0;
			}

			// 掉下屏幕 → 死亡
			if (mario.y > YSIZE)
			{
				mario.died = true;
				mciSendString(_T("play music_died from 0"), NULL, 0, NULL);
			}
		}
	}
	else
	{
		// 如果没碰到地面 → 视为空中
		map = touchMap(mario.x, mario.y + 1, myScene);
		if (map == NULL)
		{
			mario.isFly = true;
		}
	}

	// 左右移动
	if (KEY & CMD_LEFT && mario.ending == false)
	{
		a -= A_ROLE;          // 左加速度
		mario.direction.x -= 1;
	}
	if (KEY & CMD_RIGHT || mario.ending == true)
	{
		a += A_ROLE;          // 右加速度
		mario.direction.x += 1;
	}

	//摩擦力
	if (mario.vX * a <= 0 && mario.vX != 0)
	{
		int k = -(int)(mario.vX / fabs(mario.vX));
		if (map == NULL)
			a1 = 0;
		else
			a1 = k * G * map->u;
	}

	// 计算水平移动
	double tmp = mario.vX;
	double H = rule::move(mario.vX, TIME, a + a1) * UNREAL_HEIGHT / REAL_HEIGHT;
	if (tmp * mario.vX < 0)
		mario.vX = 0;

	mario.xx += H;
	mario.x = (int)mario.xx;

	// 判断是否到达终点
	if (myScene->isEnding(-(int)mario.x0 + mario.x))
	{
		mario.ending = true;
	}
	// 走出屏幕右边通关
	if (mario.x > XSIZE)
		mario.passed = true;

	// 碰到墙壁停止
	if (touchMap(mario.x, mario.y, myScene) != NULL)
	{
		if (mario.x > XRIGHT)
			mario.x = XRIGHT;
		else
			mario.x = (int)(mario.xx - H);
		mario.xx = mario.x;
		mario.vX = 0;
		a = 0;
		a1 = 0;
	}

	// 最大速度限制
	if (fabs(mario.vX) > V_MAX)
		mario.vX = mario.vX / fabs(mario.vX) * V_MAX;

	// 射击逻辑
	if (KEY & CMD_SHOOT && mario.isShoot == true)
	{
		int x = mario.x + WIDTH / 2;
		if (mario.turn == -1)
			x = mario.x - WIDTH;

		// 射击冷却
		if (shootButtonDown == false)
		{
			mciSendString(_T("play music_bullet from 0"), NULL, 0, NULL);
			setBullet(x, mario.y);
		}
		else
		{
			if (shootTimeInterval == 0)
			{
				mciSendString(_T("play music_bullet from 0"), NULL, 0, NULL);
				setBullet(x, mario.y);
			}
			shootTimeInterval += TIME;
			if (shootTimeInterval > TIME_INTERVAL_BULLET)
				shootTimeInterval = 0;
		}
		shootButtonDown = true;
	}
	else
	{
		shootButtonDown = false;
		shootTimeInterval = 0.01;
	}


	if (mario.x < XLEFT)
	{
		mario.x = XLEFT;
		mario.xx = mario.x;
		mario.vX = 0;
		a = 0;
	}
	else if (mario.x > XRIGHT && mario.ending == false)
	{
		// 地图跟随移动
		mario.x0 -= (mario.x - XRIGHT);
		mario.x = XRIGHT;
		mario.xx = mario.x;
	}

	// 敌人移动逻辑
	int i = 0;
	if ((int)(enemy_iframe * 100) % 2 == 0)
	{
		while (i < ENEMY_TOTE)
		{
			if (myEnemy[i].turn != 0)
			{
				myEnemy[i].x += myEnemy[i].turn * ENEMY_STEP;

				int x1 = (int)mario.x0 + myEnemy[i].x;
				int y1 = myEnemy[i].y;
				int x2 = (int)mario.x0 + myEnemy[i].x + myEnemy[i].turn * WIDTH;
				int y2 = myEnemy[i].y + 1;

				// 碰到墙壁或悬空就掉头
				if (touchMap(x1, y1, myScene) != NULL || touchMap(x2, y2, myScene) == NULL)
					myEnemy[i].turn *= -1;
			}
			i++;
		}
	}

	//吃到金币
	POINT* p = touchCoins(mario.x, mario.y, myScene);
	if (p != NULL)
	{
		mciSendString(_T("play music_coin from 0"), NULL, 0, NULL);
		score += 10;
		myScene->setScorePos(p->x, p->y);
		// 销毁金币
		p->x = 0;
		p->y = 0;
	}
	//吃到道具,解锁射击
	POINT* q = touchFood(mario.x, mario.y, myScene);
	if (q != NULL)
	{
		mciSendString(_T("play music_getWeapon from 0"), NULL, 0, NULL);
		myScene->setScorePos(q->x, q->y);
		q->x = 0;
		q->y = 0;
		mario.isShoot = true; // 允许射击
	}

	//碰到敌人死亡
	Enemy* emy = touchEnemy(mario.x, mario.y, myEnemy);
	if (emy != NULL && mario.vY <= 0)
	{
		mario.died = true;
		mciSendString(_T("play music_died from 0"), NULL, 0, NULL);
	}
}

