#pragma once
class EndScene : public Scene
{
public:
	EndScene();
	~EndScene();
	virtual void Init() override;
	virtual void Update() override;
	virtual void LastUpdate() override;
	virtual void Render() override;
	virtual void Release() override;
	void GoPlayScene();
	void GoTitleScene();
private:
	World mGameoverWorld;
	World mTitleWorld;
	World mBgLightWorld;
	World mBgWorld;
	World mCursorWorld;

	Texture* mGameoverTexture;
	Texture* mBgTexture;
	Texture* mRestartBtnTexture;
	Texture* mTitleBtnTexture;
	Texture* mBgLightTexture;
	Texture* mCursorTexture;
	Button* mRestartBtn;
	Button* mTitleBtn;
};

