#include "stdafx.h"
#include "EffectManager.h"

EffectManager* EffectManager::instance = nullptr;

EffectManager::EffectManager()
{
	D3DXCreateEffectFromFile(DEVICE,
		L"CharacterShader.hlsl",
		NULL, NULL, D3DXSHADER_DEBUG, NULL, &shader, NULL);
	color = { 1.0f, 1.0f, 1.0f, 0.5f };
	value = 0;
	pass = 0;
}


EffectManager::~EffectManager()
{
}

void EffectManager::AddEffect(string key, Animator* ani, float time, int poolCount)
{
	vEffect vecEffect;
	for (int i = 0; i < poolCount; i++)
	{
		vecEffect.push_back(NEW Effect(ani, time));
	}
	EffectMeta eMeta;
	eMeta.animator = ani;
	eMeta.effects = vecEffect;
	totalEffect.insert({ key, eMeta });
}

void EffectManager::Update()
{
	for (auto mapIter : totalEffect)
	{
		for (auto effect : mapIter.second.effects)
		{
			effect->Update();
		}
	}
}

void EffectManager::Render(string key)
{
	

	auto mapIter = totalEffect.find(key);

	if (mapIter == totalEffect.end())
	{
		return;
	}

	shader->SetFloat("value", value);
	shader->SetFloatArray("color", (float*)&color, 4);
	shader->Begin(NULL, NULL);
	shader->BeginPass(1);
	for (auto effect : mapIter->second.effects)
	{
		mapIter->second.animator->GetTexture()->GetUvStart(&uvStart);
		mapIter->second.animator->GetTexture()->GetUvEnd(&uvEnd);
		shader->SetFloatArray("cutStart", (float*)&uvStart, 2);
		shader->SetFloatArray("cutEnd", (float*)&uvEnd, 2);
		shader->SetTexture("map", mapIter->second.animator->GetDxTexture());
		effect->Render();
	}
	shader->EndPass();
	shader->End();
	
}

void EffectManager::Release()
{
	for (auto mapIter : totalEffect)
	{
		SAFE_DELETE(mapIter.second.animator);
		for (Effect* effect : mapIter.second.effects)
		{
			SAFE_DELETE(effect);
		}
		mapIter.second.effects.clear();
	}
	totalEffect.clear();

	SAFE_DELETE(instance);
}

void EffectManager::Play(string key, World world, World* parentsWorld)
{
	auto mapIter = totalEffect.find(key);

	if (mapIter == totalEffect.end())
	{
		return;
	}

	for (auto effect : mapIter->second.effects)
	{
		if (!effect->GetIsRun())
		{
			if (parentsWorld == nullptr)
			{
			effect->Start(world);
			}
			else
			{
				effect->Start(world, parentsWorld);
			}
			return;
		}
	}
}

