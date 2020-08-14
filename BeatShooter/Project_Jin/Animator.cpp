#include "stdafx.h"
#include "Animator.h"

Animator::Animator(vector<AnimationClip>* srcClips, Type type)
	:mRepeatType(type) , mCurClipNum(0),mIsPlay(false),mIsReverse(false),mStopFunc(nullptr)
{
	for (auto clip : *srcClips)
	{
		AnimationClip aniClip = clip;
		aniClip.mTime = clip.mTime < 1 ? 100 : clip.mTime;
		mClips.push_back(clip);
	}
}

Animator::~Animator()
{
}

void Animator::Update()
{
	if (!mIsPlay)
		return;

	AnimationClip clip = mClips[mCurClipNum];
	mAniWorld.Update();

	if (timeGetTime() - mPlayTime > clip.mTime)
	{
		switch (mRepeatType)
		{
		case Type::END:
			mCurClipNum++;
			if (mCurClipNum >= mClips.size())
				Stop();
			break;
		case Type::LOOP:
			mCurClipNum++;
			mCurClipNum %= mClips.size();
			break;
		case Type::PINGPONG:
			if (mIsReverse)
			{
				mCurClipNum--;
				if (mCurClipNum <=0)
					mIsReverse = false;
			}
			else
			{
				mCurClipNum++;
				if (mCurClipNum >= mClips.size()-1)
					mIsReverse = true;
			}
			break;
		default:
			break;
		}
		mPlayTime = timeGetTime();
	}
}

void Animator::Render()
{
	mAniWorld.SetWorld();
	mClips[mCurClipNum].mTexture->Render();
}

void Animator::Play()
{
	mIsPlay = true;
	mPlayTime = timeGetTime();
}

void Animator::Pause()
{
	mIsPlay = false;
}

void Animator::Start()
{
	mIsPlay = true;
	mIsReverse = false;
	mCurClipNum = 0;
}

void Animator::Stop()
{
	mIsPlay = false;
	mIsReverse = false;
	mCurClipNum = 0;

	if (mStopFunc != nullptr)
		mStopFunc();
}





