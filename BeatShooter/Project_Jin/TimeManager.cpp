#include "stdafx.h"
#include "TimeManager.h"


TimeManager::TimeManager()
{
	font = NULL;
	QueryPerformanceFrequency((LARGE_INTEGER*)&periodFrequency);
	QueryPerformanceCounter((LARGE_INTEGER*)&lastTime);
	timeScale = 1.0f / periodFrequency;

	frameRate = 0;
	frameCount = 0;
	timeElapsed = 0;
	oneSecCount = 0;
	worldTime = 0;
	curTime = 0;
}


TimeManager::~TimeManager()
{
}

void TimeManager::Update(float lockFPS)
{
	QueryPerformanceCounter((LARGE_INTEGER*)&curTime);

	//timeElapsed = (curTime - lastTime) / periodFrequency;
	timeElapsed = (curTime - lastTime) * timeScale;

	if (lockFPS > 0)
	{
		while (timeElapsed < (1.0f / lockFPS))
		{
			QueryPerformanceCounter((LARGE_INTEGER*)&curTime);
			timeElapsed = (curTime - lastTime) * timeScale;
		}
	}

	lastTime = curTime;
	frameCount++;
	oneSecCount += timeElapsed;
	worldTime += timeElapsed;

	if (oneSecCount > 1.0f)
	{
		frameRate = frameCount;
		frameCount = 0;
		oneSecCount = 0;
	}
}

void TimeManager::Render()
{
#ifdef _DEBUG
	if (font == NULL)
	{
		wstring str;
		D3DXFONT_DESC desc = {};
		desc.Width = 20;
		desc.Height = 20;
		desc.CharSet = HANGUL_CHARSET;
		str = L"Consolas";
		wcscpy_s(desc.FaceName, str.c_str());

		D3DXCreateFontIndirect(DEVICE, &desc, &font);
	}
	wstring str;
	str = to_wstring((int)frameRate);

	RECT rect = { WIN_WIDTH - 100, 5, WIN_WIDTH, 25 };

	font->DrawText(NULL, str.c_str(), -1,
		&rect, DT_LEFT, 0xffffff00);
#endif
	
}
