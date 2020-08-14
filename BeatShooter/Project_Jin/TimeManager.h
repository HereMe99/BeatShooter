#pragma once
class TimeManager
{
private:
	float timeScale;
	float timeElapsed;

	__int64 curTime;
	__int64 lastTime;
	__int64 periodFrequency;

	UINT frameRate;
	UINT frameCount;

	float oneSecCount;
	float worldTime;

	LPD3DXFONT font;

	TimeManager();
	~TimeManager();
public:
	static TimeManager* GetInstance()
	{
		static TimeManager instance;
		return &instance;
	}

	void Update(float lockFPS = 0.0f);
	void Render();

	UINT GetFPS() { return frameRate; }
	float GetElapsedTime() { return timeElapsed; }
	float GetWorldTime() { return worldTime; }
};
