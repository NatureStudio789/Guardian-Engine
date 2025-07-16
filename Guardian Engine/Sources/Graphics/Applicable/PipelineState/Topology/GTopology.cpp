#include "GTopology.h"

namespace GE
{
	GTopology::GTopology()
	{
		this->TopologyType = GE_TOPOLOGY_TYPE_NONE;
	}

	GTopology::GTopology(const Type& type)
	{
		this->SetTopologyType(type);
	}

	GTopology::GTopology(const GTopology& other)
	{
		this->TopologyType = other.TopologyType;
	}

	GTopology::~GTopology()
	{
		this->TopologyType = GE_TOPOLOGY_TYPE_NONE;
	}

	void GTopology::SetTopologyType(const Type& type)
	{
		this->TopologyType = type;
	}

	void GTopology::Apply()
	{
		GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsCommandList()->GetCommandListObject()->
			IASetPrimitiveTopology((D3D12_PRIMITIVE_TOPOLOGY)this->TopologyType);
	}

	const GTopology::Type& GTopology::GetTopologyType() const noexcept
	{
		return this->TopologyType;
	}
}
