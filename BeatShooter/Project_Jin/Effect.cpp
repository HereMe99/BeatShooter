#include "stdafx.h"
#include "Effect.h"


Effect::Effect(Animator* ani, float time)
{
	aniNum = 0;
	this->ani = ani;
	isRun = false;
}

Effect::~Effect()
{
}

void Effect::Update()
{
	if (isRun)
	{
		world.Update();
		ani->Update();
		if (!ani->GetIsPlay())
		{
			isRun = false;
		}
		
	}
}

void Effect::Render()
{
	if (isRun)
	{
		ani->Render();
	}
}

void Effect::Start(World world)
{
	//ani->SetParentsWorld(&world);
	this->world = world;
	isRun = true;
	ani->Play();
}
void Effect::Start(World world, World *parentsWorld)
{
	ani->SetParentsWorld(parentsWorld);
	this->world = world;
	isRun = true;
	ani->Play();
}


