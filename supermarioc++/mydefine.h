#pragma once

#ifndef MYDEFINE
#define MYDIFINE

#define XSIZE 512
#define YSIZE 384
#define WIDTH 32
#define HEIGHT 32
#define X0 2*WIDTH//主角的坐标
#define Y0 3*HEIGHT
#define TIME 0.01
#define STEP 10
#define ENEMY_STEP 1
#define K_MAP_BG 5//地图每移动5个像素点背景就移动1个像素点。及map/bg=5
#define XLEFT 0
#define XRIGHT WIDTH*6
#define REAL_HEIGHT 3.5
#define UNREAL_HEIGHT (3*HEIGHT+5)
#define G 30.0
#define V_MAX 8.0
#define A_ROLE 20.0
#define T1 0.5//主角加速到最大速度所用的时间
#define T2 1.2
#define T3 1.5
#define F TIME*0.3;//控制景物刷新的频率
#define LIFE 5;

//按键的宏定义
#define	CMD_LEFT 1
#define	CMD_RIGHT 2	
#define	CMD_UP 4
#define CMD_DOWN 8
#define CMD_SHOOT 16
#define CMD_ESC 32
//虚拟按键的宏定义
#define VIR_RETURN 64
#define VIR_RESTART 128
#define VIR_HOME 256

#endif
