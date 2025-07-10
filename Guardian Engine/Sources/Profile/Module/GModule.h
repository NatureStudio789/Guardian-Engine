#ifndef _GE_GMODULE_H_
#define _GE_GMODULE_H_
#include "../../Aplication/Window/GWindow.h"

namespace GE
{
	class GUARDIAN_API GModule
	{
	public:
		GModule() = default;
		GModule(const GModule& other)
		{
			this->ModuleName = other.ModuleName;
			this->IsModuleLoaded = other.IsModuleLoaded;
		}
		virtual ~GModule() = default;

		virtual void InitializeModule() {}
		virtual void UpdateModule() {}
		virtual void ReleaseModule() {}

		const std::string& GetModuleName() const noexcept
		{
			return this->ModuleName;
		}
		const bool& GetModuleLoaded() const noexcept
		{
			return this->IsModuleLoaded;
		}

	protected:
		std::string ModuleName;
		bool IsModuleLoaded = false;

		friend class GModuleSystem;
	};
}

#endif