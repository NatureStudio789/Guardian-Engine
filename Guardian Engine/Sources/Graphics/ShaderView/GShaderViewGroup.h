#ifndef _GE_GSHADERVIEWGROUP_H_
#define _GE_GSHADERVIEWGROUP_H_
#include "GShaderView.h"

namespace GE
{
	class GUARDIAN_API GShaderViewGroup
	{
	public:
		GShaderViewGroup();
		GShaderViewGroup(const std::string& name, UINT size);
		GShaderViewGroup(const GShaderViewGroup& other);
		~GShaderViewGroup();

		void InitializeGroup(const std::string& name, UINT size);
		void AllocateDescriptor(UINT descriptorSize);
		void AddGroupMember(UINT index, std::shared_ptr<GShaderView> shaderViewMember);

		void SetRootParameterIndex(UINT index);

		void ApplyShaderViewGroup();

		const GUUID& GetShaderViewGroupId() const noexcept;
		const std::string& GetShaderViewGroupName() const noexcept;
		const UINT& GetGroupViewSize() const noexcept;
		std::shared_ptr<GDescriptorHandle> GetGroupHeadViewDescriptorHandle();
		const UINT& GetGroupViewRootParameterIndex() const noexcept;
		const bool& GetDescriptorAllocated() const noexcept;

	protected:
		GUUID ShaderViewGroupId;
		std::string ShaderViewGroupName;

		UINT GroupViewSize;
		std::shared_ptr<GDescriptorHandle> GroupHeadViewDescriptorHandle;
		std::map<UINT, std::string> GroupMemberRegistry;
		UINT GroupViewRootParameterIndex;

		bool IsDescriptorAllocated;
	};
}

#endif