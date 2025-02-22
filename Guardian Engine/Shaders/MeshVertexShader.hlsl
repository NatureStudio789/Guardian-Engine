cbuffer TransformCBuffer : register(b0)
{
	row_major float4x4 TransformMatrix;
	row_major float4x4 WorldTransformMatrix;
};

struct VertexInput
{
	float3 InputPosition : POSITION;
	float2 InputTextureCoord : TEXCOORD;
	float3 InputNormal : NORMAL;
};

struct VertexOutput
{
	float4 OutputPosition : SV_POSITION;
	float3 OutputWorldPosition : WORLDPOSITION;
	float2 OutputTextureCoord : TEXCOORD;
	float3 OutputNormal : NORMAL;
};

VertexOutput main( VertexInput InputData )
{
	VertexOutput OutputData;
	OutputData.OutputPosition = mul(float4(InputData.InputPosition, 1.0f), TransformMatrix);
	OutputData.OutputWorldPosition = mul(float4(InputData.InputPosition, 1.0f), WorldTransformMatrix).xyz;
	OutputData.OutputTextureCoord = InputData.InputTextureCoord;
	OutputData.OutputNormal = normalize(mul(float4(InputData.InputNormal, 1.0f), WorldTransformMatrix).xyz);
	return OutputData;
}
