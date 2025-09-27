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
			GVector3 Vertex;
		};

		struct Data
		{
			Data() = default;
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
		GWireframe(const Data& data, std::string renderGraphName = "Scene");
		GWireframe(const GWireframe& other);
		virtual ~GWireframe();

		void InitializeWireframe(const Data& data, std::string renderGraphName = "Scene");

		void SetTransform(GTransform transform);
		void Submit(const std::string& channel);

		void LinkTechnique(std::string renderGraphName);

		const Data& GetWireframeData() const noexcept;
		std::shared_ptr<GWireframeInstance> GetWireframeInstance(const std::string& name);

		static std::shared_ptr<GWireframe> CreateNewWireframe(const Data& data, std::string renderGraphName = "Scene")
		{
			return std::make_shared<GWireframe>(data, renderGraphName);
		}

	private:
		Data WireframeData;

		std::map<std::string, std::shared_ptr<GWireframeInstance>> InstanceList;

		GTransform WireframeTransform;
	};

	class GUARDIAN_API GGeometryWireframe : public GWireframe
	{
	public:
		enum Category
		{
			GE_GEOMETRY_BOX,
			GE_GEOMETRY_SPHERE,
			GE_GEOMETRY_CAPSULE,
			GE_GEOMETRY_PLANE
		};

		struct Box
		{
			GVector3 EdgeLength;
		};

		struct Sphere
		{
			float Radius;
		};

		struct Capsule
		{
			float Height;
			float HalfSphereRadius;
		};

		struct Plane
		{
			GVector2 EdgeLength;
		};

	public:
		GGeometryWireframe();
		GGeometryWireframe(std::shared_ptr<GGeometry> geometry, std::string renderGraphName = "Scene");
		GGeometryWireframe(const GGeometryWireframe& other);
		~GGeometryWireframe() override;

		void InitializeGeometryWireframe(std::shared_ptr<GGeometry> geometry, std::string renderGraphName = "Scene");

	private:
		Category GeometryCategory;
		std::shared_ptr<GGeometry> Geometry;
	};
}

#endif