#pragma once

struct AnimationClip
{
	Texture* mTexture;
	DWORD mTime;

	AnimationClip()
	{
		mTexture = nullptr;
		mTime = 0;
	}
	AnimationClip(Texture * texture, DWORD time)
	{
		this->mTexture = texture;
		this->mTime = time;
	}
};

enum class Type
{
	END,
	LOOP,
	PINGPONG
};

class Animator
{
public:
	Animator(vector<AnimationClip>* srcClips,Type type = Type::END);
	~Animator();

	function<void()> mStopFunc;

	void Update();
	void Render();

	void Play();
	void Pause();
	void Start();
	void Stop();

	inline Texture* GetTexture();
	inline LPDIRECT3DTEXTURE9 GetDxTexture();
	inline vector<AnimationClip> GetClips() const; 
	inline UINT GetCurrentClipNumber() const;
	inline bool GetIsPlay() const;

	inline void SetParentsWorld(World* parentsWorld);
	inline void SetAniWorld(World* world);
	inline void SetCurClipNum(int clipNum);

	inline void CurClipClipNumInit();

private:
	vector<AnimationClip>	mClips;
	Type					mRepeatType;
	bool					mIsPlay;
	bool					mIsReverse;
	DWORD					mPlayTime;
	UINT					mCurClipNum;
	UINT					mMaxFrameNum;
	World					mAniWorld;
};

inline Texture* Animator::GetTexture()
{
	return mClips[mCurClipNum].mTexture;
}

inline LPDIRECT3DTEXTURE9 Animator::GetDxTexture()
{
	return mClips[mCurClipNum].mTexture->GetDxTexture();
}

inline vector<AnimationClip> Animator::GetClips() const
{
	return mClips;
}

inline UINT Animator::GetCurrentClipNumber() const
{
	return mCurClipNum;
}
inline bool Animator::GetIsPlay() const
{
	return mIsPlay;
}

inline void Animator::SetParentsWorld(World* parentsWorld)
{
	mAniWorld.parentsWorld = &parentsWorld->matWorld;
}

inline void Animator::SetAniWorld(World* world)
{
	mAniWorld = *world;
}

inline void Animator::SetCurClipNum(const int clipNum)
{
	this->mCurClipNum = clipNum;
}

inline void Animator::CurClipClipNumInit()
{
	mCurClipNum = 0;
}
