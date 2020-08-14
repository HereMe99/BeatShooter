#include "stdafx.h"
#include "XmlLoad.h"

XmlLoad* XmlLoad::instance = nullptr;

XmlLoad::XmlLoad()
{
}

XmlLoad::~XmlLoad()
{
}

void XmlLoad::ActionsLoad(vector<Animator*>& actions, string filepath)
{
	Animator* animator = nullptr;
	string xmlFilepath = filepath + ".xml";
	string pngFilepath = filepath + ".png";
	XmlDocument* document = NEW XmlDocument;
	document->LoadFile(xmlFilepath.c_str());

	XmlElement* atlas = document->FirstChildElement();
	wstring wPath;
	wPath.assign(pngFilepath.begin(), pngFilepath.end());

	XmlElement* action = atlas->FirstChildElement();
	for (; action != NULL; action = action->NextSiblingElement())
	{
		DWORD time = action->UnsignedAttribute("time");
		Type type = (Type)action->UnsignedAttribute("type");

		vector<AnimationClip> clips;
		XmlElement* sprite = action->FirstChildElement();
		for (; sprite != NULL; sprite = sprite->NextSiblingElement())
		{
			D3DXVECTOR2 cutStart(sprite->FloatAttribute("x"), sprite->FloatAttribute("y"));
			D3DXVECTOR2 cutEnd(sprite->FloatAttribute("w"), sprite->FloatAttribute("h"));
			cutEnd += cutStart;

			Texture* texture = Texture::Add(wPath, cutStart, cutEnd, Pivot::CENTERBOTTOM);
			clips.push_back(AnimationClip(texture, time));
		}
		animator = NEW Animator(&clips, type);
		actions.push_back(animator);
	}
	delete document;

}

void XmlLoad::EffectLoad(string key, string filepath, int poolNum)
{
	string xmlFilepath = filepath + ".xml";
	string pngFilepath = filepath + ".png";
	XmlDocument* document = NEW XmlDocument;
	document->LoadFile(xmlFilepath.c_str());

	XmlElement* atlas = document->FirstChildElement();
	wstring wPath;
	wPath.assign(pngFilepath.begin(), pngFilepath.end());

	XmlElement* action = atlas->FirstChildElement();
	for (; action != NULL; action = action->NextSiblingElement())
	{
		DWORD time = action->UnsignedAttribute("time");
		Type type = (Type)action->UnsignedAttribute("type");

		vector<AnimationClip> clips;
		XmlElement* sprite = action->FirstChildElement();
		for (; sprite != NULL; sprite = sprite->NextSiblingElement())
		{
			D3DXVECTOR2 cutStart(sprite->FloatAttribute("x"), sprite->FloatAttribute("y"));
			D3DXVECTOR2 cutEnd(sprite->FloatAttribute("w"), sprite->FloatAttribute("h"));
			cutEnd += cutStart;

			Texture* texture = Texture::Add(wPath, cutStart, cutEnd, Pivot::CENTERBOTTOM);
			clips.push_back(AnimationClip(texture, time));
		}
		Animator* animator = NEW Animator(&clips, type);
		EFFECT->AddEffect(key, animator, (float)time, 10);
	}

	delete document;
}

void XmlLoad::Release()
{
	delete instance;
}
