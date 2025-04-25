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

	GuardianShaderGroup::GuardianShaderGroup(std::shared_ptr<GuardianGraphics> graphics,
		const GString& name, std::shared_ptr<GuardianVertexShader> vs,
		D3D11_INPUT_ELEMENT_DESC layoutData[], int dataSize, std::shared_ptr<GuardianPixelShader> ps)
	{
		this->InitializeShaderGroup(graphics, name, vs, layoutData, dataSize, ps);
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

	void GuardianShaderGroup::InitializeShaderGroup(std::shared_ptr<GuardianGraphics> graphics, 
		const GString& name, std::shared_ptr<GuardianVertexShader> vs,
		D3D11_INPUT_ELEMENT_DESC layoutData[], int dataSize, std::shared_ptr<GuardianPixelShader> ps)
	{
		this->GroupId = GuardianUUID();
		this->GroupName = name;

		this->GroupVertexShader = vs;
		this->GroupInputLayout = GuardianInputLayout::CreateNewInputLayout(graphics, this->GroupVertexShader,
			layoutData, dataSize);

		this->GroupPixelShader = ps;
	}

	void GuardianShaderGroup::SetGroupName(const GString& name)
	{
		this->GroupName = name;
	}

	void GuardianShaderGroup::SetVertexShader(std::shared_ptr<GuardianVertexShader> shader,
		D3D11_INPUT_ELEMENT_DESC layoutData[], int dataSize, std::shared_ptr<GuardianGraphics> graphics)
	{
		this->GroupVertexShader = shader;
		this->GroupInputLayout = GuardianInputLayout::CreateNewInputLayout(graphics, this->GroupVertexShader,
			layoutData, dataSize);
	}

	void GuardianShaderGroup::SetPixelShader(std::shared_ptr<GuardianPixelShader> shader)
	{
		this->GroupPixelShader = shader;
	}

	void GuardianShaderGroup::Apply(std::shared_ptr<GuardianGraphics> graphics)
	{
		this->GroupVertexShader->Apply(graphics);
		this->GroupInputLayout->Apply(graphics);

		this->GroupPixelShader->Apply(graphics);
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
