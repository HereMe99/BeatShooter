texture map1;
sampler samp1
{
    MinFilter = Point;
    MagFilter = Point;
    Texture = map1;
};

texture map2;
sampler samp2
{
    MinFilter = Linear;
    MagFilter = Linear;
    Texture = map2;
};

float4 main(float2 uv : TEXCOORD0) : SV_Target
{
    float4 tex1 = tex2D(samp1, uv);
    float4 tex2 = tex2D(samp2, uv);

    //여기 추가
    //tex1.rgb = tex2.rgb;
    if(tex1.a > 0.0f)
        return tex2;

    return tex1;
}

float4 Modulate(float2 uv : TEXCOORD0) : SV_Target
{
    float4 tex1 = tex2D(samp1, uv);
    float4 tex2 = tex2D(samp2, uv);
        
    return tex1 * tex2 * 2;
}

float4 Add(float2 uv : TEXCOORD0) : SV_Target
{
    float4 tex1 = tex2D(samp1, uv);
    float4 tex2 = tex2D(samp2, uv);
        
    return tex1 + tex2;
}

float4 AddSmooth(float2 uv : TEXCOORD0) : SV_Target
{    
    float4 tex1 = tex2D(samp1, uv);
    float4 tex2 = tex2D(samp2, uv);
        
    return tex1 + tex2 - tex1*tex2;
}

float4 AddSigned(float2 uv : TEXCOORD0) : SV_Target
{
    float4 tex1 = tex2D(samp1, uv);
    float4 tex2 = tex2D(samp2, uv);
        
    return (tex1 + tex2 - 0.5f) * 2;
}

float4 Subtract(float2 uv : TEXCOORD0) : SV_Target
{
    float4 tex1 = tex2D(samp1, uv);
    float4 tex2 = tex2D(samp2, uv);
        
    return tex1 - tex2;
}

float4 InvSubtract(float2 uv : TEXCOORD0) : SV_Target
{
    float4 tex1 = tex2D(samp1, uv);
    float4 tex2 = tex2D(samp2, uv);
        
    return tex2 - tex1;
}

float4 PointFilter(float2 uv : TEXCOORD0) : SV_Target
{
    float4 tex = tex2D(samp1, uv);    
        
    return tex;
}

float4 LinearFilter(float2 uv : TEXCOORD0) : SV_Target
{
    float4 tex = tex2D(samp2, uv);
        
    return tex;
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
    pass p1
    {
        AlphaBlendEnable = true;
        BlendOp = Add;
        DestBlend = InvSrcAlpha;
        SrcBlend = SrcAlpha;
        PixelShader = compile ps_2_0 Modulate();
    }
    pass p2
    {
        AlphaBlendEnable = true;
        BlendOp = Add;
        DestBlend = InvSrcAlpha;
        SrcBlend = SrcAlpha;
        PixelShader = compile ps_2_0 Add();
    }
    pass p3
    {
        AlphaBlendEnable = true;
        BlendOp = Add;
        DestBlend = InvSrcAlpha;
        SrcBlend = SrcAlpha;
        PixelShader = compile ps_2_0 AddSmooth();
    }
    pass p4
    {
        AlphaBlendEnable = true;
        BlendOp = Add;
        DestBlend = InvSrcAlpha;
        SrcBlend = SrcAlpha;
        PixelShader = compile ps_2_0 AddSigned();
    }
    pass p5
    {
        AlphaBlendEnable = false;        
        PixelShader = compile ps_2_0 Subtract();

    }
    pass p6
    {
        AlphaBlendEnable = false;        
        PixelShader = compile ps_2_0 InvSubtract();
    }
    pass p7
    {
        AlphaBlendEnable = false;
        PixelShader = compile ps_2_0 PointFilter();
    }
    pass p8
    {
        AlphaBlendEnable = false;
        PixelShader = compile ps_2_0 LinearFilter();
    }
}