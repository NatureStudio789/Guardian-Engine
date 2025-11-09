#include "GShaderView.h"
#include "../GGraphicsContextRegistry.h"

namespace GE
{
	GShaderView::GShaderView()
	{
		this->ShaderViewId = GUUID();
		this->ShaderViewName = "";
		
		this->ViewRootParameterIndex = 0;
		this->ViewDescriptorHandle = std::make_shared<GDescriptorHandle>();
		this->IsDescriptorAllocated = false;
	}

	GShaderView::GShaderView(std::string viewName, UINT rootParameterIndex, UINT descriptorCount)
	{
		this->ShaderViewId = GUUID();
		this->SetShaderViewName(viewName);

		this->SetRootParameterIndex(rootParameterIndex);
		this->AllocateDescriptor(descriptorCount);
	}

	GShaderView::GShaderView(const GShaderView& other)
	{
		this->ShaderViewId = other.ShaderViewId;
		this->ShaderViewName = other.ShaderViewName;

		this->ViewRootParameterIndex = other.ViewRootParameterIndex;
		this->ViewDescriptorHandle = other.ViewDescriptorHandle;
		this->IsDescriptorAllocated = other.IsDescriptorAllocated;
	}

	GShaderView::~GShaderView()
	{
		this->ShaderViewId = 0;
		this->ShaderViewName.clear();
		
		this->ViewRootParameterIndex = 0;
		this->ViewDescriptorHandle.reset();
		this->IsDescriptorAllocated = false;
	}

	void GShaderView::AllocateDescriptor(UINT descriptorCount)
	{
		this->ViewDescriptorHandle = GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetSRVDescriptorHeap()->Allocate(descriptorCount);
		this->IsDescriptorAllocated = true;
	}

	void GShaderView::ResetDescriptor(CD3DX12_CPU_DESCRIPTOR_HANDLE cpuHandle, CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle)
	{
		this->ViewDescriptorHandle->CPUHandle = cpuHandle;
		this->ViewDescriptorHandle->GPUHandle = gpuHandle;
	}

	void GShaderView::SetShaderViewName(std::string name)
	{
		this->ShaderViewName = name;
	}

	void GShaderView::SetRootParameterIndex(UINT index)
	{
		this->ViewRootParameterIndex = index;
	}

	void GShaderView::ApplyShaderView()
	{
		GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsCommandList()->GetCommandListObject()->
			SetGraphicsRootDescriptorTable(this->ViewRootParameterIndex, this->ViewDescriptorHandle->GPUHandle);
	}

	const GUUID& GShaderView::GetShaderViewId() const noexcept
	{
		return this->ShaderViewId;
	}

	const std::string& GShaderView::GetShaderViewName() const noexcept
	{
		return this->ShaderViewName;
	}

	const UINT& GShaderView::GetViewRootParameterIndex() const noexcept
	{
		return this->ViewRootParameterIndex;
	}

	std::shared_ptr<GDescriptorHandle> GShaderView::GetViewDescriptorHandle()
	{
		return this->ViewDescriptorHandle;
	}

	const bool& GShaderView::GetDescriptorAllocated() const noexcept
	{
		return this->IsDescriptorAllocated;
	}
}
