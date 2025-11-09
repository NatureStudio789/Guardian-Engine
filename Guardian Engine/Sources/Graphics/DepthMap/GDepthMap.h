#ifndef _GE_GDEPTHMAP_H_
#define _GE_GDEPTHMAP_H_
#include "../GGraphicsContextRegistry.h"

namespace GE
{
	class GUARDIAN_API GDepthMap : public GShaderView
	{
	public:
		GDepthMap();
		GDepthMap(UINT width, UINT height);
		GDepthMap(const GDepthMap& other);
		~GDepthMap() override;

		void InitializeDepthMap(UINT width, UINT height);

		void ResetDescriptor(CD3DX12_CPU_DESCRIPTOR_HANDLE cpuHandle, CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle) override;

		void ApplyDepthMap();
		void ClearDepthMap();

		void BeginRendering();
		void EndUpRendering();

		void ResizeDepthMap(UINT newWidth, UINT newHeight);

		const UINT& GetDepthMapWidth() const noexcept;
		const UINT& GetDepthMapHeight() const noexcept;

		WRL::ComPtr<ID3D12Resource> GetDepthMapBuffer();
		std::shared_ptr<GDescriptorHandle> GetDepthMapDSVDescriptorHandle();

	private:
		UINT MapWidth;
		UINT MapHeight;

		WRL::ComPtr<ID3D12Resource> DepthMapBuffer;
		std::shared_ptr<GDescriptorHandle> DSVDescriptorHandle;

		std::shared_ptr<GViewport> DepthMapViewport;
	};
}

#endif