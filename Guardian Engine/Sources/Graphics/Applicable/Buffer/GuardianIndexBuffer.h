#ifndef _GE_GUARDIANINDEXBUFFER_H_
#define _GE_GUARDIANINDEXBUFFER_H_
#include "GuardianVertexBuffer.h"

namespace GE
{
	class GUARDIAN_API GuardianIndexBuffer : public GuardianApplicable
	{
	public:
		GuardianIndexBuffer();
		GuardianIndexBuffer(const GuardianIndexBuffer& other);
		GuardianIndexBuffer(std::vector<UINT> indices);
		~GuardianIndexBuffer() override;

		void InitializeIndexBuffer(std::vector<UINT> indices);

		void Apply() override;

		WRL::ComPtr<ID3D11Buffer> GetIndexBufferObject() noexcept;
		const std::vector<UINT>& GetIndexBufferData() const noexcept;

		static std::shared_ptr<GuardianIndexBuffer> CreateNewIndexBuffer(std::vector<UINT> indices);

	private:
		WRL::ComPtr<ID3D11Buffer> IndexBufferObject;
		std::vector<UINT> IndexBufferData;
	};
}

#endif