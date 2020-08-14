#include "stdafx.h"
#include "InputManager.h"

InputManager* InputManager::instance = nullptr;

InputManager::InputManager()
{
	ZeroMemory(keyState, sizeof(keyState));
	ZeroMemory(keyOldState, sizeof(keyOldState));
	ZeroMemory(keyMap, sizeof(keyMap));
}


InputManager::~InputManager()
{
}

void InputManager::Update()
{
	memcpy(keyOldState, keyState, sizeof(keyOldState));

	ZeroMemory(keyState, sizeof(keyState));
	ZeroMemory(keyMap, sizeof(keyMap));

	GetKeyboardState(keyState);

	for (int i = 0; i < MAXKEY; i++)
	{
		byte key = keyState[i] & 0x80;
		keyState[i] = key ? 1 : 0;

		byte oldState = keyOldState[i];
		byte state = keyState[i];

		if (oldState == 0 && state == 1)
		{
			keyMap[i] = KEY_DOWN;
		}
		else if (oldState == 1 && state == 0)
		{
			keyMap[i] = KEY_UP;
		}
		else if (oldState == 1 && state == 1)
		{
			keyMap[i] = KEY_PRESS;
		}
		else
		{
			keyMap[i] = KEY_NONE;
		}
	}
}

void InputManager::Release()
{
	delete instance;
}
