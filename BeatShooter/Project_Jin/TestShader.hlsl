//matrix world;
float4x4 world;
float4x4 view;
float4x4 projection;

struct VertexInput
{
    float4 position : POSITION;
    float2 uv : TEXCOORD0;
};

float4 main(VertexInput input) : POSITION
{
    float4 pos;

    pos = mul(input.position, world);
    pos = mul(pos, view);
    pos = mul(pos, projection);

    return pos;
}

float4 PS(float4 pos : POSITION) : SV_Target
{
    return float4(1, 1, 0, 1);
}

technique tech
{
    pass p0
    {
        VertexShader = compile vs_2_0 main();
        PixelShader = compile ps_2_0 PS();
    }
}