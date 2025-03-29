#ifndef _GE_GUARDIANRENDERTARGET_H_
#define _GE_GUARDIANRENDERTARGET_H_
#include "../../Physics/RigidBody/GuardianDynamicRigidBody.h"

namespace GE
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
		GuardianRenderTarget(std::shared_ptr<GuardianGraphics> graphics, int width, int height);
		~GuardianRenderTarget() = default;

		void InitializeRenderTarget(std::shared_ptr<GuardianGraphics> graphics);
		void InitializeRenderTarget(std::shared_ptr<GuardianGraphics> graphics, int width, int height);

		void ApplyRenderTarget(std::shared_ptr<GuardianGraphics> graphics, 
			std::shared_ptr<GuardianDepthStencil> depthStencil, const GVector3& clearColor);

		WRL::ComPtr<ID3D11RenderTargetView> GetRenderTargetView() noexcept;
		WRL::ComPtr<ID3D11ShaderResourceView> GetRenderTargetShaderResource() noexcept;
		WRL::ComPtr<ID3D11Texture2D> GetRenderTargetTexture() noexcept;

		GuardianSurface ConvertToSurface(std::shared_ptr<GuardianGraphics> graphics);

		static std::shared_ptr<GuardianRenderTarget> CreateNewRenderTarget(
			std::shared_ptr<GuardianGraphics> graphics);
		static std::shared_ptr<GuardianRenderTarget> CreateNewRenderTarget(
			std::shared_ptr<GuardianGraphics> graphics, int width, int height);

	private:
		WRL::ComPtr<ID3D11RenderTargetView> RenderTargetView;
		int RenderTargetWidth;
		int RenderTargetHeight;
		WRL::ComPtr<ID3D11Texture2D> RenderTargetTexture;
		WRL::ComPtr<ID3D11ShaderResourceView> RenderTargetShaderResource;
	};
}

#endif