#ifndef _GE_GCOMPONENT_H_
#define _GE_GCOMPONENT_H_
#include "../../Asset/Loader/GAssetLoaderRegistry.h"

namespace GE
{
	struct GUARDIAN_API GTagComponent
	{
	public:
		GTagComponent()
		{
			this->Tag = "";
		}
		GTagComponent(const std::string& tag)
		{
			this->Tag = tag;
		}
		GTagComponent(const GTagComponent& other) = default;

		std::string Tag;
	};

	struct GUARDIAN_API GTransformComponent
	{
	public:
		GTransformComponent()
		{
			this->Transform = GTransform();
			this->AccumulatedMatrix = GMatrix::IdentityMatrix();
		}
		GTransformComponent(const GTransform& transform)
		{
			this->Transform = transform;
			this->AccumulatedMatrix = GMatrix::IdentityMatrix();
		}
		GTransformComponent(const GTransformComponent& ) = default;

		GTransform Transform;

	private:
		GMatrix AccumulatedMatrix;

		friend class GScene;
	};

	struct GUARDIAN_API GCameraComponent
	{
	public:
		GCameraComponent()
		{
			this->Camera = std::make_shared<GCamera>();
		}
		GCameraComponent(std::shared_ptr<GCamera> camera)
		{
			this->Camera = camera;
		}
		GCameraComponent(const GCameraComponent&) = default;

		std::shared_ptr<GCamera> Camera;
	};

	struct GUARDIAN_API GPointLightComponent
	{
	public:
		GPointLightComponent()
		{
			this->PointLight = std::make_shared<GPointLight>();
		}
		GPointLightComponent(std::shared_ptr<GPointLight> pointLight)
		{
			this->PointLight = pointLight;
		}
		GPointLightComponent(const GPointLightComponent&) = default;

		std::shared_ptr<GPointLight> PointLight;
	};

	struct GUARDIAN_API GModelComponent
	{
	public:
		GModelComponent()
		{
			this->Model = null;
		}
		GModelComponent(const std::string& modelAssetName)
		{
			this->Model = null;
			this->ModelAssetName = modelAssetName;
		}
		GModelComponent(const GModelComponent&) = default;

		std::string ModelAssetName;

	private:
		std::shared_ptr<GModel> Model;

		friend class GScene;
	};
}

#endif