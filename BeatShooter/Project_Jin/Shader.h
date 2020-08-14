#pragma once

enum class ShaderType
{
	ALPHAON,
	BLUR,
	BOUNDBOX,
	MULTI,
	LIGHT,
	DELETE_COLOR
};

class Shader
{
private:
	static vector<LPD3DXEFFECT> shaders;

public:
	static void Create();
	static void Delete();

	static void AddShader(wstring file);

	static void CreateShader(wstring fileName, LPD3DXEFFECT* shader);

	static LPD3DXEFFECT GetShader(ShaderType type);
};

