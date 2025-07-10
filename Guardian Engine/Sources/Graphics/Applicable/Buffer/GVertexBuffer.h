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

	private:
	};
}

#endif