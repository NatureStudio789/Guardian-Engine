#ifndef _GE_GUARDIANRASTERIZERSTATE_H_
#define _GE_GUARDIANRASTERIZERSTATE_H_
#include "../Topology/GuardianTopology.h"

namespace GE
{
	enum GUARDIAN_API GuardianFillMode
	{
		GE_FILL_WIREFRAME = 2,
		GE_FILL_SOLID = 3
	};

	enum GUARDIAN_API GuardianCullMode
	{
		GE_CULL_NONE = 1,
		GE_CULL_FRONT = 2,
		GE_CULL_BACK = 3
	};

	class GUARDIAN_API GuardianRasterizerState : public GuardianApplicable
	{
	public:
		GuardianRasterizerState();
		GuardianRasterizerState(std::shared_ptr<GuardianGraphics> graphics,
			const GuardianFillMode& fillMode, const GuardianCullMode& cullMode);
		GuardianRasterizerState(const GuardianRasterizerState& other);
		~GuardianRasterizerState() override = default;

		void InitializeRasterizerState(std::shared_ptr<GuardianGraphics> graphics,
			const GuardianFillMode& fillMode, const GuardianCullMode& cullMode);

		void Apply(std::shared_ptr<GuardianGraphics> graphics) override;

		WRL::ComPtr<ID3D11RasterizerState> GetRasterizerStateObject() noexcept;
		const GuardianFillMode& GetRasterizerStateFillMode() const noexcept;
		const GuardianCullMode& GetRasterizerStateCullMode() const noexcept;

		static std::shared_ptr<GuardianRasterizerState> CreateNewRasterizerState(std::shared_ptr<GuardianGraphics> graphics, 
			const GuardianFillMode& fillMode, const GuardianCullMode& cullMode);

	private:
		WRL::ComPtr<ID3D11RasterizerState> RasterizerStateObject;
		GuardianFillMode RasterizerStateFillMode;
		GuardianCullMode RasterizerStateCullMode;
	};
}

#endif