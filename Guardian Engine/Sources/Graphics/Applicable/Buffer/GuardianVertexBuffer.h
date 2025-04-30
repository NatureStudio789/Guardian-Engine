#ifndef _GE_GUARDIANVERTEXBUFFER_H_
#define _GE_GUARDIANVERTEXBUFFER_H_
#include "../RasterizerState/GuardianRasterizerState.h"

namespace GE
{
	class GUARDIAN_API GuardianVertexBuffer : public GuardianApplicable
	{
	public:
		GuardianVertexBuffer();
		GuardianVertexBuffer(const GuardianVertexBuffer& other);
		GuardianVertexBuffer(void* vertices, UINT dataSize, UINT dataTypeStride);
		~GuardianVertexBuffer() override;

		void InitializeVertexBuffer(void* vertices, UINT dataSize, UINT dataTypeStride);

		void UpdateVertices(void* vertices);
		void Apply() override;

		const void* GetVertexBufferData() const noexcept;
		const UINT GetVertexBufferDataSize() const noexcept;
		WRL::ComPtr<ID3D11Buffer> GetVertexBufferObject() noexcept;

		static std::shared_ptr<GuardianVertexBuffer> CreateNewVertexBuffer(
			void* vertices, UINT dataSize, UINT dataTypeStride);

	private:
		void* VertexBufferData;
		UINT BufferDataSize;
		UINT BufferDataTypeStride;
		WRL::ComPtr<ID3D11Buffer> VertexBufferObject;
	};
}

#endif