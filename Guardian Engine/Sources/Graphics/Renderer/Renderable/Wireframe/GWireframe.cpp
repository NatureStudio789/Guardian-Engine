#include "GWireframe.h"

namespace GE
{
	GWireframeInstance::GWireframeInstance() : GRenderable()
	{
		this->WireframeInstanceData = {};
	}

	GWireframeInstance::GWireframeInstance(const Data& data)
	{
		this->InitializeWireframeInstance(data);
	}

	GWireframeInstance::GWireframeInstance(const GWireframeInstance& other) : GRenderable(other)
	{
		this->WireframeInstanceData = other.WireframeInstanceData;
	}

	GWireframeInstance::~GWireframeInstance()
	{
		this->WireframeInstanceData.InstanceName.clear();
		this->WireframeInstanceData.Vertices.clear();
		this->WireframeInstanceData.Indices.clear();
	}

	void GWireframeInstance::InitializeWireframeInstance(const Data& data)
	{
		this->WireframeInstanceData = data;

		this->InitializeRenderable(data.InstanceName,
			GVertexBuffer::CreateNewVertexBuffer((void*)data.Vertices.data(), (UINT)data.Vertices.size(), (UINT)sizeof(Vertex)),
			GIndexBuffer::CreateNewIndexBuffer((UINT*)data.Indices.data(), (UINT)data.Indices.size()), 
			GTopology::CreateNewTopology(GTopology::GE_TOPOLOGY_TYPE_TRIANGLELIST));

		auto& WireframeTechnique = GTechnique::CreateNewTechnique("Wireframe", "debug", true);

		{
			auto& WireframeRenderStep = GStep::CreateNewStep("Wireframe");

			WireframeRenderStep->AddApplicable(GTransformCBuffer::CreateNewTransformCBuffer(
				GPipelineStateRegistry::GetPipelineState(GPipelineStateRegistry::WIREFRAME_PSO)->GetPipelineRootSignature()));

			WireframeTechnique->AddStep(WireframeRenderStep);
		}

		this->AddTechnique(WireframeTechnique);
	}

	const GWireframeInstance::Data& GWireframeInstance::GetWireframeInstanceData() const noexcept
	{
		return this->WireframeInstanceData;
	}

	GWireframe::GWireframe()
	{
		this->WireframeData = {};

		this->InstanceList.clear();
	}

	GWireframe::GWireframe(const Data& data)
	{
		this->InitializeWireframe(data);
	}

	GWireframe::GWireframe(const GWireframe& other)
	{
		this->WireframeData = other.WireframeData;
		this->InstanceList = other.InstanceList;
	}

	GWireframe::~GWireframe()
	{
		this->WireframeData.InstanceDataList.clear();

		for (auto& [name, instance] : this->InstanceList)
		{
			instance.reset();
			instance = null;
		}
		this->InstanceList.clear();
	}

	void GWireframe::InitializeWireframe(const Data& data)
	{
		this->WireframeData = data;

		for (auto& instanceData : this->WireframeData.InstanceDataList)
		{
			this->InstanceList[instanceData.InstanceName] = GWireframeInstance::CreateNewWireframeInstance(instanceData);
		}

		this->LinkTechnique("SceneEdit");
		this->LinkTechnique("SceneRuntime");
	}

	void GWireframe::SetTransform(GTransform transform)
	{
		this->WireframeTransform = transform;
	}

	void GWireframe::Submit(const std::string& channel)
	{
		for (auto& [name, instance] : this->InstanceList)
		{
			instance->SetAccumulatedMatrix(this->WireframeTransform.GetTransformMatrix());

			instance->Submit(channel);
		}
	}

	void GWireframe::LinkTechnique(std::string renderGraphName)
	{
		for (auto& [name, instance] : this->InstanceList)
		{
			instance->LinkTechnique(renderGraphName);
		}
	}
	
	const GWireframe::Data& GWireframe::GetWireframeData() const noexcept
	{
		return this->WireframeData;
	}

	std::shared_ptr<GWireframeInstance> GWireframe::GetWireframeInstance(const std::string& name)
	{
		if (!this->InstanceList.count(name))
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("The instance named '{}' doesn't exist in the wireframe!"));
		}

		return this->InstanceList[name];
	}

	GGeometryWireframe::GGeometryWireframe() : GWireframe()
	{
		this->Geometry = null;
	}

	GGeometryWireframe::GGeometryWireframe(std::shared_ptr<GGeometry> geometry)
	{
		this->InitializeGeometryWireframe(geometry);
	}

	GGeometryWireframe::GGeometryWireframe(const GGeometryWireframe& other) : GWireframe(other)
	{
		this->Geometry = other.Geometry;
	}

	GGeometryWireframe::~GGeometryWireframe()
	{
		this->Geometry.reset();
	}

	void GGeometryWireframe::InitializeGeometryWireframe(std::shared_ptr<GGeometry> geometry)
	{
		this->Geometry = geometry;

		Data InitialData;
		for (auto& geometryData : this->Geometry->GeometryDataList)
		{
			std::vector<GWireframeInstance::Vertex> Vertices;

			for (auto& vertex : geometryData.Vertices)
			{
				Vertices.push_back(GWireframeInstance::Vertex(vertex));
			}

			InitialData.InstanceDataList.push_back(GWireframeInstance::Data(geometryData.PartName, Vertices, geometryData.Indices));
		}
		
		this->InitializeWireframe(InitialData);
	}

	void GGeometryWireframe::Submit(const std::string& channel)
	{
		for (auto& geometryData : this->Geometry->GeometryDataList)
		{
			this->GetWireframeInstance(geometryData.PartName)->SetTransform(geometryData.LocalTransform);
		}

		GWireframe::Submit(channel);
	}

	const GGeometry::Category& GGeometryWireframe::GetGeometryCategory() const noexcept
	{
		return this->Geometry->GetGeometryCategory();
	}

	std::shared_ptr<GGeometry> GGeometryWireframe::GetGeometryObject()
	{
		return this->Geometry;
	}
}
