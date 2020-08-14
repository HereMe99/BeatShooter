#include "stdafx.h"
#include "RenderTarget.h"

RenderTarget::RenderTarget(UINT width, UINT height)
	:width(width), height(height)
{
	shader = Shader::GetShader(ShaderType::DELETE_COLOR);
	targetWorld.pos = { 0,0,0 };
	LPDIRECT3DSURFACE9 colorSurface;
	LPDIRECT3DSURFACE9 depthSurface;

	DEVICE->GetRenderTarget(0, &colorSurface);
	DEVICE->GetDepthStencilSurface(&depthSurface);

	D3DSURFACE_DESC colorDesc;
	colorSurface->GetDesc(&colorDesc);

	D3DSURFACE_DESC depthDesc;
	depthSurface->GetDesc(&depthDesc);

	colorSurface->Release();
	depthSurface->Release();

	colorFormat = colorDesc.Format;
	depthFormat = depthDesc.Format;

	DEVICE->CreateTexture(width, height, 1, D3DUSAGE_RENDERTARGET,
		colorFormat, D3DPOOL_DEFAULT, &texture, NULL);

	texture->GetSurfaceLevel(0, &surface);

	DEVICE->ColorFill(surface, NULL, 0xffffffff);

	D3DXCreateRenderToSurface(DEVICE, width, height,
		colorFormat, TRUE, depthFormat, &renderSurface);

	vertex = new Vertex[6];
	vertex[0] = Vertex(0, 0, 0, 0);
	vertex[1] = Vertex(WIN_WIDTH, 0, 1, 0);
	vertex[2] = Vertex(0, WIN_HEIGHT, 0, 1);
	vertex[3] = Vertex(0, WIN_HEIGHT, 0, 1);
	vertex[4] = Vertex(WIN_WIDTH, 0, 1, 0);
	vertex[5] = Vertex(WIN_WIDTH, WIN_HEIGHT, 1, 1);

	UINT vertexCount = 6;
	UINT vertexSize = vertexCount * sizeof(Vertex);

	DEVICE->CreateVertexBuffer(vertexSize, 0, Vertex::fvf,
		D3DPOOL_DEFAULT, &vertexBuffer, NULL);

	void* vertexData = NULL;
	vertexBuffer->Lock(0, vertexSize, &vertexData, NULL);
	memcpy(vertexData, vertex, vertexSize);
	vertexBuffer->Unlock();

	delete[] vertex;
}

RenderTarget::~RenderTarget()
{
	texture->Release();
	surface->Release();
	renderSurface->Release();
	vertexBuffer->Release();
}

void RenderTarget::BeginScene(DWORD color)
{
	renderSurface->BeginScene(surface, NULL);
	DEVICE->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		color, 1.0f, 0);
	//DEVICE->ColorFill(surface, NULL, 0x00000000);
	//D3DXFillTexture(texture, NULL, NULL); 

}

void RenderTarget::EndScene()
{
	renderSurface->EndScene(0);
}

void RenderTarget::Update()
{
	targetWorld.Update();
}

void RenderTarget::Render()
{ 
	World::InitWorld();
	targetWorld.SetView();
	//targetWorld.SetWorld();
	DEVICE->SetFVF(Vertex::fvf);
	DEVICE->SetTexture(0, texture);
	DEVICE->SetStreamSource(0, vertexBuffer, 0, sizeof(Vertex));

	DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	DEVICE->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	DEVICE->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
	DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//World::InitWorld();
}
