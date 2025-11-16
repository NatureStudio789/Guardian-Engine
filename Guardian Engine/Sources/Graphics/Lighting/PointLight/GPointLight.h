#ifndef _GE_GPOINTLIGHT_H_
#define _GE_GPOINTLIGHT_H_
#include "../../Math/Ray/GRay.h"

namespace GE
{
	class GUARDIAN_API GScene;
	class GUARDIAN_API GDepthCubeMap;
	class GUARDIAN_API GCameraCBuffer;

	struct GUARDIAN_API GPointLight
	{
	public:
		struct Data
		{
		public:
			Data()
			{
				this->Position = {};
				this->Strength = 100.0f;
				this->Color = { 1.0f };
				this->padding = 114514.0f;
			}

			Data(const Data& other)
			{
				this->Position = other.Position;
				this->Strength = other.Strength;
				this->Color = other.Color;
			}

			Data(const GVector3& position, const GVector3& color, float strength)
			{
				this->Position = position;
				this->Color = color;
				this->Strength = strength;
				this->padding = 114514.0f;
			}

			GVector3 Position;
			float Strength;
			GVector3 Color;
		private:
			float padding;
		};

	public:
		GPointLight();
		GPointLight(const GPointLight& other);
		GPointLight(const GVector3& position,
			const GVector3& color, float strength);

		void Register(GScene* scene);
		void Unregister(GScene* scene);

		const GUUID& GetPointLightId() const noexcept;
		const bool& GetLightRegistered() const noexcept;

		Data LightData;

	private:
		void InitializeDepthRendering();
		void UpdateDepthRendering(UINT faceIndex);

		GUUID PointLightId;
		bool IsLightRegistered;
		
		std::shared_ptr<GDepthCubeMap> LightDepthCubeMap;
		std::shared_ptr<GCameraCBuffer> LightCameraCBuffer;

		friend class GLightRegistry;
		friend class GDepthPass;
	};
}

#endif