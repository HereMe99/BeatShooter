#pragma once
class Device
{
private:
	LPDIRECT3D9 d3d;
	LPDIRECT3DDEVICE9 device;

	static Device* instance;

	Device();
	~Device();
public:
	static Device* GetInstance()
	{
		if (instance == nullptr)
			instance = NEW Device;

		return instance;
	}

	LPDIRECT3DDEVICE9 GetDevice()
	{
		return device;
	}

	void BeginRender();
	void EndRender();

	void Release();
};

