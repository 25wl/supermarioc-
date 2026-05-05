#pragma once
#include<windows.h>

class MyTimer//计时器，用于进行Sleep延时
{
private:
	LARGE_INTEGER m_clk;
	LONGLONG m_oldclk;//上一次计时结束时的时钟数值
	int m_freq;//时钟频率
public:
	MyTimer();
	void Sleep(int ms);//相较于普通的Sleep函数计时更加精确
	void Reset();//重置
};
MyTimer::MyTimer()
{
	QueryPerformanceFrequency(&m_clk);//获取系统高精度计数器频率并保存
	m_freq = (int)m_clk.QuadPart / 1000;
	m_oldclk = 0;
}
void MyTimer::Sleep(int ms)
{
	unsigned int c = ms * m_freq;//计算需要延迟的时钟周期数
	if (m_oldclk == 0)
	{
		QueryPerformanceCounter(&m_clk);//记录当前时钟值
		m_oldclk = m_clk.QuadPart;
	}
	m_oldclk += c;
	QueryPerformanceCounter(&m_clk);
	if (m_clk.QuadPart > m_oldclk)
		m_oldclk = m_clk.QuadPart;

	else
		do
		{
			::Sleep(1);

			QueryPerformanceCounter(&m_clk);
		}

	while (m_clk.QuadPart < m_oldclk);
}
//重置计时器
void MyTimer::Reset()
{
	m_oldclk = 0;
}