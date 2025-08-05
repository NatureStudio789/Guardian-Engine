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

		std::shared_ptr<GDescriptorHandle> GetTextureDescriptorHandle();
		WRL::ComPtr<ID3D12Resource> GetTextureResource();
		WRL::ComPtr<ID3D12Resource> GetTextureUploadHeap();

		static std::shared_ptr<GTexture> CreateNewTexture(std::shared_ptr<GRootSignature> rootSignature, const GSurface& surface, int index = 0)
		{
			return std::make_shared<GTexture>(rootSignature, surface, index);
		}

	private:
		std::shared_ptr<GDescriptorHandle> TextureDescriptorHandle;
		std::shared_ptr<GRootSignature> TextureRootSignature;
		WRL::ComPtr<ID3D12Resource> TextureResource;
		WRL::ComPtr<ID3D12Resource> TextureUploadHeap;
		UINT TextureRootParameterIndex;
	};
}

#endif