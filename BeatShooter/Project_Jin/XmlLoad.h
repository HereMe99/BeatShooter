#pragma once

class XmlLoad
{
public:
	static XmlLoad* instance;
	inline static XmlLoad* GetInstance();

	void ActionsLoad(vector<Animator*>& actions,string filepath);
	void EffectLoad(string key, string filepath, int poolNum);
	void Release();
private:
	XmlLoad();
	~XmlLoad();
};

inline XmlLoad* XmlLoad::GetInstance()
{
	if (instance == nullptr)
	{
		instance = NEW XmlLoad;
	}
	return instance;
}