#ifndef _GE_RASTERIZERSTATE_H_
#define _GE_RASTERIZERSTATE_H_
#include "../Topology/GTopology.h"

namespace GE
{
	class GUARDIAN_API GRasterizerState
	{
	public:
		enum FillMode
		{
			GE_FILL_SOLID = D3D12_FILL_MODE_SOLID,
			GE_FILL_WIREFRAME = D3D12_FILL_MODE_WIREFRAME
		};

		enum CullMode
		{
			GE_CULL_NONE = D3D12_CULL_MODE_NONE,
			GE_CULL_BACK = D3D12_CULL_MODE_BACK,
			GE_CULL_FRONT = D3D12_CULL_MODE_FRONT,
		};

	public:
		GRasterizerState();
		GRasterizerState(FillMode fill, CullMode cull);
		GRasterizerState(const GRasterizerState& other);
		~GRasterizerState();

		void InitializeRasterizerState(FillMode fill, CullMode cull);

		const FillMode& GetFillMode();
		const CullMode& GetCullMode();

		static std::shared_ptr<GRasterizerState> CreateNewRasterizerState(
			FillMode fill, CullMode cull)
		{
			return std::make_shared<GRasterizerState>(fill, cull);
		}

	private:
		FillMode Fill;
		CullMode Cull;
	};
}

#endif