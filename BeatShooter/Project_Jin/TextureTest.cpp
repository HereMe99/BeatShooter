#include "stdafx.h"
#include "TextureTest.h"


TextureTest::TextureTest()
{
	//D3DXCreateTextureFromFile(DEVICE, L"image/character.png", &texture);
	D3DXCreateTextureFromFileEx(
		DEVICE,
		L"image/character.png",
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
		&texture
	);
	D3DSURFACE_DESC desc;
	texture->GetLevelDesc(0, &desc);

	vertex = NEW Vertex[6];
	vertex[0] = Vertex(0, 0, 0, 0);
	vertex[1] = Vertex(desc.Width, 0, 1, 0);
	vertex[2] = Vertex(0, desc.Height, 0, 1);

	vertex[3] = Vertex(0, desc.Height, 0, 1);
	vertex[4] = Vertex(desc.Width, 0, 1, 0);
	vertex[5] = Vertex(desc.Width, desc.Height, 1, 1);

	D3DXMatrixOrthoOffCenterLH(&projection, 0, WIN_WIDTH, WIN_HEIGHT, 0, -1, 1);
	DEVICE->SetTransform(D3DTS_PROJECTION, &projection);
	DEVICE->SetRenderState(D3DRS_LIGHTING, false);
}


TextureTest::~TextureTest()
{
	texture->Release();
	delete[] vertex;
}

void TextureTest::Update()
{
	
}

void TextureTest::Render()
{
	DEVICE->SetFVF(Vertex::fvf);
	DEVICE->SetTexture(0, texture);

	//���Ŀ����� �����ϰڴ�.
	DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	//���Ŀ����� �Ҳ��� �������� �����ϰڴ�
	DEVICE->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	//SRC : �Ѹ��� ��������
	//DEST : �ѷ����� �� ����� ����
	//SRCALPHA : �Ѹ� ������ ���İ�(���� ������ �Ҷ��� ���İ��� 0~1)
	//INVSRCALPHA : 1 - SRCALPHA
	DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);


	DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	DEVICE->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, vertex, sizeof(Vertex));


	DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

}
