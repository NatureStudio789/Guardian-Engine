struct PixelInput
{
	float4 InputPosition : SV_POSITION;
	float4 InputColor : COLOR;
	float2 InputTextureCoord : TEXCOORD;
};

Texture2D ObjectTexture : TEXTURE : register(t0);
SamplerState ObjectSampler : SAMPLER : register(s0);

float4 main( PixelInput InputData ) : SV_TARGET
{
	return float4(ObjectTexture.Sample(ObjectSampler, InputData.InputTextureCoord).rgb + 
		mul(InputData.InputColor.rgb, InputData.InputColor.a), 1.0f);
}