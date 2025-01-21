#ifndef _GE_GUARDIANRENDERABLE_H_
#define _GE_GUARDIANRENDERABLE_H_
#include "../Applicable/Sampler/GuardianSampler.h"

namespace guardian 
{
	class GUARDIAN_API GuardianRenderer;

	enum GUARDIAN_API GuardianSubmitPassLevel
	{
		GE_SUBMIT_DEFAULT3D = 0,
		GE_SUBMIT_TRANSPARENT3D = 1,
		GE_SUBMIT_DEFAULT2D = 2,
		GE_SUBMIT_TRANSPARENT2D = 3,
		GE_SUBMIT_GUI2D = 4,
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

		void SubmitToRenderer(GuardianSubmitPassLevel level);

		void Render(std::shared_ptr<GuardianGraphics> graphics);
		virtual void Update() {}

	protected:
		std::shared_ptr<GuardianVertexBuffer> RenderingVertexBuffer;
		std::shared_ptr<GuardianIndexBuffer> RenderingIndexBuffer;
		std::shared_ptr<GuardianTransformConstantBuffer> RenderingTransformConstantBuffer;;

		std::vector<std::shared_ptr<GuardianApplicable>> ApplicableList;

		friend class GuardianRenderer;
	};
}

#endif