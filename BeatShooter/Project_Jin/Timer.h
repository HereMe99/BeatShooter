#pragma once
struct Timer
{
	enum TimerState
	{
		NONEACTIVE,
		TIMERFALSE,
		TIMERTRUE
	};
	float limitTime;
	float curTime;

	bool isActive;
	bool isLoop;

	Timer(float limitTime,bool isLoop);
	~Timer();

	function<void()> timerFunc;

	void Update();

	void StartTimer();
	void StopTimer();
	
	bool IsLimit();
};

