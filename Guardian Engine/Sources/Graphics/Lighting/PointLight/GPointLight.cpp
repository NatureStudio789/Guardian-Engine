#include "GPointLight.h"
#include "../../../Gameplay/Scene/GScene.h"

namespace GE
{
	GPointLight::GPointLight()
	{
		this->PointLightId = GUUID();
		this->LightData = Data();

		this->IsLightRegistered = false;

		this->LightDepthMap = std::make_shared<GDepthMap>();
		this->LightCameraCBuffer = null;
	}

	GPointLight::GPointLight(const GPointLight& other)
	{
		this->PointLightId = other.PointLightId;
		this->LightData = other.LightData;

		this->IsLightRegistered = other.IsLightRegistered;

		this->LightDepthMap = other.LightDepthMap;
		this->LightCameraCBuffer = other.LightCameraCBuffer;
	}

	GPointLight::GPointLight(const GVector3& position,
		const GVector3& color, float strength)
	{
		this->PointLightId = GUUID();

		this->LightData = { position, color, strength };
		this->IsLightRegistered = false;

		this->LightDepthMap = std::make_shared<GDepthMap>();
		this->LightCameraCBuffer = null;
	}

	void GPointLight::InitializeDepthRendering()
	{
		this->LightDepthMap->InitializeDepthMap(1024, 1024);

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

	void GPointLight::UpdateDepthRendering()
	{
		GCamera LightCamera = { this->LightData.Position, GVector3(),GPerspectiveProjection(90.0f, 1024 / 1024, 1.0f, 25.0f) };

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
