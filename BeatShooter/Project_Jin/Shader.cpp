#include "stdafx.h"
#include "Shader.h"

vector<LPD3DXEFFECT> Shader::shaders;


void Shader::Create()
{
	AddShader(L"CharacterShader.hlsl");
	AddShader(L"BlurShader.hlsl");
	AddShader(L"BoundShader.hlsl");
	AddShader(L"MultiShader.hlsl");
	AddShader(L"LightShader.hlsl");
	AddShader(L"DeleteColor.hlsl");
}
 
void Shader::Delete()
{
	for (auto shader : shaders)
		SAFE_RELEASE(shader);
}

void Shader::AddShader(wstring file)
{
	LPD3DXEFFECT shader;
	CreateShader(file, &shader);
	shaders.push_back(shader);
}

void Shader::CreateShader(wstring fileName, LPD3DXEFFECT* shader)
{
	HRESULT hr = D3DXCreateEffectFromFile(DEVICE,
		fileName.c_str(),
		NULL, NULL, D3DXSHADER_DEBUG, NULL, shader, NULL);

	assert(SUCCEEDED(hr));
}

LPD3DXEFFECT Shader::GetShader(ShaderType type)
{
	return shaders[(int)type];
}
