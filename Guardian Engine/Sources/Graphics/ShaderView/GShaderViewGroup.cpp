#include "GShaderViewGroup.h"
#include "../GGraphicsContextRegistry.h"

namespace GE
{
	GShaderViewGroup::GShaderViewGroup()
	{
		this->ShaderViewGroupId = 0;
		this->ShaderViewGroupName = "";

		this->GroupViewSize = 0;
		this->GroupHeadViewDescriptorHandle = null;

		this->GroupViewRootParameterIndex = 0;

		this->IsDescriptorAllocated = false;
	}

	GShaderViewGroup::GShaderViewGroup(const std::string& name, UINT size)
	{
		this->InitializeGroup(name, size);
	}

	GShaderViewGroup::GShaderViewGroup(const GShaderViewGroup& other)
	{
		this->ShaderViewGroupId = other.ShaderViewGroupId;
		this->ShaderViewGroupName = other.ShaderViewGroupName;

		this->GroupViewSize = other.GroupViewSize;
		this->GroupHeadViewDescriptorHandle = other.GroupHeadViewDescriptorHandle;
		this->GroupMemberRegistry = other.GroupMemberRegistry;

		this->GroupViewRootParameterIndex = other.GroupViewRootParameterIndex;

		this->IsDescriptorAllocated = other.IsDescriptorAllocated;
	}

	GShaderViewGroup::~GShaderViewGroup() 
	{
		this->ShaderViewGroupId = 0;
		this->ShaderViewGroupName.clear();

		this->GroupViewSize = 0;
		this->GroupHeadViewDescriptorHandle.reset();
		GroupMemberRegistry.clear();

		this->GroupViewRootParameterIndex = 0;
		
		this->IsDescriptorAllocated = false;
	}

	void GShaderViewGroup::InitializeGroup(const std::string& name, UINT size)
	{
		this->ShaderViewGroupId = GUUID();
		this->ShaderViewGroupName = name;

		this->AllocateDescriptor(size);
	}

	void GShaderViewGroup::AllocateDescriptor(UINT descriptorSize)
	{
		this->GroupViewSize = descriptorSize;

		this->GroupHeadViewDescriptorHandle = GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetSRVDescriptorHeap()->Allocate(this->GroupViewSize);
		this->IsDescriptorAllocated = true;
	}

	void GShaderViewGroup::AddGroupMember(UINT index, std::shared_ptr<GShaderView> shaderViewMember)
	{
		GUARDIAN_CHECK_POINTER(shaderViewMember);

		if (this->GroupMemberRegistry.count(index))
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("The group member at index of '{}' already exists in the group named '{}'",
				index, this->ShaderViewGroupName));
		}

		this->GroupMemberRegistry[index] = shaderViewMember->GetShaderViewName();
		shaderViewMember->GetViewDescriptorHandle()->CPUHandle = this->GroupHeadViewDescriptorHandle->CPUHandle;
		shaderViewMember->GetViewDescriptorHandle()->CPUHandle.Offset(index,
			GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetSRVDescriptorHeap()->GetDescriptorIncrementSize());
		shaderViewMember->GetViewDescriptorHandle()->GPUHandle = this->GroupHeadViewDescriptorHandle->GPUHandle;
		shaderViewMember->GetViewDescriptorHandle()->GPUHandle.Offset(index,
			GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetSRVDescriptorHeap()->GetDescriptorIncrementSize());
	}

	void GShaderViewGroup::SetRootParameterIndex(UINT index)
	{
		this->GroupViewRootParameterIndex = index;
	}

	void GShaderViewGroup::ApplyShaderViewGroup()
	{
		GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsCommandList()->GetCommandListObject()->
			SetGraphicsRootDescriptorTable(this->GroupViewRootParameterIndex, this->GroupHeadViewDescriptorHandle->GPUHandle);
	}

	const GUUID& GShaderViewGroup::GetShaderViewGroupId() const noexcept
	{
		return this->ShaderViewGroupId;
	}

	const std::string& GShaderViewGroup::GetShaderViewGroupName() const noexcept
	{
		return this->ShaderViewGroupName;
	}

	const UINT& GShaderViewGroup::GetGroupViewSize() const noexcept
	{
		return this->GroupViewSize;
	}

	std::shared_ptr<GDescriptorHandle> GShaderViewGroup::GetGroupHeadViewDescriptorHandle()
	{
		return this->GroupHeadViewDescriptorHandle;
	}

	const UINT& GShaderViewGroup::GetGroupViewRootParameterIndex() const noexcept
	{
		return this->GroupViewRootParameterIndex;
	}

	const bool& GShaderViewGroup::GetDescriptorAllocated() const noexcept
	{
		return this->IsDescriptorAllocated;
	}
}
