#pragma once
class TextureTest: public Scene
{
	struct Vertex
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 uv;
		
		static const DWORD fvf = D3DFVF_XYZ | D3DFVF_TEX1;

		Vertex() {}
		Vertex(float x, float y, float u, float v)
		{
			position = { x,y,0 };
			uv = { u,v };
		}
	};

	Vertex * vertex;

	D3DXMATRIX world;
	LPDIRECT3DTEXTURE9 texture;
	D3DXMATRIX projection;

public:
	TextureTest();
	~TextureTest();

	void Update();
	void Render();
};

