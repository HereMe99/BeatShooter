#pragma once
struct Vertex
{
	static const DWORD fvf = D3DFVF_XYZ | D3DFVF_TEX1;

	Vector3 position;
	Vector2 uv;

	Vertex() {}
	Vertex(float x, float y, float u, float v)
	{
		position = { x, y, 0 };
		uv = { u, v };
	}
};