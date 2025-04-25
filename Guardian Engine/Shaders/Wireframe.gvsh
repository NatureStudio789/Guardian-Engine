cbuffer TransformCBuffer : register(b0)
{
	row_major float4x4 TransformMatrix;
};

struct VertexInput
{
	float3 InputPosition : POSITION;
	float3 InputColor : COLOR;
};

struct VertexOutput
{
	float4 OutputPosition : SV_POSITION;
	float3 OutputColor : COLOR;
};

VertexOutput main( VertexInput InputData )
{
	VertexOutput OutputData;
	OutputData.OutputPosition = mul(float4(InputData.InputPosition, 1.0f), TransformMatrix);
	OutputData.OutputColor = InputData.InputColor;
	return OutputData;
}