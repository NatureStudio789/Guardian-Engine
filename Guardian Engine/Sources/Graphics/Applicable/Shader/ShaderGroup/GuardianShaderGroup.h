#ifndef _GE_GUARDIANSHADERGROUP_H_
#define _GE_GUARDIANSHADERGROUP_H_
#include "../GuardianPixelShader.h"

namespace GE
{
	class GuardianShaderGroup : public GuardianApplicable
	{
	public:
		GuardianShaderGroup();
		GuardianShaderGroup(std::shared_ptr<GuardianGraphics> graphics, const GString& name, std::shared_ptr<GuardianVertexShader> vs,
			D3D11_INPUT_ELEMENT_DESC layoutData[], int dataSize, std::shared_ptr<GuardianPixelShader> ps);
		GuardianShaderGroup(const GuardianShaderGroup& other);
		~GuardianShaderGroup() override;

		void InitializeShaderGroup(std::shared_ptr<GuardianGraphics> graphics, const GString& name, std::shared_ptr<GuardianVertexShader> vs,
			D3D11_INPUT_ELEMENT_DESC layoutData[], int dataSize, std::shared_ptr<GuardianPixelShader> ps);

		void SetGroupName(const GString& name);
		void SetVertexShader(std::shared_ptr<GuardianVertexShader> shader, 
			D3D11_INPUT_ELEMENT_DESC layoutData[], int dataSize, std::shared_ptr<GuardianGraphics> graphics);
		void SetPixelShader(std::shared_ptr<GuardianPixelShader> shader);

		void Apply(std::shared_ptr<GuardianGraphics> graphics) override;

		const GuardianUUID& GetGroupId() const noexcept;
		const GString& GetGroupName() const noexcept;

	private:
		GuardianUUID GroupId;
		GString GroupName;

		std::shared_ptr<GuardianVertexShader> GroupVertexShader;
		std::shared_ptr<GuardianInputLayout> GroupInputLayout;
		std::shared_ptr<GuardianPixelShader> GroupPixelShader;
	};
}

#endif