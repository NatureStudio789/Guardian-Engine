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
		
		D3D12_CPU_DESCRIPTOR_HANDLE GetDepthStencilView();
		std::shared_ptr<GDescriptorHeap> GetDepthStencilDescriptorHeap();
		WRL::ComPtr<ID3D12Resource> GetDepthStencilBuffer();

		static std::shared_ptr<GDepthStencil> CreateNewDepthStencil(
			std::shared_ptr<GGraphicsContext> graphicsContext)
		{
			return std::make_shared<GDepthStencil>(graphicsContext);
		}

	private:
		std::shared_ptr<GDescriptorHeap> DepthStencilDescriptorHeap;
		WRL::ComPtr<ID3D12Resource> DepthStencilBuffer;
	};
}

#endif