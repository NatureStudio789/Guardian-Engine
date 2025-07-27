#include "GMeshInstance.h"

namespace GE
{
	GMeshInstance::GMeshInstance() : GRenderable()
	{
		this->InstanceData = {};
	}

	GMeshInstance::GMeshInstance(const std::string& name, const Data& data)
	{
		this->InitializeMeshInstance(name, data);
	}

	GMeshInstance::GMeshInstance(const GMeshInstance& other) : GRenderable(other)
	{
		this->InstanceData = other.InstanceData;
	}

	GMeshInstance::~GMeshInstance()
	{
		this->InstanceData.Vertices.clear();
		this->InstanceData.Indices.clear();
	}

	void GMeshInstance::InitializeMeshInstance(const std::string& name, const Data& data)
	{
		this->InstanceData = data;

		this->InitializeRenderable(name,
			GVertexBuffer::CreateNewVertexBuffer((void*)this->InstanceData.Vertices.data(), (UINT)this->InstanceData.Vertices.size(), (UINT)sizeof(Vertex)),
			GIndexBuffer::CreateNewIndexBuffer(this->InstanceData.Indices.data(), (UINT)this->InstanceData.Indices.size()),
			GTopology::CreateNewTopology(GTopology::GE_TOPOLOGY_TYPE_TRIANGLELIST));
	}
}
