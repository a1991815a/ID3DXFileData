uniform float4x4	worldMatrix : World;
uniform float4x4	viewMatrix : View;
uniform float4x4	projMatrix : Projection;

static float4x4 MVPMatrix;

texture texture0;

sampler TexSampler = sampler_state {
	Texture = <texture0>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};

struct VS_INPUT {
	float3 position : POSITION;
	float2 texcoord : TEXCOORD;
};

struct VS_OUTPUT {
	float4 position : POSITION;
	float2 texcoord : TEXCOORD;
};

VS_OUTPUT vs_main(VS_INPUT input) {
	VS_OUTPUT output = (VS_OUTPUT)0;
	MVPMatrix = mul(worldMatrix, viewMatrix);
	MVPMatrix = mul(MVPMatrix, projMatrix);

	output.position = mul(float4(input.position, 1.0f), MVPMatrix);
	output.texcoord = input.texcoord;

	return output;
};

float4 ps_main(
	float2 texcoord : TEXCOORD
	)
	:COLOR
{
	return tex2D(TexSampler, texcoord);
};

technique NormalShader {
	pass P0 {
		VertexShader = compile vs_2_0 vs_main();
		PixelShader = compile ps_2_0 ps_main();
	}
};