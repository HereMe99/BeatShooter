texture map;
sampler samp
{
    Texture = map;
};

float2 imageSize;
float alpha;
float4 color;

float4 main(float2 uv : TEXCOORD0) : SV_Target
{
    float4 result = tex2D(samp, uv);
    result.w *= alpha;
    return result;
}

float4 GaussianBlur(float2 uv : TEXCOORD) : SV_Target
{
    
    const float weight[7] =
    {
        0.486f, 0.726f, 0.923f, 1, 0.923f, 0.726f, 0.486f
    };

    float4 result = float4(0, 0, 0, 0);
    float divX = 1 / imageSize.x;
    float divY = 1 / imageSize.y;
    int i = 0;
    for (i = -3; i <= 3; i++)
    {
        result += weight[i + 3] * tex2D(samp, float2(uv.x + i * divX, uv.y));
        result += weight[i + 3] * tex2D(samp, float2(uv.x, uv.y + i * divY));
    }

    result /= 2.135f * 2 * 2 + 2;
    //result.xyz *= 1+1-alpha;
   
    result.w *= alpha;
    return result;
}
float4 ColorShader(float2 uv : TEXCOORD0) : SV_Target
{
    float4 result = tex2D(samp, uv);
    return result * color;
}

technique tech
{
    pass p0
    {
        PixelShader = compile ps_2_0 main();
    }
    pass p1
    {
        PixelShader = compile ps_2_0 GaussianBlur();
    }
    pass p2
    {
        PixelShader = compile ps_2_0 ColorShader();
    }

}