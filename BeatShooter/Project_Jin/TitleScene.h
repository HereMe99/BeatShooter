#pragma once
class TitleScene : public Scene
{
public:
	TitleScene();
	~TitleScene();
	virtual void Init() override;
	virtual void Update() override;
	virtual void LastUpdate() override;
	virtual void Render() override;
	virtual void Release() override;

	void GoPlayScene();
private:
	World mTitleWorld;
	World mBgLightWorld;
	World mBgWorld;
	World mCursorWorld;

	Texture* mBgTexture;
	Texture* mTitleTexture;
	Texture* mStartBtnTexture;
	Texture* mBgLightTexture;
	Texture* mCursorTexture;
	Button* mStartBtn;
};

