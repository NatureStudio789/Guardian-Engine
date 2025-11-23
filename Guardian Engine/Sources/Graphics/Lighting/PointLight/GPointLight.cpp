#include "GPointLight.h"
#include "../../../Gameplay/Scene/GScene.h"

namespace GE
{
	GPointLight::GPointLight()
	{
		this->PointLightId = GUUID();
		this->LightData = Data();

		this->IsLightRegistered = false;

		this->LightDepthCubeMap = std::make_shared<GDepthCubeMap>();
		this->LightCameraCBuffer = null;
	}

	GPointLight::GPointLight(const GPointLight& other)
	{
		this->PointLightId = other.PointLightId;
		this->LightData = other.LightData;

		this->IsLightRegistered = other.IsLightRegistered;

		this->LightDepthCubeMap = other.LightDepthCubeMap;
		this->LightCameraCBuffer = other.LightCameraCBuffer;
	}

	GPointLight::GPointLight(const GVector3& position,
		const GVector3& color, float strength)
	{
		this->PointLightId = GUUID();

		this->LightData = { position, color, strength };
		this->IsLightRegistered = false;

		this->LightDepthCubeMap = std::make_shared<GDepthCubeMap>();
		this->LightCameraCBuffer = null;
	}

	GPointLight::~GPointLight()
	{

	}

	void GPointLight::InitializeDepthRendering()
	{
		this->LightDepthCubeMap->InitializeDepthCubeMap(1024);

		this->LightCameraCBuffer = GCameraCBuffer::CreateNewCameraCBuffer(
			GPipelineStateRegistry::GetPipelineState(GPipelineStateRegistry::DEPTH_PSO)->GetPipelineRootSignature());
	}

	void GPointLight::Register(GScene* scene)
	{
		scene->GetLightRegistry()->RegisterPointLight(this);
		this->IsLightRegistered = true;
	}

	void GPointLight::Unregister(GScene* scene)
	{
		scene->GetLightRegistry()->UnregisterPointLight(this);
		this->IsLightRegistered = false;
	}

	void GPointLight::UpdateDepthRendering(UINT faceIndex)
	{
		static GVector3 CameraAllAxisesRotations[] =
		{
			{ 0.0f,   90.0f,  0.0f },
			{ 0.0f,   -90.0f, 0.0f },
			{ 90.0f,  0.0f,   0.0f },
			{ -90.0f, 0.0f,   0.0f },
			{ 0.0f,   0.0f,   0.0f },
			{ 0.0f,   180.0f, 0.0f },
		};

		GCamera LightCamera = { this->LightData.Position, CameraAllAxisesRotations[faceIndex], GPerspectiveProjection(90.0f, 1024 / 1024, 1.0f, 100.0f)};

		auto& CameraMatrix = LightCamera.GetViewMatrix() * LightCamera.Projection.GetProjectionMatrix();
		CameraMatrix.Transpose();
		this->LightCameraCBuffer->UpdateBufferData(GCameraCBData(CameraMatrix, this->LightData.Position));
	}

	const GUUID& GPointLight::GetPointLightId() const noexcept
	{
		return this->PointLightId;
	}

	const bool& GPointLight::GetLightRegistered() const noexcept
	{
		return this->IsLightRegistered;
	}
}
