#ifndef _GE_GDEPTHCUBEMAP_H_
#define _GE_GDEPTHCUBEMAP_H_
#include "GDepthMap.h"

namespace GE
{
	class GUARDIAN_API GDepthCubeMap : public GShaderView
	{
	public:
		GDepthCubeMap();
		GDepthCubeMap(UINT edgeLength);
		GDepthCubeMap(const GDepthCubeMap& other);
		~GDepthCubeMap();

		void InitializeDepthCubeMap(UINT edgeLength);

		void ResetDescriptor(CD3DX12_CPU_DESCRIPTOR_HANDLE cpuHandle, CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle) override;

		void ApplyDepthCubeMap(UINT faceIndex);
		void ClearDepthCubeMap(UINT faceIndex);

		void BeginRendering();
		void EndUpRendering();

		const UINT& GetDepthCubeMapFaceLength() const noexcept;

		WRL::ComPtr<ID3D12Resource> GetDepthCubeMapBuffer();
		std::shared_ptr<GDescriptorHandle> GetDepthCubeMapDSVDescriptorHandle(UINT faceIndex);

	private:
		UINT CubeMapFaceLength;

		WRL::ComPtr<ID3D12Resource> DepthCubeMapBuffer;
		std::vector<std::shared_ptr<GDescriptorHandle>> DSVDescriptorHandleList;

		std::shared_ptr<GViewport> CubeMapFaceViewport;
	};
}

#endif