#ifndef _GE_GWIREFRAME_H_
#define _GE_GWIREFRAME_H_
#include "../GRenderable.h"

namespace GE
{
	class GUARDIAN_API GWireframeInstance : public GRenderable
	{
	public:
		struct Vertex
		{
			Vertex() = default;
			Vertex(const GVector3& position)
			{
				this->Position = position;
			}
			Vertex(const Vertex&) = default;

			GVector3 Position;
		};

		struct Data
		{
			Data() = default;
			Data(std::string instanceName, std::vector<Vertex> vertices, std::vector<UINT> indices)
			{
				this->InstanceName = instanceName;
				this->Vertices = vertices;
				this->Indices = indices;
			}
			Data(const Data&) = default;

			std::string InstanceName;
			std::vector<Vertex> Vertices;
			std::vector<UINT> Indices;
		};

	public:
		GWireframeInstance();
		GWireframeInstance(const Data& data);
		GWireframeInstance(const GWireframeInstance& other);
		~GWireframeInstance() override;

		void InitializeWireframeInstance(const Data& data);

		const Data& GetWireframeInstanceData() const noexcept;

		static std::shared_ptr<GWireframeInstance> CreateNewWireframeInstance(const Data& data)
		{
			return std::make_shared<GWireframeInstance>(data);
		}

	private:
		Data WireframeInstanceData;
	};

	class GUARDIAN_API GWireframe
	{
	public:
		struct Data
		{
			Data() = default;
			Data(const Data&) = default;

			std::vector<GWireframeInstance::Data> InstanceDataList;
		};

	public:
		GWireframe();
		GWireframe(const Data& data);
		GWireframe(const GWireframe& other);
		virtual ~GWireframe();

		void InitializeWireframe(const Data& data);

		void SetTransform(GTransform transform);
		virtual void Submit(const std::string& channel);

		void LinkTechnique(std::string renderGraphName);

		const Data& GetWireframeData() const noexcept;
		std::shared_ptr<GWireframeInstance> GetWireframeInstance(const std::string& name);

		static std::shared_ptr<GWireframe> CreateNewWireframe(const Data& data)
		{
			return std::make_shared<GWireframe>(data);
		}

	private:
		Data WireframeData;

		std::map<std::string, std::shared_ptr<GWireframeInstance>> InstanceList;

		GTransform WireframeTransform;
	};

	class GUARDIAN_API GGeometryWireframe : public GWireframe
	{
	public:
		GGeometryWireframe();
		GGeometryWireframe(std::shared_ptr<GGeometry> geometry);
		GGeometryWireframe(const GGeometryWireframe& other);
		~GGeometryWireframe() override;

		void InitializeGeometryWireframe(std::shared_ptr<GGeometry> geometry);

		void Submit(const std::string& channel) override;

		const GGeometry::Category& GetGeometryCategory() const noexcept;
		std::shared_ptr<GGeometry> GetGeometryObject();

		static std::shared_ptr<GGeometryWireframe> CreateNewGeometryWireframe(
			std::shared_ptr<GGeometry> geometry)
		{
			return std::make_shared<GGeometryWireframe>(geometry);
		}

	private:
		std::shared_ptr<GGeometry> Geometry;
	};
}

#endif