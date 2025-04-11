#include "GuardianMesh.h"
#include "../../Renderer/GuardianRenderer.h"
#include "../../../Application/GuardianApplication.h"

namespace GE
{
	GuardianMesh::GuardianMesh()
	{
		this->MeshInstancesList.clear();
		this->MeshName.clear();
	}

	GuardianMesh::GuardianMesh(const GuardianMesh& other)
	{
		this->MeshName = other.MeshName;
		this->MeshInstancesList = other.MeshInstancesList;
	}

	GuardianMesh::~GuardianMesh()
	{
		for (auto& instance : this->MeshInstancesList)
		{
			instance.reset();
			instance = null;
		}
		this->MeshInstancesList.clear();
		this->MeshName.clear();
	}

	void GuardianMesh::InitializeMesh(std::shared_ptr<GuardianGraphics> graphics,
		const GString& meshName, std::vector<GuardianMeshInstance::Data> instanceData)
	{
		this->SetMeshName(meshName);

		for (auto& data : instanceData)
		{
			this->AddInstanceToMesh(graphics, data);
		}
	}

	void GuardianMesh::InitializeMesh(std::shared_ptr<GuardianGraphics> graphics,
		const GString& meshName, const GString& meshFilePath)
	{
		if (std::filesystem::path(meshFilePath).extension() == ".obj" || 
			std::filesystem::path(meshFilePath).extension() == ".fbx")
		{
			this->InitializeMesh(graphics, meshName, GuardianModelImporter(graphics, meshFilePath).GetModelMeshInstanceDataList());
		}
		else if (std::filesystem::path(meshFilePath).extension() == ".gmesh")
		{
			this->MeshName = meshName;
			/*this->Deserialize(meshFilePath);*/
		}
	}

	void GuardianMesh::SetMeshName(const GString& meshName)
	{
		this->MeshName = meshName;
	}

	void GuardianMesh::AddInstanceToMesh(
		std::shared_ptr<GuardianGraphics> graphics, const GuardianMeshInstance::Data& instanceData)
	{
		auto& instance = std::make_shared<GuardianMeshInstance>();
		instance->InitializeMeshInstance(graphics, instanceData);

		this->MeshInstancesList.push_back(instance);
	}

	void GuardianMesh::UpdateMeshTransform(XMMATRIX worldMatrix)
	{
		for (auto& instance : this->MeshInstancesList)
		{
			instance->UpdateMeshInstanceTransform(worldMatrix);
		}
	}

	void GuardianMesh::UpdateMeshLighting(
		std::shared_ptr<GuardianGraphics> graphics, GuardianLightProperties properties)
	{
		for (auto& instance : this->MeshInstancesList)
		{
			instance->UpdateMeshInstanceLighting(graphics, properties);
		}
	}

	void GuardianMesh::SubmitToRenderer(const GString& submitFramebuffer)
	{
		for (auto& instance : this->MeshInstancesList)
		{
			GuardianRenderer::SubmitRenderable(GE_SUBMIT_DEFAULT3D, submitFramebuffer, instance);
		}
	}

	/*const GString GuardianMesh::Serialize()
	{
		YAML::Emitter MeshOutput;
		MeshOutput << YAML::BeginMap;
		MeshOutput << YAML::Key << "Mesh";
		MeshOutput << YAML::Value << "Unnamed";
		MeshOutput << YAML::Key << "Mesh Instances";
		MeshOutput << YAML::Value << YAML::BeginSeq;

		for (auto& instance : this->MeshInstancesList)
		{
			MeshOutput << YAML::BeginMap;
			MeshOutput << YAML::Key << "Mesh Instance";
			MeshOutput << YAML::Value << instance->GetRenderableId();

			MeshOutput << YAML::Key << "Name";
			MeshOutput << YAML::Value << instance->GetMeshInstanceData().MeshInstanceName;
			
			MeshOutput << YAML::Key << "Material";
			MeshOutput << YAML::Value << instance->GetMeshInstanceData().MaterialId;

			MeshOutput << YAML::Key << "Vertices";
			MeshOutput << YAML::Value << YAML::BeginSeq;

			for (auto& vertex : instance->GetMeshInstanceData().VertexData)
			{
				MeshOutput << YAML::BeginMap;

				MeshOutput << YAML::Key << "Position" << YAML::Value << vertex.Position;
				MeshOutput << YAML::Key << "Texture Coord" << YAML::Value << vertex.TextureCoord;
				MeshOutput << YAML::Key << "Normal" << YAML::Value << vertex.Normal;

				MeshOutput << YAML::EndMap;
			}

			MeshOutput << YAML::EndSeq;

			MeshOutput << YAML::Key << "Indices";
			MeshOutput << YAML::Value << YAML::BeginSeq;

			for (int i = 0; i < (int)instance->GetMeshInstanceData().IndexData.size(); i += 3)
			{
				MeshOutput << YAML::BeginMap;

				MeshOutput << YAML::Value << GVector3(
					(float)instance->GetMeshInstanceData().IndexData[i],
					(float)instance->GetMeshInstanceData().IndexData[i + 1],
					(float)instance->GetMeshInstanceData().IndexData[i + 2]);

				MeshOutput << YAML::EndMap;
			}

			MeshOutput << YAML::EndSeq;

			MeshOutput << YAML::EndMap;
		}

		MeshOutput << YAML::EndSeq;

		MeshOutput << YAML::EndMap;

		GString OutputData = MeshOutput.c_str();

		return OutputData;
	}

	void GuardianMesh::Deserialize(const GString& filePath)
	{
		std::ifstream MeshFile(filePath);
		std::stringstream MeshFileStringStream;
		MeshFileStringStream << MeshFile.rdbuf();

		auto& MeshInput = YAML::Load(MeshFileStringStream.str());
		auto instances = MeshInput["Mesh Instances"];
		if (instances)
		{
			for (auto instance : instances)
			{
				auto& ins = std::make_shared<GuardianMeshInstance>();
				GuardianMeshInstance::Data instanceData;

				auto meshInstance = instance["Mesh Instance"].as<uint64_t>();
				ins->RenderableId = meshInstance;

				instanceData.MeshInstanceName = instance["Name"].as<GString>();

				instanceData.MaterialId = instance["Material"].as<uint64_t>();

				auto vertices = instance["Vertices"];
				for (auto vertex : vertices)
				{
					GuardianMeshInstance::Vertex v;
					v.Position = vertex["Position"].as<GVector3>();
					v.TextureCoord = vertex["Texture Coord"].as<GVector2>();
					v.Normal = vertex["Normal"].as<GVector3>();

					instanceData.VertexData.push_back(v);
				}

				auto indices = instance["Indices"];
				for (auto index : indices)
				{
					auto face = index.as<GVector3>();

					instanceData.IndexData.push_back((UINT)face.x);
					instanceData.IndexData.push_back((UINT)face.y);
					instanceData.IndexData.push_back((UINT)face.z);
				}

				ins->InitializeMeshInstance(GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext(), instanceData);

				this->MeshInstancesList.push_back(ins);
			}
		}
	}*/

	std::shared_ptr<GuardianMeshInstance> GuardianMesh::GetMeshInstance(const GuardianUUID& id)
	{
		for (auto& instance : this->MeshInstancesList)
		{
			if (instance->GetRenderableId() == id)
			{
				return instance;
			}
		}

		throw GUARDIAN_ERROR_EXCEPTION("Failed to find the mesh instance with id : '" + std::to_string(id) + "' !");
	}

	const GString& GuardianMesh::GetMeshName() const noexcept
	{
		return this->MeshName;
	}
}
