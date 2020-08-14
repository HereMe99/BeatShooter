#pragma once

#define MAXKEY 255

class InputManager
{
	enum KeyState
	{
		KEY_NONE,
		KEY_DOWN,
		KEY_UP,
		KEY_PRESS
	};

private:
	byte keyState[MAXKEY];
	byte keyOldState[MAXKEY];
	byte keyMap[MAXKEY];

	static InputManager* instance;
	InputManager();
	~InputManager();
public:
	static InputManager* GetInstance()
	{
		if (instance == nullptr)
			instance = NEW InputManager;

		return instance;
	}

	void Update();

	bool KeyDown(DWORD key)
	{
		return keyMap[key] == KEY_DOWN;
	}

	bool KeyUp(DWORD key)
	{
		return keyMap[key] == KEY_UP;
	}

	bool KeyPress(DWORD key)
	{
		return keyMap[key] == KEY_PRESS;
	}

	void Release();
};

