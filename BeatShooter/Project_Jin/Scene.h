#pragma once
class Scene
{
protected:
	Camera* camera;
public:
	Scene()
	{
		camera = NEW Camera;
		camera->SetProjection();
	}
	virtual ~Scene() { delete camera; }

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void LastUpdate() = 0;
	virtual void Render() = 0;
	virtual void Release() = 0;
};
