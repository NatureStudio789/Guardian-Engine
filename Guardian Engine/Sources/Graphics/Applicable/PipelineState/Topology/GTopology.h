#ifndef _GE_GTOPOLOGY_H_
#define _GE_GTOPOLOGY_H_
#include "../InputLayout/GInputLayout.h"

namespace GE
{
	class GUARDIAN_API GTopology : public GApplicable
	{
	public:
		enum Type
		{
			GE_TOPOLOGY_TYPE_NONE = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED,
			GE_TOPOLOGY_TYPE_POINTLIST = D3D_PRIMITIVE_TOPOLOGY_POINTLIST,
			GE_TOPOLOGY_TYPE_LINELIST = D3D_PRIMITIVE_TOPOLOGY_LINELIST,
			GE_TOPOLOGY_TYPE_LINESTRIP = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP,
			GE_TOPOLOGY_TYPE_TRIANGLELIST = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			GE_TOPOLOGY_TYPE_TRIANGLESTRIP = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		};

	public:
		GTopology();
		GTopology(const Type& type);
		GTopology(const GTopology& other);
		~GTopology();

		void SetTopologyType(const Type& type);

		void Apply() override;

		const Type& GetTopologyType() const noexcept;

		static std::shared_ptr<GTopology> CreateNewTopology(const Type& type)
		{
			return std::make_shared<GTopology>(type);
		}

	private:
		Type TopologyType;
	};
}

#endif