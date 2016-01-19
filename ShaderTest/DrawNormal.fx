uniform float4x4 worldMatrix : World;
uniform float4x4 viewMatrix : View;
uniform float4x4 projMatrix : Projection;

uniform float4 normalColor;

static float4x4 MVPMatrix;

float4 vs_main(float3 position : POSITION0) : POSITION0
{
    MVPMatrix = mul(worldMatrix, viewMatrix);
    MVPMatrix = mul(MVPMatrix, projMatrix);
    
    return mul(float4(position, 1.0f), MVPMatrix);
};

float4 ps_main() : COLOR
{
    return normalColor;
};

technique DrawNormalShader
{
    pass P0
    {
        VertexShader = compile vs_2_0 vs_main();
        PixelShader = compile ps_2_0 ps_main();
    }
};