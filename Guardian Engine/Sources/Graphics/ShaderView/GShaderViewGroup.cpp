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

	void GShaderViewGroup::AddGroupMember(GShaderView* shaderViewMember)
	{
		this->GroupMemberRegistry.push_back(shaderViewMember);

		UINT index = (UINT)(this->GroupMemberRegistry.size() - 1);

		shaderViewMember->GetViewDescriptorHandle()->CPUHandle = this->GroupHeadViewDescriptorHandle->CPUHandle;
		shaderViewMember->GetViewDescriptorHandle()->CPUHandle.Offset(index,
			GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetSRVDescriptorHeap()->GetDescriptorIncrementSize());
		shaderViewMember->GetViewDescriptorHandle()->GPUHandle = this->GroupHeadViewDescriptorHandle->GPUHandle;
		shaderViewMember->GetViewDescriptorHandle()->GPUHandle.Offset(index,
			GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetSRVDescriptorHeap()->GetDescriptorIncrementSize());

		shaderViewMember->IsDescriptorAllocated = true;
	}

	void GShaderViewGroup::RemoveGroupMember(GShaderView* shaderViewMember)
	{
		int index = -1;
		for (int i = 0; i < (int)this->GroupMemberRegistry.size(); i++)
		{
			if (this->GroupMemberRegistry[i]->GetShaderViewId() == shaderViewMember->GetShaderViewId())
			{
				index = i;
				break;
			}
		}

		if (index < 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION("Can't the right shader view member to remove!");
		}

		this->GroupMemberRegistry.erase(this->GroupMemberRegistry.begin() + index);

		for (UINT i = (UINT)index; i < (UINT)this->GroupMemberRegistry.size(); i++)
		{
			auto& CpuHandle = this->GroupHeadViewDescriptorHandle->CPUHandle;
			CpuHandle.Offset(i, GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetSRVDescriptorHeap()->GetDescriptorIncrementSize());
			auto& GpuHandle = this->GroupHeadViewDescriptorHandle->GPUHandle;
			GpuHandle.Offset(i, GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetSRVDescriptorHeap()->GetDescriptorIncrementSize());
			this->GroupMemberRegistry[i]->ResetDescriptor(CpuHandle, GpuHandle);
		}
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
