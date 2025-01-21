#ifndef _GE_GUARDIANTOPOLOGY_H_
#define _GE_GUARDIANTOPOLOGY_H_
#include "../Shader/GuardianPixelShader.h"

namespace guardian
{
	enum GUARDIAN_API GuardianTopologyCategory
	{
		GE_TOPOLOGY_NONE = 0,

		GE_TOPOLOGY_POINTLIST = 1,
		GE_TOPOLOGY_LINELIST = 2,
		GE_TOPOLOGY_LINESTRIP = 3,
		GE_TOPOLOGY_TRIANGLELIST = 4,
		GE_TOPOLOGY_TRIANGLESTRIP = 5,
		GE_TOPOLOGY_LINELIST_ADJ = 10,
		GE_TOPOLOGY_LINESTRIP_ADJ = 11,
		GE_TOPOLOGY_TRIANGLELIST_ADJ = 12,
		GE_TOPOLOGY_TRIANGLESTRIP_ADJ = 13,
	};

	class GUARDIAN_API GuardianTopology : public GuardianApplicable
	{
	public:
		GuardianTopology();
		GuardianTopology(const GuardianTopologyCategory& topology);
		GuardianTopology(const GuardianTopology& other);
		~GuardianTopology() override = default;

		void SetTopologyCategory(const GuardianTopologyCategory& topology);

		void Apply(std::shared_ptr<GuardianGraphics> graphics) override;

		const GuardianTopologyCategory& GetTopologyCategory() const noexcept;

		static std::shared_ptr<GuardianTopology> CreateNewTopology(const GuardianTopologyCategory& topology);

	private:
		GuardianTopologyCategory TopologyCategory;
	};
}

#endif