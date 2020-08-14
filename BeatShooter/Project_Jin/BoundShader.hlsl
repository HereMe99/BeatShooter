float4 main(float4 color : COLOR0) : COLOR0
{
	return color;
}

technique tech
{
	pass p0
	{
		PixelShader = compile ps_2_0 main();
	}
}