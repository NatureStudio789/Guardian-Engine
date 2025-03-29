#include "GuardianTopology.h"

namespace GE
{
	GuardianTopology::GuardianTopology()
	{
		this->TopologyCategory = GE_TOPOLOGY_NONE;
	}

	GuardianTopology::GuardianTopology(const GuardianTopologyCategory& topology)
	{
		this->SetTopologyCategory(topology);
	}

	GuardianTopology::GuardianTopology(const GuardianTopology& other)
	{
		this->TopologyCategory = other.TopologyCategory;
	}

	void GuardianTopology::SetTopologyCategory(const GuardianTopologyCategory& topology)
	{
		this->TopologyCategory = topology;
	}

	void GuardianTopology::Apply(std::shared_ptr<GuardianGraphics> graphics)
	{
		graphics->GetGraphicsDeviceContext()->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)this->TopologyCategory);
	}

	const GuardianTopologyCategory& GuardianTopology::GetTopologyCategory() const noexcept
	{
		return this->TopologyCategory;
	}

	std::shared_ptr<GuardianTopology> GuardianTopology::CreateNewTopology(const GuardianTopologyCategory& topology)
	{
		return std::make_shared<GuardianTopology>(topology);
	}
}
