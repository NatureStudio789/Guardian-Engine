#ifndef _GE_GRENDERTARET_H_
#define _GE_GRENDERTARET_H_
#include "../../ShaderView/GShaderViewRegistry.h"

namespace GE
{
	class GUARDIAN_API GRenderTarget
	{
	public:
		GRenderTarget();
		GRenderTarget(std::shared_ptr<GGraphicsContext> graphicsContext, bool enableRTT = false);
		GRenderTarget(const GRenderTarget& other);
		~GRenderTarget();

		void InitializeRenderTarget(std::shared_ptr<GGraphicsContext> graphicsContext, bool enableRTT = false);

		void ResizeRenderTargetView(
			std::shared_ptr<GGraphicsContext> graphicsContext, int newWidth, int newHeight);
		void ClearRenderTarget(std::shared_ptr<GGraphicsContext> graphicsContext);
		void ApplyRenderTarget(std::shared_ptr<GGraphicsContext> graphicsContext,
			D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView);

		WRL::ComPtr<ID3D12Resource> GetRTTBuffer();
		D3D12_CPU_DESCRIPTOR_HANDLE GetRenderTargetView(std::shared_ptr<GGraphicsContext> graphicsContext);

		static std::shared_ptr<GRenderTarget> CreateNewRenderTarget(
			std::shared_ptr<GGraphicsContext> graphicsContext, bool enableRTT = false)
		{
			return std::make_shared<GRenderTarget>(graphicsContext, enableRTT);
		}

	private:
		bool EnableRTT;
		WRL::ComPtr<ID3D12Resource> RTTBuffer;

		std::shared_ptr<GDescriptorHandle> RTVDescriptorHandle;
	};
}

#endif