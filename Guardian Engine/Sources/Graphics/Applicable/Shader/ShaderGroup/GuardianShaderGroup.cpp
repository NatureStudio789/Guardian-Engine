#include "GuardianShaderGroup.h"

namespace GE
{
	GuardianShaderGroup::GuardianShaderGroup()
	{
		this->GroupId = GuardianUUID();
		this->GroupName = "Unnamed";

		this->GroupVertexShader = null;
		this->GroupInputLayout = null;
		this->GroupPixelShader = null;
	}

	GuardianShaderGroup::GuardianShaderGroup(const GString& name, std::shared_ptr<GuardianVertexShader> vs,
		D3D11_INPUT_ELEMENT_DESC layoutData[], int dataSize, std::shared_ptr<GuardianPixelShader> ps)
	{
		this->InitializeShaderGroup(name, vs, layoutData, dataSize, ps);
	}

	GuardianShaderGroup::GuardianShaderGroup(const GuardianShaderGroup& other)
	{
		this->GroupId = other.GroupId;
		this->GroupName = other.GroupName;
		this->GroupVertexShader = other.GroupVertexShader;
		this->GroupInputLayout = other.GroupInputLayout;
		this->GroupPixelShader;
	}

	GuardianShaderGroup::~GuardianShaderGroup()
	{
		this->GroupId = 0;
		this->GroupName = "";
		this->GroupVertexShader = null;
		this->GroupPixelShader = null;
		this->GroupInputLayout = null;
	}

	void GuardianShaderGroup::InitializeShaderGroup(const GString& name, std::shared_ptr<GuardianVertexShader> vs,
		D3D11_INPUT_ELEMENT_DESC layoutData[], int dataSize, std::shared_ptr<GuardianPixelShader> ps)
	{
		this->GroupId = GuardianUUID();
		this->GroupName = name;

		this->GroupVertexShader = vs;
		this->GroupInputLayout = GuardianInputLayout::CreateNewInputLayout(this->GroupVertexShader,
			layoutData, dataSize);

		this->GroupPixelShader = ps;
	}

	void GuardianShaderGroup::SetGroupName(const GString& name)
	{
		this->GroupName = name;
	}

	void GuardianShaderGroup::SetVertexShader(std::shared_ptr<GuardianVertexShader> shader,
		D3D11_INPUT_ELEMENT_DESC layoutData[], int dataSize)
	{
		this->GroupVertexShader = shader;
		this->GroupInputLayout = GuardianInputLayout::CreateNewInputLayout(this->GroupVertexShader,
			layoutData, dataSize);
	}

	void GuardianShaderGroup::SetPixelShader(std::shared_ptr<GuardianPixelShader> shader)
	{
		this->GroupPixelShader = shader;
	}

	void GuardianShaderGroup::Apply()
	{
		this->GroupVertexShader->Apply();
		this->GroupInputLayout->Apply();

		this->GroupPixelShader->Apply();
	}

	const GuardianUUID& GuardianShaderGroup::GetGroupId() const noexcept
	{
		return this->GroupId;
	}

	const GString& GuardianShaderGroup::GetGroupName() const noexcept
	{
		return this->GroupName;
	}
}
