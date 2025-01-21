#ifndef _GE_GUARDIANRENDERTARGET_H_
#define _GE_GUARDIANRENDERTARGET_H_
#include "../Camera/GuardianCamera.h"

namespace guardian
{
	class GUARDIAN_API GuardianGraphics;
	class GUARDIAN_API GuardianSurface;
	class GUARDIAN_API GuardianDepthStencil;

	class GUARDIAN_API GuardianRenderTarget
	{
	public:
		GuardianRenderTarget() = default;
		GuardianRenderTarget(const GuardianRenderTarget&) = default;
		GuardianRenderTarget(std::shared_ptr<GuardianGraphics> graphics);
		~GuardianRenderTarget() = default;

		void InitializeRenderTarget(std::shared_ptr<GuardianGraphics> graphics);

		void ApplyRenderTarget(std::shared_ptr<GuardianGraphics> graphics, 
			std::shared_ptr<GuardianDepthStencil> depthStencil, const GVector3& clearColor);

		WRL::ComPtr<ID3D11RenderTargetView> GetRenderTargetView() noexcept;

		GuardianSurface ConvertToSurface(std::shared_ptr<GuardianGraphics> graphics);

		static std::shared_ptr<GuardianRenderTarget> CreateNewRenderTarget(std::shared_ptr<GuardianGraphics> graphics);

	private:
		WRL::ComPtr<ID3D11RenderTargetView> RenderTargetView;
		int RenderTargetWidth;
		int RenderTargetHeight;
	};
}

#endif