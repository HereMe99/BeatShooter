#pragma once
class RenderTarget
{
public:
	RenderTarget(UINT width = WIN_WIDTH, UINT height = WIN_HEIGHT);
	~RenderTarget();

	void BeginScene(DWORD color = 0x000000);
	void EndScene();
	void Update();
	void Render();

	LPDIRECT3DTEXTURE9 GetTexture() { return texture; }
	void SetTexture(LPDIRECT3DTEXTURE9 texture) { this->texture = texture; }
	World* GetWorld() { return &targetWorld; }
private:
	UINT width, height;

	LPDIRECT3DTEXTURE9 texture;
	LPDIRECT3DSURFACE9 surface;

	LPD3DXRENDERTOSURFACE renderSurface;

	D3DFORMAT colorFormat;
	D3DFORMAT depthFormat;

	World targetWorld;

	Vertex* vertex;
	LPDIRECT3DVERTEXBUFFER9 vertexBuffer;

	LPD3DXEFFECT shader;
};

