#include "stdafx.h"
#include "Texture.h"

bool Texture::isBoundBoxDraw = false;
LPDIRECT3DVERTEXBUFFER9 Texture::vertexBuffer = NULL;
LPDIRECT3DVERTEXBUFFER9 Texture::boundVertexBuffer = NULL;
vector<Texture*> Texture::textures;
map<wstring, LPDIRECT3DTEXTURE9> Texture::textureMap;

Texture::Texture(LPDIRECT3DTEXTURE9 texture, D3DXVECTOR2 cutStart, D3DXVECTOR2 cutEnd, Pivot pivot)
{
	this->texture = texture;
	D3DSURFACE_DESC desc;

	texture->GetLevelDesc(0, &desc);

	textureSize.x = (float)desc.Width;
	textureSize.y = (float)desc.Height;

	uvStart.x = cutStart.x / textureSize.x;
	uvStart.y = cutStart.y / textureSize.y;
	uvEnd.x = (cutEnd.x > 0) ? cutEnd.x / textureSize.x : 1.0f;
	uvEnd.y = (cutEnd.y > 0) ? cutEnd.y / textureSize.y : 1.0f;

	this->cutStart = cutStart;
	this->cutEnd.x = (cutEnd.x < 1) ? textureSize.x : cutEnd.x;
	this->cutEnd.y = (cutEnd.y < 1) ? textureSize.y : cutEnd.y;
	this->pivot = pivot;
	uvPivot = { 0,0 };
}

Texture::Texture(LPDIRECT3DTEXTURE9 texture, D3DXVECTOR2 cutStart, D3DXVECTOR2 cutEnd, D3DXVECTOR2 pivot)
{
	this->texture = texture;
	D3DSURFACE_DESC desc;
	texture->GetLevelDesc(0, &desc);

	textureSize.x = (float)desc.Width;
	textureSize.y = (float)desc.Height;

	uvStart.x = cutStart.x / textureSize.x;
	uvStart.y = cutStart.y / textureSize.y;
	uvEnd.x = (cutEnd.x > 0) ? cutEnd.x / textureSize.x : 1.0f;
	uvEnd.y = (cutEnd.y > 0) ? cutEnd.y / textureSize.y : 1.0f;

	this->cutStart = cutStart;
	this->cutEnd.x = (cutEnd.x < 1) ? textureSize.x : cutEnd.x;
	this->cutEnd.y = (cutEnd.y < 1) ? textureSize.y : cutEnd.y;
	this->pivot = Pivot::NONE;
	this->uvPivot = pivot;
}

Texture::~Texture()
{

}

void Texture::CreateVertexBuffer()
{
	SAFE_RELEASE(vertexBuffer);

	UINT vertexCount = textures.size() * 6;
	UINT vertexSize = vertexCount * sizeof(Vertex);

	DEVICE->CreateVertexBuffer(vertexSize, 0, Vertex::fvf,
		D3DPOOL_DEFAULT, &vertexBuffer, NULL);

	Vertex* vertex = NEW Vertex[vertexCount];
	for (size_t i = 0; i < textures.size(); i++)
	{
		switch (textures[i]->pivot)
		{
		case Pivot::NONE:
		{
			D3DXVECTOR2 size;
			D3DXVECTOR2 pivot = textures[i]->uvPivot;
			textures[i]->GetRealSize(&size);
			D3DXVECTOR2 start = textures[i]->uvStart;
			D3DXVECTOR2 end = textures[i]->uvEnd;

			vertex[i * 6 + 0] = Vertex(0, 0, start.x, start.y);
			vertex[i * 6 + 1] = Vertex(size.x, 0, end.x, start.y);
			vertex[i * 6 + 2] = Vertex(0, size.y, start.x, end.y);
			vertex[i * 6 + 3] = Vertex(0, size.y, start.x, end.y);
			vertex[i * 6 + 4] = Vertex(size.x, 0, end.x, start.y);
			vertex[i * 6 + 5] = Vertex(size.x, size.y, end.x, end.y);
		}
		case Pivot::LEFTTOP:
		{

			D3DXVECTOR2 size;
			textures[i]->GetRealSize(&size);

			D3DXVECTOR2 start = textures[i]->uvStart;
			D3DXVECTOR2 end = textures[i]->uvEnd;

			vertex[i * 6 + 0] = Vertex(0, 0, start.x, start.y);
			vertex[i * 6 + 1] = Vertex(size.x, 0, end.x, start.y);
			vertex[i * 6 + 2] = Vertex(0, size.y, start.x, end.y);
			vertex[i * 6 + 3] = Vertex(0, size.y, start.x, end.y);
			vertex[i * 6 + 4] = Vertex(size.x, 0, end.x, start.y);
			vertex[i * 6 + 5] = Vertex(size.x, size.y, end.x, end.y);
		}
		break;
		case Pivot::CENTERTOP:
		{
			D3DXVECTOR2 size;
			textures[i]->GetRealSize(&size);

			D3DXVECTOR2 start = textures[i]->uvStart;
			D3DXVECTOR2 end = textures[i]->uvEnd;

			vertex[i * 6 + 0] = Vertex(-(size.x * 0.5f), 0, start.x, start.y);
			vertex[i * 6 + 1] = Vertex(size.x * 0.5f, 0, end.x, start.y);
			vertex[i * 6 + 2] = Vertex(-(size.x * 0.5f), size.y, start.x, end.y);
			vertex[i * 6 + 3] = Vertex(-(size.x * 0.5f), size.y, start.x, end.y);
			vertex[i * 6 + 4] = Vertex(size.x * 0.5f, 0, end.x, start.y);
			vertex[i * 6 + 5] = Vertex(size.x * 0.5f, size.y * 0.5f, end.x, end.y);

		}
		break;
		case Pivot::RIGHTTOP:
		{

			D3DXVECTOR2 size;
			textures[i]->GetRealSize(&size);

			D3DXVECTOR2 start = textures[i]->uvStart;
			D3DXVECTOR2 end = textures[i]->uvEnd;

			vertex[i * 6 + 0] = Vertex(-size.x, 0, start.x, start.y);
			vertex[i * 6 + 1] = Vertex(0, 0, end.x, start.y);
			vertex[i * 6 + 2] = Vertex(-size.x, size.y, start.x, end.y);
			vertex[i * 6 + 3] = Vertex(-size.x, size.y, start.x, end.y);
			vertex[i * 6 + 4] = Vertex(0, 0, end.x, start.y);
			vertex[i * 6 + 5] = Vertex(0, size.y, end.x, end.y);

		}
		break;
		case Pivot::LEFTMIDDLE:
		{

			D3DXVECTOR2 size;
			textures[i]->GetRealSize(&size);

			D3DXVECTOR2 start = textures[i]->uvStart;
			D3DXVECTOR2 end = textures[i]->uvEnd;

			vertex[i * 6 + 0] = Vertex(0, -(size.y * 0.5f), start.x, start.y);
			vertex[i * 6 + 1] = Vertex(size.x, -(size.y * 0.5f), end.x, start.y);
			vertex[i * 6 + 2] = Vertex(0, size.y * 0.5f, start.x, end.y);
			vertex[i * 6 + 3] = Vertex(0, size.y * 0.5f, start.x, end.y);
			vertex[i * 6 + 4] = Vertex(size.x, -(size.y * 0.5f), end.x, start.y);
			vertex[i * 6 + 5] = Vertex(size.x, size.y * 0.5f, end.x, end.y);

		}
		break;
		case Pivot::CENTERMIDDLE:
		{

			D3DXVECTOR2 size;
			textures[i]->GetRealSize(&size);

			D3DXVECTOR2 start = textures[i]->uvStart;
			D3DXVECTOR2 end = textures[i]->uvEnd;

			vertex[i * 6 + 0] = Vertex(-(size.x * 0.5f), -(size.y * 0.5f), start.x, start.y);
			vertex[i * 6 + 1] = Vertex(size.x * 0.5f, -(size.y * 0.5f), end.x, start.y);
			vertex[i * 6 + 2] = Vertex(-(size.x * 0.5f), size.y * 0.5f, start.x, end.y);
			vertex[i * 6 + 3] = Vertex(-(size.x * 0.5f), size.y * 0.5f, start.x, end.y);
			vertex[i * 6 + 4] = Vertex(size.x * 0.5f, -(size.y * 0.5f), end.x, start.y);
			vertex[i * 6 + 5] = Vertex(size.x * 0.5f, size.y * 0.5f, end.x, end.y);

		}
		break;
		case Pivot::RIGHTMIDDLE:
		{

			D3DXVECTOR2 size;
			textures[i]->GetRealSize(&size);

			D3DXVECTOR2 start = textures[i]->uvStart;
			D3DXVECTOR2 end = textures[i]->uvEnd;

			vertex[i * 6 + 0] = Vertex(-size.x, -(size.y * 0.5f), start.x, start.y);
			vertex[i * 6 + 1] = Vertex(0, -(size.y * 0.5f), end.x, start.y);
			vertex[i * 6 + 2] = Vertex(-size.x, size.y * 0.5f, start.x, end.y);
			vertex[i * 6 + 3] = Vertex(-size.x, size.y * 0.5f, start.x, end.y);
			vertex[i * 6 + 4] = Vertex(0, -(size.y * 0.5f), end.x, start.y);
			vertex[i * 6 + 5] = Vertex(0, size.y * 0.5f, end.x, end.y);

		}
		break;
		case Pivot::LEFTBOTTOM:
		{

			D3DXVECTOR2 size;
			textures[i]->GetRealSize(&size);

			D3DXVECTOR2 start = textures[i]->uvStart;
			D3DXVECTOR2 end = textures[i]->uvEnd;

			vertex[i * 6 + 0] = Vertex(0, -size.y, start.x, start.y);
			vertex[i * 6 + 1] = Vertex(size.x, -size.y, end.x, start.y);
			vertex[i * 6 + 2] = Vertex(0, 0, start.x, end.y);
			vertex[i * 6 + 3] = Vertex(0, 0, start.x, end.y);
			vertex[i * 6 + 4] = Vertex(size.x, -size.y, end.x, start.y);
			vertex[i * 6 + 5] = Vertex(size.x, 0, end.x, end.y);

		}
		break;
		case Pivot::CENTERBOTTOM:
		{

			D3DXVECTOR2 size;
			textures[i]->GetRealSize(&size);

			D3DXVECTOR2 start = textures[i]->uvStart;
			D3DXVECTOR2 end = textures[i]->uvEnd;

			vertex[i * 6 + 0] = Vertex(-(size.x * 0.5f), -size.y, start.x, start.y);
			vertex[i * 6 + 1] = Vertex(size.x * 0.5f, -size.y, end.x, start.y);
			vertex[i * 6 + 2] = Vertex(-(size.x * 0.5f), 0, start.x, end.y);
			vertex[i * 6 + 3] = Vertex(-(size.x * 0.5f), 0, start.x, end.y);
			vertex[i * 6 + 4] = Vertex(size.x * 0.5f, -size.y, end.x, start.y);
			vertex[i * 6 + 5] = Vertex(size.x * 0.5f, 0, end.x, end.y);

		}
		break;
		case Pivot::RIGHTBOTTOM:
		{

			D3DXVECTOR2 size;
			textures[i]->GetRealSize(&size);

			D3DXVECTOR2 start = textures[i]->uvStart;
			D3DXVECTOR2 end = textures[i]->uvEnd;

			vertex[i * 6 + 0] = Vertex(-size.x, -size.y, start.x, start.y);
			vertex[i * 6 + 1] = Vertex(0, -size.y, end.x, start.y);
			vertex[i * 6 + 2] = Vertex(-size.x, 0, start.x, end.y);
			vertex[i * 6 + 3] = Vertex(-size.x, 0, start.x, end.y);
			vertex[i * 6 + 4] = Vertex(0, -size.y, end.x, start.y);
			vertex[i * 6 + 5] = Vertex(0, 0, end.x, end.y);

		}
		break;
		default:
			break;
		}
	}


	void* vertexData = NULL;
	vertexBuffer->Lock(0, vertexSize, &vertexData, NULL);
	memcpy(vertexData, vertex, vertexSize);
	vertexBuffer->Unlock();

	delete[] vertex;
}

void Texture::CreateBoundVertexBuffer()
{
	SAFE_RELEASE(boundVertexBuffer);

	UINT vertexCount = textures.size() * 5;
	UINT vertexSize = vertexCount * sizeof(BoundVertex);

	DEVICE->CreateVertexBuffer(vertexSize, 0, BoundVertex::fvf,
		D3DPOOL_DEFAULT, &boundVertexBuffer, NULL);

	BoundVertex* vertex = new BoundVertex[vertexCount];
	for (size_t i = 0; i < textures.size(); i++)
	{
		D3DXVECTOR2 size;
		textures[i]->GetRealSize(&size);

		D3DXVECTOR2 halfSize = size * 0.5f;

		vertex[i * 5 + 0] = BoundVertex(-halfSize.x, -halfSize.y, 0xff00ff00);
		vertex[i * 5 + 1] = BoundVertex(halfSize.x, -halfSize.y, 0xff00ff00);
		vertex[i * 5 + 2] = BoundVertex(halfSize.x, halfSize.y, 0xff00ff00);
		vertex[i * 5 + 3] = BoundVertex(-halfSize.x, halfSize.y, 0xff00ff00);
		vertex[i * 5 + 4] = BoundVertex(-halfSize.x, -halfSize.y, 0xff00ff00);
	}

	void* vertexData = NULL;
	boundVertexBuffer->Lock(0, vertexSize, &vertexData, NULL);
	memcpy(vertexData, vertex, vertexSize);
	boundVertexBuffer->Unlock();

	delete[] vertex;
}

Texture* Texture::Add(wstring fileName, D3DXVECTOR2 cutStart, D3DXVECTOR2 cutEnd, Pivot pivot)
{
	LPDIRECT3DTEXTURE9 dxTexture;
	if (textureMap.count(fileName) < 1)
	{
		D3DXCreateTextureFromFileEx(
			DEVICE,
			fileName.c_str(),
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT_NONPOW2,
			1,
			NULL,
			D3DFMT_UNKNOWN,
			D3DPOOL_DEFAULT,
			D3DX_FILTER_NONE,
			D3DX_FILTER_NONE,
			0xffff00ff,
			NULL,
			NULL,
			&dxTexture
		);

		textureMap.insert({ fileName,dxTexture });

	}
	else
	{
		dxTexture = textureMap[fileName];
	}

	Texture* texture = NEW Texture(dxTexture, cutStart, cutEnd, pivot);
	texture->number = textures.size();
	textures.push_back(texture);

	CreateVertexBuffer();
	CreateBoundVertexBuffer();

	return texture;
}

Texture* Texture::Add(wstring fileName, UINT frameX, UINT frameY, UINT maxFrameX, UINT maxFrameY, Pivot pivot)
{
	LPDIRECT3DTEXTURE9 dxTexture;
	if (textureMap.count(fileName) < 1)
	{
		D3DXCreateTextureFromFileEx(
			DEVICE,
			fileName.c_str(),
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT_NONPOW2,
			1,
			NULL,
			D3DFMT_UNKNOWN,
			D3DPOOL_DEFAULT,
			D3DX_FILTER_NONE,
			D3DX_FILTER_NONE,
			0xffff00ff,
			NULL,
			NULL,
			&dxTexture
		);

		textureMap.insert({ fileName,dxTexture });

	}
	else
	{
		dxTexture = textureMap[fileName];
	}
	D3DSURFACE_DESC desc;
	textureMap[fileName]->GetLevelDesc(0, &desc);
	D3DXVECTOR2 start, end;
	D3DXVECTOR2 frameSize = { (float)desc.Width / maxFrameX,(float)desc.Height / maxFrameY };
	start = { frameX * frameSize.x, frameY * frameSize.y };
	end = { start.x + frameSize.x , start.y + frameSize.y };
	Texture* texture = NEW Texture(dxTexture, start, end, pivot);
	texture->number = textures.size();
	textures.push_back(texture);

	CreateVertexBuffer();
	CreateBoundVertexBuffer();

	return texture;
}

Texture* Texture::Find(wstring fileName)
{
	return nullptr;
}

void Texture::Render()
{
	DEVICE->SetFVF(Vertex::fvf);
	DEVICE->SetTexture(0, texture);
	DEVICE->SetStreamSource(0, vertexBuffer, 0, sizeof(Vertex));

	DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	DEVICE->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	DEVICE->DrawPrimitive(D3DPT_TRIANGLELIST, number * 6, 2);
	DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	if (isBoundBoxDraw)
	{
		LPD3DXEFFECT shader = Shader::GetShader(ShaderType::BOUNDBOX);

		shader->Begin(NULL, NULL);
		shader->BeginPass(0);
		DEVICE->SetStreamSource(0, boundVertexBuffer, 0, sizeof(BoundVertex));
		DEVICE->SetFVF(BoundVertex::fvf);
		DEVICE->DrawPrimitive(D3DPT_LINESTRIP, number * 5, 4);
		shader->EndPass();
		shader->End();
	}
}

void Texture::Release()
{
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(boundVertexBuffer);

	for (auto tex : textures)
	{
		SAFE_DELETE(tex);
	}

	for (auto tex : textureMap)
	{
		SAFE_RELEASE(tex.second);
	}
}
