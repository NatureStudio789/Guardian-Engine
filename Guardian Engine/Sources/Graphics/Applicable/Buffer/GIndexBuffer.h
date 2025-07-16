#ifndef _GE_GINDEXBUFFER_H_
#define _GE_GINDEXBUFFER_H_
#include "GVertexBuffer.h"

namespace GE
{
	class GUARDIAN_API GIndexBuffer : public GBuffer
	{
	public:
		GIndexBuffer();
		GIndexBuffer(UINT* indicesData, UINT dataSize);
		GIndexBuffer(const GIndexBuffer& other);

		void Apply() override;

		static std::shared_ptr<GIndexBuffer> CreateNewIndexBuffer(
			UINT* indicesData, UINT dataSize)
		{
			return std::make_shared<GIndexBuffer>(indicesData, dataSize);
		}

	private:
	};
}

#endif