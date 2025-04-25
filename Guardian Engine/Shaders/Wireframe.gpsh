struct PixelInput
{
	float4 InputPosition : SV_POSITION;
	float3 InputColor : COLOR;
};

float4 main(PixelInput InputData) : SV_TARGET
{
	return float4(InputData.InputColor, 1.0f);
}