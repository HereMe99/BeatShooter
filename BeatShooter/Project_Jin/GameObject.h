#pragma once
enum class ObjTag
{
	PLAYER,
	ENEMY,
	BOSSENEMY,
	PLAYERBULLET,
	ENEMYBULLET,
	NONE
};
class GameObject
{
public:
	GameObject();
	virtual ~GameObject();
	virtual void Init();
	virtual void Update()		= 0;
	virtual void LastUpdate()	= 0;
	virtual void PrevRender()	= 0;
	virtual void Render()		= 0;

	bool CollisionObjByCicle(GameObject& obj);
	bool CollisionObjPushByCicle(GameObject& obj);
	inline World& GetWorld();
	inline ObjTag GetTag();
protected:
	World mWorld;
	ObjTag mTag;
};

inline ObjTag GameObject::GetTag()
{
	return mTag;
}


inline World& GameObject::GetWorld()
{
	return mWorld;
}
