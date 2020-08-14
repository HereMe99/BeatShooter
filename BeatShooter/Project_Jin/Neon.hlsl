float2 lightPos;
float2 imageSize;
float range;

texture map;
sampler samp
{
	Texture = map;
};

float4 main(float2 uv : TEXCOORD0) : SV_target
{
	float4 result = tex2D(samp, uv);

	float2 pixelCoord = uv * imageSize;

	float dist = distance(pixelCoord, lightPos);

	result -= dist * 0.001f;

	return result;
}

technique tech
{
	pass p0
	{
		AlphaBlendEnable = false;
		PixelShader = compile ps_2_0 main();
	}
}