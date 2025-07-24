#ifndef _GE_GBUFFER_H_
#define _GE_GBUFFER_H_
#include "../GApplicable.h"

namespace GE
{
	class GUARDIAN_API GBuffer : public GApplicable
	{
	public:
		GBuffer();
		GBuffer(void* bufferData, UINT dataSize, UINT dataStride);
		GBuffer(const GBuffer& other);
		~GBuffer() override;

		void InitializeBuffer(void* bufferData, UINT dataSize, UINT dataStride);

		WRL::ComPtr<ID3DBlob> GetCPUBuffer();
		WRL::ComPtr<ID3D12Resource> GetGPUBuffer();
		WRL::ComPtr<ID3D12Resource> GetUploadBuffer();
		const void* GetBufferData() const noexcept;
		const UINT& GetDataSize() const noexcept;
		const UINT& GetDataStride() const noexcept;

	protected:
		WRL::ComPtr<ID3DBlob> CPUBuffer;
		WRL::ComPtr<ID3D12Resource> GPUBuffer;
		WRL::ComPtr<ID3D12Resource> UploadBuffer;

		void* BufferData;
		UINT DataSize;
		UINT DataStride;
	};
}

#endif