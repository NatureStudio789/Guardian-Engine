#ifndef _GE_GPASS_H_
#define _GE_GPASS_H_
#include "../../Renderable/GRenderable.h"

namespace GE
{
	class GUARDIAN_API GPass
	{
	public:
		GPass();
		GPass(const std::string& name);
		GPass(const GPass& other);
		~GPass();

		virtual void Execute();
		virtual void Release();

		const std::string& GetPassName() const noexcept;

	private:
		std::string PassName;
	};
}

#endif