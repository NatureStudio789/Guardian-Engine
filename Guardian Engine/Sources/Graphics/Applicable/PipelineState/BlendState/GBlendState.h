#ifndef _GE_GBLENDSTATE_H_
#define _GE_GBLENDSTATE_H_
#include "../RasterizerState/GRasterizerState.h"

namespace GE
{
	class GUARDIAN_API GBlendState
	{
	public:
		GBlendState() = default;
		~GBlendState() = default;

		static std::shared_ptr<GBlendState> CreateNewBlendState()
		{
			return std::make_shared<GBlendState>();
		}
	};
}

#endif