#ifndef _GE_GUARDIANRENDERABLE_H_
#define _GE_GUARDIANRENDERABLE_H_
#include "../MaterialSystem/GuardianMaterialSystem.h"

namespace guardian 
{
	enum GUARDIAN_API GuardianSubmitPassLevel
	{
		GE_SUBMIT_DEFAULT3D = 0,
		GE_SUBMIT_TRANSPARENT3D = 1,
		GE_SUBMIT_DEFAULT2D = 2,
		GE_SUBMIT_TRANSPARENT2D = 3,
		GE_SUBMIT_GUI2D = 4,
		GE_SUBMIT_SPECIALLY = 5,
	};

	class GUARDIAN_API GuardianRenderable
	{
	public:
		GuardianRenderable();
		GuardianRenderable(const GuardianRenderable& other);
		virtual ~GuardianRenderable();

		void AddApplicable(std::shared_ptr<GuardianApplicable> applicable);
		void AddVertexBuffer(std::shared_ptr<GuardianVertexBuffer> vertexBuffer);
		void AddIndexBuffer(std::shared_ptr<GuardianIndexBuffer> indexBuffer);
		void AddTransformConstantBuffer(std::shared_ptr<GuardianTransformConstantBuffer> transformCBuffer);
		void AddLightConstantBuffer(std::shared_ptr<GuardianLightConstantBuffer> lightCBuffer);

		void AddLightConstantBufferFromStatic();

		void Render(std::shared_ptr<GuardianGraphics> graphics);
		virtual void Update() {}

		const GuardianUUID& GetRenderableId() const noexcept;
		std::shared_ptr<GuardianLightConstantBuffer> GetLightConstantBuffer();
		std::shared_ptr<GuardianTransformConstantBuffer> GetTransformConstantBuffer();
		std::shared_ptr<GuardianVertexBuffer> GetVertexBuffer();
		std::shared_ptr<GuardianIndexBuffer> GetIndexBuffer();
		const UINT& GetTexturesNumber() const noexcept;

	protected:
		GuardianUUID RenderableId;
		std::shared_ptr<GuardianVertexBuffer> RenderingVertexBuffer;
		std::shared_ptr<GuardianIndexBuffer> RenderingIndexBuffer;
		std::shared_ptr<GuardianTransformConstantBuffer> RenderingTransformConstantBuffer;
		std::shared_ptr<GuardianLightConstantBuffer> RenderingLightConstantBuffer;
		std::shared_ptr<GuardianMaterial> RenderingMaterial;
		UINT RenderingTexturesNumber;

		std::vector<std::shared_ptr<GuardianApplicable>> ApplicableList;

	private:
		virtual std::vector<std::shared_ptr<GuardianApplicable>> GetStaticApplicableList() noexcept
		{
			return std::vector<std::shared_ptr<GuardianApplicable>>();
		}

		friend class GuardianRenderer;
		template<typename T>
		friend class GuardianRenderableBase;
	};
}

#endif