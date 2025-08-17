cbuffer TransformCBuffer : register(b0)
{
    float4x4 WorldMatrix;
};

cbuffer CameraCBuffer : register(b1)
{
    float4x4 CameraMatrix;
    float3 CameraPosition;
};