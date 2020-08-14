texture map;
sampler samp
{
    Texture = map;
};

float4 color;
float2 cutStart;
float2 cutEnd;

float4 main(float2 uv : TEXCOORD0) : SV_Target
{

    float4 color = tex2D(samp, uv);

    if(color.r == 0 && color.g ==  0 && color.b == 0)
        color.a = 0;

    return color;
}

technique tech
{
    pass p0
    {
        AlphaBlendEnable = true;
        BlendOp = Add;
        DestBlend = InvSrcAlpha;
        SrcBlend = SrcAlpha;
        PixelShader = compile ps_2_0 main();
    }

}