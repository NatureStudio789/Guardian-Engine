cbuffer TransformCBuffer : register(b0)
{
	row_major float4x4 TransformMatrix;
};

struct VertexInput
{
	float3 InputPosition : POSITION;
	float4 InputColor : COLOR;
	float2 InputTextureCoord : TEXCOORD;
};

struct VertexOutput
{
	float4 OutputPosition : SV_POSITION;
	float4 OutputColor : COLOR;
	float2 OutputTextureCoord : TEXCOORD;
};

VertexOutput main( VertexInput InputData )
{
	VertexOutput OutputData;
	OutputData.OutputPosition = mul(float4(InputData.InputPosition, 1.0f), TransformMatrix);
	OutputData.OutputColor = InputData.InputColor;
	OutputData.OutputTextureCoord = InputData.InputTextureCoord;
	return OutputData;
}