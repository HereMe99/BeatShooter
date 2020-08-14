//BlurShader
float2 imageSize;
int value;
float2 power;
texture map;
sampler samp
{
	Texture = map;
};

float4 main(float2 uv : TEXCOORD) : SV_TARGET
{
	float4 result = tex2D(samp, uv);

	return result;
}

float4 Blur(float2 uv : TEXCOORD) : SV_TARGET
{
	float4 result = tex2D(samp, uv);

	[unroll(4)]
	for (int i = 0; i < value; i++)
	{
		float divX = (1 + i) / imageSize.x;
		float divY = (1 + i) / imageSize.y;

		result += tex2D(samp, float2(uv.x + divX, uv.y)); //¿À¸¥ÂÊ ÇÈ¼¿
		result += tex2D(samp, float2(uv.x - divX, uv.y)); //¿ÞÂÊ ÇÈ¼¿
		result += tex2D(samp, float2(uv.x, uv.y + divY)); //¾Æ·§ÂÊ ÇÈ¼¿
		result += tex2D(samp, float2(uv.x, uv.y - divY)); //À­ÂÊ ÇÈ¼¿
	}

	result /= value * 4 + 1;

	return result;
}

float4 Blur2(float2 uv : TEXCOORD) : SV_TARGET
{
	float4 result = tex2D(samp, uv);

	float divX = (1 + value) / imageSize.x;
	float divY = (1 + value) / imageSize.y;

	result += tex2D(samp, float2(uv.x + divX, uv.y)); //¿À¸¥ÂÊ ÇÈ¼¿
	result += tex2D(samp, float2(uv.x - divX, uv.y)); //¿ÞÂÊ ÇÈ¼¿
	result += tex2D(samp, float2(uv.x, uv.y + divY)); //¾Æ·§ÂÊ ÇÈ¼¿
	result += tex2D(samp, float2(uv.x, uv.y - divY)); //À­ÂÊ ÇÈ¼¿

	result /= 4 + 1;

	return result;
}

float4 Blur3(float2 uv : TEXCOORD) : SV_TARGET
{
	float4 result = tex2D(samp, uv);

	for (int i = 0; i < 4; i++)
	{
		float divX = (1 + i * value) / imageSize.x;
		float divY = (1 + i * value) / imageSize.y;

		result += tex2D(samp, float2(uv.x + divX, uv.y)); //¿À¸¥ÂÊ ÇÈ¼¿
		result += tex2D(samp, float2(uv.x - divX, uv.y)); //¿ÞÂÊ ÇÈ¼¿
		result += tex2D(samp, float2(uv.x, uv.y + divY)); //¾Æ·§ÂÊ ÇÈ¼¿
		result += tex2D(samp, float2(uv.x, uv.y - divY)); //À­ÂÊ ÇÈ¼¿
	}

	result /= 4 * 4 + 1;

	return result;
}

float4 GaussianBlur(float2 uv : TEXCOORD) : SV_Target
{
	const float weight[7] =
	{
		0.486f, 0.726f, 0.923f, 1, 0.923f, 0.726f, 0.486f
	};

	float4 result = float4(0, 0, 0, 0);
 //   float divX = (1 + value) / imageSize.x;
   // float divX = 1 / imageSize.x;
   // float divY = 1 / imageSize.y;
    float divX = 1 + power.x / imageSize.x;
    float divY = 1 + power.y / imageSize.y;

	int i = 0;
	for (i = -3; i <= 3; i++)
	{
		float2 temp = float2(uv.x + i * divX, uv.y);
		result += weight[i + 3] * tex2D(samp, temp);

		temp = float2(uv.x, uv.y + i * divY);
		result += weight[i + 3] * tex2D(samp, temp);
	}

	result /= 2.135f * 2 * 2 + 2;
	//return float4(result.x, result.y, result.z, 1);
	return result;
}

float4 Mosaic(float2 uv : TEXCOORD) : SV_TARGET
{
	int x = (int)(uv.x * value);
	int y = (int)(uv.y * value);

	float2 temp;
	temp.x = (float)x / value;
	temp.y = (float)y / value;

	return tex2D(samp, temp);
}

technique tech
{
	pass p0
	{
		PixelShader = compile ps_2_0 main();
	}

	pass p1
	{
		PixelShader = compile ps_2_0 Blur();
	}

	pass p2
	{
		PixelShader = compile ps_2_0 Blur2();
	}

	pass p3
	{
		PixelShader = compile ps_2_0 Blur3();
	}

	pass p4
	{
		PixelShader = compile ps_2_0 GaussianBlur();
	}

	pass p5
	{
		PixelShader = compile ps_2_0 Mosaic();
	}
}