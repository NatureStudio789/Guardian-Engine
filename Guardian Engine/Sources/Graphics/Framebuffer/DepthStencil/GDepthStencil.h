#ifndef _GE_GDEPTHSTENCIL_H_
#define _GE_GDEPTHSTENCIL_H_
#include "../RenderTarget/GRenderTarget.h"

namespace GE
{
	class GUARDIAN_API GDepthStencil
	{
	public:
		GDepthStencil();
		GDepthStencil(std::shared_ptr<GGraphicsContext> graphicsContext);
		GDepthStencil(const GDepthStencil& other);
		~GDepthStencil();

		void InitializeDepthStencil(std::shared_ptr<GGraphicsContext> graphicsContext);

		void ResizeDepthStencilView(std::shared_ptr<GGraphicsContext> graphicsContext);
		void ClearDepthStencil(std::shared_ptr<GGraphicsContext> graphicsContext);
		
		WRL::ComPtr<ID3D12Resource> GetDepthStencilBuffer();
		std::shared_ptr<GDescriptorHandle> GetDSVDescriptorHandle();

		static std::shared_ptr<GDepthStencil> CreateNewDepthStencil(
			std::shared_ptr<GGraphicsContext> graphicsContext)
		{
			return std::make_shared<GDepthStencil>(graphicsContext);
		}

	private:
		std::shared_ptr<GDescriptorHandle> DSVDescriptorHandle;
		WRL::ComPtr<ID3D12Resource> DepthStencilBuffer;
	};
}

#endif