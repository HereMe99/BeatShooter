#pragma once
enum class Pivot
{
	NONE,
	LEFTTOP,
	CENTERTOP,
	RIGHTTOP,
	LEFTMIDDLE,
	CENTERMIDDLE,
	RIGHTMIDDLE,
	LEFTBOTTOM,
	CENTERBOTTOM,
	RIGHTBOTTOM
};

class Texture
{
private:
	struct BoundVertex
	{
		D3DXVECTOR3 position;
		DWORD color;

		static const DWORD fvf = D3DFVF_XYZ | D3DFVF_DIFFUSE;

		BoundVertex():color(0x00000000) {}
		BoundVertex(float x, float y, DWORD color)
		{
			position = { x, y, 0 };
			this->color = color;
		}
	};

	UINT number;

	LPDIRECT3DTEXTURE9 texture;
	D3DXVECTOR2 textureSize;

	D3DXVECTOR2 cutStart;
	D3DXVECTOR2 cutEnd;
	D3DXVECTOR2 uvStart;
	D3DXVECTOR2 uvEnd;
	Vector2 uvPivot;

	//World world;

	Pivot pivot;

	static LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
	static LPDIRECT3DVERTEXBUFFER9 boundVertexBuffer;

	static bool isBoundBoxDraw;

	static vector<Texture*> textures;
	static map<wstring, LPDIRECT3DTEXTURE9> textureMap;

	Texture(LPDIRECT3DTEXTURE9 texture, D3DXVECTOR2 cutStart, D3DXVECTOR2 cutEnd, Pivot pivot);
	Texture(LPDIRECT3DTEXTURE9 texture, D3DXVECTOR2 cutStart, D3DXVECTOR2 cutEnd, D3DXVECTOR2 pivot);
	~Texture();


	static void CreateVertexBuffer();
	static void CreateBoundVertexBuffer();
public:
	static Texture* Add(wstring fileName,
		D3DXVECTOR2 cutStart = D3DXVECTOR2(0.0f, 0.0f),
		D3DXVECTOR2 cutEnd = D3DXVECTOR2(0.0f, 0.0f),
		Pivot pivot = Pivot::CENTERMIDDLE);

	static Texture* Add(wstring fileName,
		UINT frameX, UINT frameY, UINT maxFrameX, UINT maxFrameY, Pivot pivot = Pivot::CENTERMIDDLE);
	static Texture* Find(wstring fileName);
	void Render();
	static void Release();

	void SetWorldScale(OUT World* world, IN D3DXVECTOR2 size ) const
	{
		world->scale.x = size.x / textureSize.x;
		world->scale.y = size.y / textureSize.y;
		world->size = size;
	}

	void GetTextureSize(D3DXVECTOR2* size) const
	{
		*size = this->textureSize;
	}

	void GetRealSize(D3DXVECTOR2* size) const
	{
		*size = cutEnd - cutStart;
	}
	void GetUvStart(D3DXVECTOR2* uvStart) const
	{
		*uvStart = this->uvStart;
	}

	void GetUvEnd(D3DXVECTOR2* uvEnd) const
	{
		*uvEnd = this->uvEnd;
	}
	Pivot GetPivot() 
	{
		return pivot;
	}
	LPDIRECT3DTEXTURE9 GetDxTexture() { return texture; }

	static void SetBoundBox(bool isDraw) { isBoundBoxDraw = isDraw; }
};

