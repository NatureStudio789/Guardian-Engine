#ifndef _GE_GTEXTURE_H_
#define _GE_GTEXTURE_H_
#include "../Sampler/GSampler.h"

namespace GE
{
	class GUARDIAN_API GTexture : public GApplicable
	{
	public:
		GTexture();
		GTexture(std::shared_ptr<GRootSignature> rootSignature, const GSurface& surface, int index = 0);
		GTexture(const GTexture& other);
		~GTexture();

		void InitializeTexture(std::shared_ptr<GRootSignature> rootSignature, const GSurface& surface, int index = 0);

		void Apply() override;

		WRL::ComPtr<ID3D12Resource> GetTextureResource();
		WRL::ComPtr<ID3D12Resource> GetTextureUploadHeap();
		CD3DX12_GPU_DESCRIPTOR_HANDLE GetShaderResourceView();

		static std::shared_ptr<GTexture> CreateNewTexture(std::shared_ptr<GRootSignature> rootSignature, const GSurface& surface, int index = 0)
		{
			return std::make_shared<GTexture>(rootSignature, surface, index);
		}

	private:
		CD3DX12_CPU_DESCRIPTOR_HANDLE GetShaderResourceCPUView();
		CD3DX12_GPU_DESCRIPTOR_HANDLE GetShaderResourceGPUView();

		std::shared_ptr<GRootSignature> TextureRootSignature;
		WRL::ComPtr<ID3D12Resource> TextureResource;
		WRL::ComPtr<ID3D12Resource> TextureUploadHeap;
		UINT TextureIndex;
		UINT TextureRootParameterIndex;
	};
}

#endif