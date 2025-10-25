#ifndef _GE_GSHADERVIEW_H_
#define _GE_GSHADERVIEW_H_
#include "../Surface/GSurface.h"

namespace GE
{
	class GUARDIAN_API GShaderView
	{
	public:
		GShaderView();
		GShaderView(std::string viewName,
			UINT rootParameterIndex, UINT descriptorCount = 1);
		GShaderView(const GShaderView& other);
		virtual ~GShaderView();

		void AllocateDescriptor(UINT descriptorCount = 1);

		void SetShaderViewName(std::string name);
		void SetRootParameterIndex(UINT index);

		void ApplyShaderView();

		const GUUID& GetShaderViewId() const noexcept;
		const std::string& GetShaderViewName() const noexcept;
		const UINT& GetViewRootParameterIndex() const noexcept;
		std::shared_ptr<GDescriptorHandle> GetViewDescriptorHandle();

	protected:
		GUUID ShaderViewId;
		std::string ShaderViewName;
		
		UINT ViewRootParameterIndex;
		std::shared_ptr<GDescriptorHandle> ViewDescriptorHandle;
	};
}

#endif