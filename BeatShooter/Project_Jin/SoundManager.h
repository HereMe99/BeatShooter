#pragma once

#define SOUNDCOUNT 50

using namespace FMOD;

struct SoundInfo
{
	Sound* sound;
	Channel* channel;

	SoundInfo() : sound(nullptr), channel(nullptr)
	{
	}

	~SoundInfo()
	{
		sound->release();
	}
};

class SoundManager
{
private:
	SoundManager();
	~SoundManager();
public:
	static SoundManager* Get();
	static void Create();
	static void Release();

	void Update();

	void Add(string key, string fileName, bool bgm);

	void Play(string key, float volume = 1.0f);
	void Stop(string key);
	void Pause(string key);
	void Resume(string key);

	void SetSoundFrequency(string key, float frequency);
	bool isPlaying(string key);
	bool isPaused(string key);

	void DspHighpassSetting(string key);
	void DspSetHighpass(float amount = 5000.0f);
private:
	static SoundManager* instance;

	System* soundSystem;

	map<string, SoundInfo*> totalSound;

	DSP* dsp = NULL;
};