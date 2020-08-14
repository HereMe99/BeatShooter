#include "stdafx.h"
#include "Timer.h"



Timer::Timer(float limitTime, bool isLoop)
{
	this->limitTime = limitTime;
	curTime = 0.0f;

	this->isLoop = isLoop;
	isActive = false;
	timerFunc = nullptr;
}

Timer::~Timer()
{
}

void Timer::Update()
{
	
	if (isActive)
	{
		if (IsLimit())
		{
			if (!isLoop)
			{
				StopTimer();
			}
		}
		return;
	}
	else
	{
		return;
	}
}

void Timer::StartTimer()
{
	isActive = true;
	curTime = 0.0f;
}

void Timer::StopTimer()
{
	isActive = false;
	curTime = 0.0f;
}

bool Timer::IsLimit()
{
	if (curTime < limitTime)
	{
		curTime += DELTATIME * 1.0f;
		return false;
	}
	else
	{
		curTime = 0.0f;

		if (timerFunc != nullptr)
			timerFunc();

		if (!isLoop)
			StopTimer();

		return true;
	}
}

