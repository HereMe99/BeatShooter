#pragma once
#define DEFAULT_BG_WORLD_SCALE {1.5f,1.5f}
enum class ColorState
{
	WHITE,
	RED,
	GREEN,
	BLUE,
};
class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	virtual void Init()			override;
	virtual void Update()		override;
	virtual void LastUpdate()	override;
	virtual void Render()		override;
	virtual void Release()		override;

	void ColorUpdate();
private:
	const UINT			BLUR_COUNT;

	ColorState			mColorState;
	UINT				mRenderCount;
	World				mBgWorld;
	World				mBgLightWorld;
	World				mCursorWorld;
	World				mGameoverWorld;
	Vector2				mBgSize;
	D3DXCOLOR			mColor;

	LPD3DXEFFECT		mShader;
	LPD3DXEFFECT		mColorShader;
	RenderTarget*		mRenderTarget;
	MotionTrailer*			mCursorBlur;
	Texture*			mBgTexture;
	Texture*			mBgBackTexture;
	Texture*			mBgLightTexture;
	Texture*			mCursorTexture;
	Texture*			mGameoverTexture;
};

