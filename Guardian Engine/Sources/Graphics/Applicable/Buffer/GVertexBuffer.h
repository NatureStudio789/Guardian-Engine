#ifndef _GE_GVERTEXBUFFER_H_
#define _GE_GVERTEXBUFFER_H_
#include "GBuffer.h"

namespace GE
{
	class GUARDIAN_API GVertexBuffer : public GBuffer
	{
	public:
		GVertexBuffer();
		GVertexBuffer(void* verticesData, UINT dataSize, UINT dataStride);
		GVertexBuffer(const GVertexBuffer& other);

		void Apply() override;

		const UINT& GetVerticesCount() const noexcept;

		static std::shared_ptr<GVertexBuffer> CreateNewVertexBuffer(
			void* verticesData, UINT dataSize, UINT dataStride)
		{
			return std::make_shared<GVertexBuffer>(verticesData, dataSize, dataStride);
		}

	private:
	};
}

#endif