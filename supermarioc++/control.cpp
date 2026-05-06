#define _CRT_SECURE_NO_WARNINGS
#include"control.h"
#include"graphics.h"
#include"conio.h"
#include"mydefine.h"
#include<cstring>
#include <tchar.h> 
control::control(void)
{
	controlkey = 0;
}
control::~control(void)
{

}
//每一帧都强制检测按键，以防卡顿
int control::getKey()
{
	controlkey = GetCommand();//实时读键，长按也有效
	if (controlkey & CMD_ESC)//当按键为ESC时，人物停止移动
		pauseClick();
	return controlkey;
}
int control::GetCommand()
{
	int c = 0;//高位为1，表示此时正在按的键盘信号
	if (GetAsyncKeyState('A')& 0x8000)
		c |= CMD_LEFT;//当按键A时，人物左移
	if (GetAsyncKeyState('D') & 0x8000)
		c |= CMD_RIGHT;//当按键D时，人物右移
	if (GetAsyncKeyState('W') & 0x8000)
		c |= CMD_UP;//当按键W时，人物上跳
	if (GetAsyncKeyState('S') & 0x8000)
		c |= CMD_DOWN;//当按键S时，人物蹲下
	if (GetAsyncKeyState('J') & 0x8000)
		c |= CMD_SHOOT;//当按键J时，人物发射火球
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
		c |= CMD_ESC;
	return c;//返回键盘信号
}
void control::pauseClick()//暂停菜单绘制函数
{
	BeginBatchDraw();//开始批量绘制，画完再一次性刷新
	POINT points[8] = { {XSIZE / 2 - 45,YSIZE / 3},{XSIZE / 2 + 45,YSIZE / 3},{XSIZE / 2 + 45,YSIZE / 3 + 90},{XSIZE / 2 -45,YSIZE / 3 + 90} };
	fillpolygon(points, 4);//根据上方提供的四个点绘制一个相应的矩形作为“回到游戏”的按钮背景框
	setbkmode(TRANSPARENT);//设置文字背景为透明
	setfont(20, 0,_T("黑体"));//设置字体；高20，宽度自适应，黑体

		//绘制“回到游戏”按钮区域
		RECT r2 = { XSIZE / 2 - 45,YSIZE / 3,XSIZE / 2 + 45,YSIZE / 3 + 30 };
		rectangle(XSIZE / 2 - 45, YSIZE / 3, XSIZE / 2 + 45, YSIZE / 3 + 30);//绘制按钮的边框
		//居中绘制文字
		drawtext(_T("回到游戏"), &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		//绘制“重新开始”按钮
		RECT r3 = { XSIZE / 2 - 45,YSIZE / 3 + 30,XSIZE / 2 + 45,YSIZE / 3 + 60 };
		rectangle(XSIZE / 2 - 45, YSIZE / 3 + 30, XSIZE / 2 + 45, YSIZE / 3 + 60);
		drawtext(_T("重新开始"), &r3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		//绘制主菜单按钮
		RECT r4 = { XSIZE / 2 - 45,YSIZE / 3 + 60,XSIZE / 2 + 45,YSIZE / 3 + 90 };
		rectangle(XSIZE / 2 - 45, YSIZE / 3 + 60, XSIZE / 2 + 45, YSIZE / 3 + 90);
		drawtext(_T(" 主 菜 单 "), &r4, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		FlushBatchDraw();//结束批量绘图，刷新屏幕
		FlushMouseMsgBuffer();//清空鼠标消息队列，以防旧消息干扰

		//创造死循环等待玩家鼠标操作
		while (true)
		{
			BeginBatchDraw();//批量绘图
			MOUSEMSG m = GetMouseMsg();//获取一条鼠标消息
			//判断鼠标消息类型
			switch (m.uMsg)
			{
				//如果鼠标左键按下
			case WM_LBUTTONDOWN:
				EndBatchDraw();//结束批量绘图

				//判断点击的位置
				//回到游戏按钮
				if (m.x > XSIZE / 2 - 45 && m.x<XSIZE / 2 + 45 && m.y>YSIZE / 3 && m.y < YSIZE / 3 + 30)
				{
					controlkey = VIR_RETURN;
					return;
				}
				//重新开始按钮
				else if (m.x > XSIZE / 2 - 45 && m.x<XSIZE / 2 + 45 && m.y>YSIZE / 3+30 && m.y < YSIZE / 3 + 60)
				{
					controlkey = VIR_RESTART;
					return;
				}
				//主菜单按钮
				else if (m.x > XSIZE / 2 - 45 && m.x<XSIZE / 2 + 45 && m.y>YSIZE / 3+60 && m.y < YSIZE / 3 + 90)
				{
					controlkey = VIR_HOME;
					return;
				}
				break;

				//鼠标移动按钮悬浮高亮
			case WM_MOUSEMOVE:
				RECT r;
				int i;
				for (i = 0; i < 3; i++)
				{
					if(m.x>XSIZE/2-45&&m.x<XSIZE/2+45&&m.y>YSIZE/3+i*30&&m.y<YSIZE/3+30+i*30)
					{
						// 设置按钮区域
						r.left = XSIZE / 2 - 45;
						r.top = YSIZE / 3 + i * 30;
						r.right = XSIZE / 2 + 45;
						r.bottom = YSIZE / 3 + 30 + i * 30;
						// 定义矩形顶点
						POINT points[8] = { {r.left,r.top}, {r.right,r.top}, {r.right,r.bottom}, {r.left,r.bottom} };
						setfillcolor(RED);        // 悬浮填充红色
						fillpolygon(points, 4);   // 重绘按钮
						setbkmode(TRANSPARENT);   // 文字透明

						// 根据按钮索引绘制对应文字
						switch (i)
						{
						case 0:
							drawtext(_T("回到游戏"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						case 1:
							drawtext(_T("重新开始"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						case 2:
							drawtext(_T(" 主 菜 单 "), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						}
					}
					FlushBatchDraw();  // 刷新画面
				}
			}
			break;
		}
}
void control::DisableConsoleInput()
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode;
	GetConsoleMode(hInput, &mode);
	mode &= ~ENABLE_ECHO_INPUT;      // 关闭按键回显
	mode &= ~ENABLE_LINE_INPUT;      // 关闭行输入
	SetConsoleMode(hInput, mode);
}
//游戏主菜单页面创建
void control::gameStart()
{
	cleardevice();//清空屏幕
	IMAGE img;//定义图片对象
	//加载首页背景图片
	loadimage(&img, _T("res\\home.bmp"), XSIZE, 5 * YSIZE);
	putimage(0, 0, &img);//将图片绘制到屏幕（0，0）处
	setbkmode(TRANSPARENT);//文字背景透明

	setfont(40, 0, _T("方正舒体"));
	//绘制游戏标题
	RECT r1 = { 0, 0, XSIZE, YSIZE / 3 };
	drawtext(_T("超级玛丽"),& r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	setfont(20, 0, _T("宋体"));//恢复小字体

	// 绘制”开始游戏“按钮
	RECT r2 = { XSIZE / 2 - 45,YSIZE / 3,XSIZE / 2 + 45,YSIZE / 3 + 30 };
	rectangle(XSIZE / 2 - 45, YSIZE / 3, XSIZE / 2 + 45, YSIZE / 3 + 30);
	drawtext(_T("开始游戏"), &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	// 绘制”游戏介绍“按钮
	RECT r3 = { XSIZE / 2 - 45,YSIZE / 3+30,XSIZE / 2 + 45,YSIZE / 3 + 60 };
	rectangle(XSIZE / 2 - 45, YSIZE / 3+30, XSIZE / 2 + 45, YSIZE / 3 + 60);
	drawtext(_T("游戏介绍"), &r3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	// 绘制“操作说明”按钮
	RECT r4 = { XSIZE / 2 - 45,YSIZE / 3 + 60,XSIZE / 2 + 45,YSIZE / 3 + 90 };
	rectangle(XSIZE / 2 - 45, YSIZE / 3 + 60, XSIZE / 2 + 45, YSIZE / 3 + 90);
	drawtext(_T("操作说明"), &r4, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	// 绘制“退出游戏”按钮
	RECT r5 = { XSIZE / 2 - 45,YSIZE / 3 + 90,XSIZE / 2 + 45,YSIZE / 3 + 120 };
	rectangle(XSIZE / 2 - 45, YSIZE / 3 + 90, XSIZE / 2 + 45, YSIZE / 3 + 120);
	drawtext(_T("退出游戏"), &r5, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	
	//对页面的状态进行标记，true表示当前处于该界面
	bool _HOME = true, _INTRODUCTION = false, _OPERATION = false;
	MOUSEMSG m;//鼠标消息对象

	//主菜单循环
	while (_HOME)
	{
		BeginBatchDraw();
		m = GetMouseMsg();//获取鼠标消息

		switch (m.uMsg)
		{
			//如果鼠标左键按下
		case WM_LBUTTONDOWN:
			EndBatchDraw();
			//点击开始游戏
			if (m.x > XSIZE / 2 - 45 && m.x<XSIZE / 2 + 45 && m.y>YSIZE / 3 && m.y < YSIZE / 3 + 30 && _HOME == 1 && _INTRODUCTION == 0 && _OPERATION == 0)
			{
				_HOME = false;//退出主菜单界面，进入游戏
				break;
			}
			// 点击游戏介绍
			else if (m.x > XSIZE / 2 - 45 && m.x<XSIZE / 2 + 45 && m.y>YSIZE / 3 + 30 && m.y < YSIZE / 3 + 60 && _HOME == 1 && _OPERATION == 0)
			{
				_INTRODUCTION = true;  // 进入介绍页面
				cleardevice();        // 清屏
				rectangle(50, 50, 213, 220);  // 画文本框
				outtextxy(52, 52, _T("游戏介绍："));
				outtextxy(52, 82, _T("超级玛丽"));
				outtextxy(52, 102, _T("击败蘑菇大怪闯关"));
				// 绘制右下角返回按钮
				RECT R1 = { XSIZE - 46,YSIZE - 26,XSIZE - 2,YSIZE - 2 };
				rectangle(XSIZE - 46, YSIZE - 26, XSIZE - 2, YSIZE - 2);
				drawtext(_T("返回"), &R1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				break;
			}
			// 点击操作说明
			else if (m.x > XSIZE / 2 - 45 && m.x<XSIZE / 2 + 45 && m.y>YSIZE / 3 + 60 && m.y < YSIZE / 3 + 90 && _HOME == 1 && _INTRODUCTION == 0)
			{
				_OPERATION = true;     // 进入说明页面
				cleardevice();
				rectangle(50, 50, 213, 220);
				outtextxy(52, 52, _T("操作说明："));
				outtextxy(52, 72, _T("左移：A键"));
				outtextxy(52, 92, _T("右移：D键"));
				outtextxy(52, 112, _T("发射：J键"));
				outtextxy(52, 132, _T("跳跃：W键/K键"));
				outtextxy(52, 152, _T("暂停：Esc键"));
				// 返回按钮
				RECT R2 = { XSIZE - 46,YSIZE - 26,XSIZE - 2,YSIZE - 2 };
				rectangle(XSIZE - 46, YSIZE - 26, XSIZE - 2, YSIZE - 2);
				drawtext(_T("返回"), &R2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				break;
			}
			// 点击退出游戏
			else if (m.x > XSIZE / 2 - 45 && m.x<XSIZE / 2 + 45 && m.y>YSIZE / 3 + 90 && m.y < YSIZE / 3 + 120 && _HOME == 1 && _INTRODUCTION == 0 && _OPERATION == 0)
				exit(0);  // 直接关闭程序
			// 点击介绍/说明页面的返回按钮
			else if (m.x > XSIZE - 46 && m.x<XSIZE - 3 && m.y>YSIZE - 26 && m.y < YSIZE - 3 && (_INTRODUCTION == 1 || _OPERATION == 1))
			{
				cleardevice();
				// 重置页面状态
				_HOME = false, _INTRODUCTION = false, _OPERATION = false;
				gameStart();  // 重新调用主菜单
			}
			else
				break;

			//鼠标移动按钮悬浮高光
		case WM_MOUSEMOVE:
			RECT r;
			if (_INTRODUCTION == 1 || _OPERATION == 1)
			{
				//鼠标在返回按钮上方
				if (m.x > XSIZE - 46 && m.x<XSIZE - 3 && m.y>YSIZE - 26 && m.y < YSIZE - 3)
				{
					//按钮变红
					r.left = XSIZE - 46;
					r.top = YSIZE - 26;
					r.right = XSIZE - 2;
					r.bottom = YSIZE - 2;
					POINT points[8] = { {r.left,r.top},{r.right,r.top},{r.right,r.bottom},{r.left,r.bottom} };
					setfillcolor(RED);//红色填充上方点构成的矩形
					fillpolygon(points, 4);
					setbkmode(TRANSPARENT);
					drawtext(_T("返回"),&r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				}
				// 鼠标离开按钮恢复黑色
				else
				{
					if (getpixel(XSIZE - 46 + 1, YSIZE - 26 + 1) == RED)
					{
						r.left = XSIZE - 46;
						r.top = YSIZE - 26;
						r.right = XSIZE - 2;
						r.bottom = YSIZE - 2;
						POINT points[8] = { {r.left,r.top}, {r.right,r.top}, {r.right,r.bottom}, {r.left,r.bottom} };
						setfillcolor(BLACK);
						fillpolygon(points, 4);
						setbkmode(TRANSPARENT);
						drawtext(_T("返回"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					}
				}
			}
			// 主菜单页面,4个按钮悬浮
			else
			{
				for (int i = 0; i < 4; i++)
				{
					// 鼠标在按钮上
					if (m.x > XSIZE / 2 - 45 && m.x<XSIZE / 2 + 45 && m.y>YSIZE / 3 + i * 30 && m.y < YSIZE / 3 + 30 + i * 30)
					{
						r.left = XSIZE / 2 - 45;
						r.top = YSIZE / 3 + i * 30;
						r.right = XSIZE / 2 + 45;
						r.bottom = YSIZE / 3 + 30 + i * 30;
						POINT points[8] = { {r.left,r.top}, {r.right,r.top}, {r.right,r.bottom}, {r.left,r.bottom} };
						setfillcolor(RED);  // 变红
						fillpolygon(points, 4);
						setbkmode(TRANSPARENT);
						// 绘制对应文字
						switch (i)
						{
						case 0:
							drawtext(_T("开始游戏"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						case 1:
							drawtext(_T("游戏介绍"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						case 2:
							drawtext(_T("操作说明"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						case 3:
							drawtext(_T("退出游戏"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						}
					}
					// 鼠标离开，恢复黑色
					else
					{
						if (getpixel(XSIZE / 2 - 45 + 1, YSIZE / 3 + i * 30 + 1) == RED)
						{
							r.left = XSIZE / 2 - 45;
							r.top = YSIZE / 3 + i * 30;
							r.right = XSIZE / 2 + 45;
							r.bottom = YSIZE / 3 + 30 + i * 30;
							POINT points[4] = { {r.left,r.top}, {r.right,r.top}, {r.right,r.bottom}, {r.left,r.bottom} };
							setfillcolor(BLACK);
							fillpolygon(points, 4);
							setbkmode(TRANSPARENT);
							// 重绘文字
							switch (i)
							{
							case 0:
								drawtext(_T("开始游戏"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
								break;
							case 1:
								drawtext(_T("游戏介绍"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
								break;
							case 2:
								drawtext(_T("操作说明"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
								break;
							case 3:
								drawtext(_T("退出游戏"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
								break;
							}
						}
					}
				}
			}
			FlushBatchDraw();  // 刷新画面
			break;
			// 其他消息不处理
			default:
				break;
		 }
	}
}
//在左上角展示当前分数
void control::showScore(int score)
{
	setfont(0, 0, _T("宋体"));
	TCHAR s1[20] = _T("Score");//分数前缀
	TCHAR s2[5];//存储数字转化为字符的结果
	_itot(score, s2, 10);//将数字score转化为十进制字符串
	_tcscat(s1, s2); //将前缀和分数进行拼接
	setbkmode(TRANSPARENT);//文字背景透明
	outtextxy(10, 10, s1);//在（10，10）输出分数
	setbkmode(OPAQUE);//恢复文字背景不透明

}
// 展示关卡：在右上角显示当前关卡
void control::showLevel(int level)
{
	setfont(0, 0, _T("宋体"));
	TCHAR s1[20] =_T("LEVEL:");
	TCHAR s2[2];
	_itot(level, s2, 10);
	_tcscat(s1, s2);
	setbkmode(TRANSPARENT);
	outtextxy(XSIZE - 90, 10, s1);    // 右上角显示
	setbkmode(OPAQUE);
}
//显示mario剩余生命值
void control::showDied(int life)
{
	cleardevice();
	TCHAR s1[20] = _T("生命还剩下：");
	TCHAR s2[2];
	_itot(life, s2, 10);
	outtextxy(XSIZE / 2 - 43, YSIZE / 3, s1);//居中显示文字
	outtextxy(XSIZE / 2, YSIZE / 2 - 20, s2);//居中显示剩余生命数量
	Sleep(2000);//显示2s
}
//游戏结束界面
void control::showGameOver()
{
	IMAGE img;
	loadimage(&img, _T("res\\home.bmp"), XSIZE, 5 * YSIZE);
	putimage(0, -YSIZE, &img);
	Sleep(6500);
}
//单个关卡通关展示动画
void control::showPassed(int world)
{
	cleardevice();
	TCHAR s1[20] = _T("LEVEL:");
	TCHAR s2[2];
	_itot(world, s2, 10);
	outtextxy(XSIZE / 2 - 20, YSIZE / 3, s1);//显示关卡文字
	outtextxy(XSIZE / 2, YSIZE / 2 - 20, s2);//显示关卡数
	Sleep(2000);
}
// 全部通关展示
void control::showPassedAll()
{
	IMAGE img;
	loadimage(&img, _T("res\\home.bmp"), XSIZE, 5 * YSIZE);
	putimage(0, -3 * YSIZE, &img);    // 绘制最终背景
	Sleep(7800);                  
}

